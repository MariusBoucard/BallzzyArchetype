#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>
#include "../ParameterSetup.h"
#include "../Bones/CompressorPedalFaust.h" // CHIANT C EST POUR LE DSP QU'on fait ca;...
#include "../paramsDeclaration.h"
#include "../../service/PresetManager.h"
#include "../faustParameterMappers/PostEqMap.h"
#include "../Bones/EqPedalFaust.h"


//==============================================================================
class PostEqProcessor final : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    PostEqProcessor(juce::AudioProcessorValueTreeState &inParameters, ParameterSetup &inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration);

    ~PostEqProcessor() override;

    void prepareToPlay(double inSampleRate, int inBlockSize) override {
        mSampleRate = inSampleRate;
        mBlockSize = inBlockSize;
        mParameterSetup.initParametersListener(*this);
        if (!mEqUi.get())        mEqUi        = std::make_unique<EqPedal::MapUI>();
        for (int ch = 0; ch < MAX_CHANNELS; ++ch)
        {

            mEqPedalProcessors[ch] = std::make_unique<EqPedal::EqPedalEngine>();
            mEqPedalProcessors[ch]->init(mSampleRate);
            mEqPedalProcessors[ch]->buildUserInterface(mEqUi.get());
        }
        inputs = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            inputs[channel] = new float[mBlockSize];
        }
        outputs = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            outputs[channel] = new float[mBlockSize];
        }
        postHpLp  = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            postHpLp[channel] = new float[mBlockSize];
        }

        duckingInput = new float*[4];
        for (int channel = 0; channel < 4; ++channel) {
            duckingInput[channel] = new float[mBlockSize];
        }
        duckingOutput = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            duckingOutput[channel] = new float[mBlockSize];
        }

    }


    void releaseResources() override {

        for (int channel = 0; channel < 2; ++channel) {
            delete[] outputs[channel];
        }
        delete [] outputs;
        for (int channel = 0; channel < 2; ++channel) {
            delete[] inputs[channel];
        }
        delete[] inputs;
        for (int channel = 0; channel < 2; ++channel) {
            delete[] postHpLp[channel];
        }
        delete[] postHpLp;

        for (int channel = 0; channel < 4; ++channel) {
            delete[] duckingInput[channel];
        }
        delete[] duckingInput;
        for (int channel = 0; channel < 2; ++channel) {
            delete[] duckingOutput[channel];
        }
        delete[] duckingOutput;
    }
    juce::AudioProcessorValueTreeState& getState() { return mParameters;}
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &) override;

    void updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels);

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override {
        return nullptr;
    }

    bool hasEditor() const override { return false; }

    //==============================================================================
    const juce::String getName() const override { return "BallzzyDelayProcessor"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int) override {
    }

    void parameterChanged(const juce::String& parameterID, float newValue) override {
        auto faustPath = FaustParameterMapping::getPostAmpEqPath(parameterID);
        if (!faustPath.empty()) {
            float finalValue = newValue;
            mEqUi->setParamValue(faustPath, finalValue);
        }
    }

    const juce::String getProgramName(int) override { return "None"; }

    void changeProgramName(int, const juce::String &) override {
    }


    double getRmsLevelLeft() const {
        auto a = mRmsLevelLeft.load();
        return a;
    }
    double getRmsLevelRight() const { return mRmsLevelRight.load(); }
    double getRmsOutputLevelLeft() const { return mRmsOutputLevelLeft.load(); }
    double getRmsOutputLevelRight() const { return mRmsOutputLevelRight.load(); }

    //==============================================================================
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        const auto &mainInLayout = layouts.getChannelSet(true, 0);
        const auto &mainOutLayout = layouts.getChannelSet(false, 0);

        return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
    }


    void initState() {
    }

    void getStateInformation(juce::MemoryBlock &destData) override {
    }

    void setStateInformation(const void *data, int sizeInBytes) override {
        juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
        auto newState = juce::ValueTree::readFromStream(stream);
    }

    void setRateAndBufferSizeDetails(double sampleRate, int bufferSize) {
        mSampleRate = sampleRate;
        mBlockSize = bufferSize;
    }

private:

    //==============================================================================
    juce::AudioProcessorValueTreeState &mParameters;
    ParameterSetup &mParameterSetup;
    parametersDeclaration::Parameters mParametersDeclaration;

private:
    static constexpr int MAX_CHANNELS = 2;

    std::array<std::unique_ptr<dsp>, MAX_CHANNELS> mEqPedalProcessors;

    std::unique_ptr<EqPedal::MapUI> mEqUi;

    std::atomic<float> mRmsLevelLeft{0.0f};
    std::atomic<float> mRmsLevelRight{0.0f};
    std::atomic<float> mRmsOutputLevelLeft{0.0f};
    std::atomic<float> mRmsOutputLevelRight{0.0f};

    juce::AudioPlayHead* mParentPlayHead;
    using AudioInputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using AudioOutputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;

    float** inputs;
    float** postHpLp;
    float** outputs;
    float** duckingInput;
    float** duckingOutput;

    int mBlockSize;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PostEqProcessor);
};
