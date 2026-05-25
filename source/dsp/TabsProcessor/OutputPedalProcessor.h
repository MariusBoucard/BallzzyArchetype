#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>
#include "../Bones/CompressorPedalFaust.h"
#include "../ParameterSetup.h"
#include "../paramsDeclaration.h"
#include "../../service/PresetManager.h"
#include "../Bones/DelayPedalFaust.h"
#include "../Bones/ReverbPedalFaust.h"
#include "../faustParameterMappers/PedalDelayMap.h"
#include "../faustParameterMappers/PedalReverbMap.h"


//==============================================================================
class OutputPedalProcessor final : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener

{
public:
    //==============================================================================
    OutputPedalProcessor(juce::AudioProcessorValueTreeState &inParameters, ParameterSetup &inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration);

    ~OutputPedalProcessor() override;
void writeFaustParametersToFile()
{
    std::vector<juce::String> params;

    // Use smart pointers for automatic cleanup
    std::unique_ptr<DelayPedal::DelayPedal> odDsp(new DelayPedal::DelayPedal());
    std::unique_ptr<DelayPedal::MapUI> odUI(new DelayPedal::MapUI());

    std::unique_ptr<ReverbPedal::ReverbPedal> fuzzDsp(new ReverbPedal::ReverbPedal());
    std::unique_ptr<ReverbPedal::MapUI> fuzzUI(new ReverbPedal::MapUI());

    // Build UIs
    odDsp->buildUserInterface(odUI.get());
    fuzzDsp->buildUserInterface(fuzzUI.get());

    // Collect maps (assumes getFullpathMap() returns something like std::map<juce::String, someValue> or std::unordered_map)
    // Adjust types if needed.
    using MapType = decltype(odUI->getFullpathMap());
        auto combinedMap = odUI->getFullpathMap();

        auto mergeMap = [&](const decltype(combinedMap)& m)
        {
            for (const auto& kv : m)
            {
                if (combinedMap.find(kv.first) == combinedMap.end())
                    combinedMap.insert(kv);
            }
        };

        mergeMap(fuzzUI->getFullpathMap());


    mergeMap(odUI->getFullpathMap());


    // Write to desktop file
    juce::File outputFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory)
                                .getChildFile("faust_params.txt");
    juce::FileOutputStream stream(outputFile);

    if (stream.openedOk())
    {
        int i = 0;
        for (const auto& kv : combinedMap)
        {
            const juce::String& name = kv.first;
            params.push_back(name);
            stream.writeText(juce::String(i) + ": " + name + "\n", false, false, nullptr);
            ++i;
        }
        stream.flush();
    }
}

    void prepareToPlay(double inSampleRate, int inBlockSize) override {
        mSampleRate = inSampleRate;
        mBlockSize = inBlockSize;
        mParameterSetup.initParametersListener(*this);
    if (!mDelayUi.get())        mDelayUi        = std::make_unique<DelayPedal::MapUI>();
    if (!mReverbUi.get())        mReverbUi        = std::make_unique<ReverbPedal::MapUI>();
    mDelayProcessor =  std::make_unique<DelayPedal::DelayPedal>();
    mDelayProcessor->init(mSampleRate);
    mDelayProcessor->buildUserInterface(mDelayUi.get());

    mReverbProcessor =  std::make_unique<ReverbPedal::ReverbPedal>();
    mReverbProcessor->init(mSampleRate);
    mReverbProcessor->buildUserInterface(mReverbUi.get());

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
    void parameterChanged(const juce::String& parameterID, float newValue) override {
    auto faustPath = FaustParameterMapping::getPedalDelayPath(parameterID);
    if (!faustPath.empty()) {
        float finalValue = newValue;
        mDelayUi->setParamValue(faustPath, finalValue);
    }
    auto compressorPath = FaustParameterMapping::getReverbPath(parameterID);
    if (!compressorPath.empty()) {
        float finalValue = newValue;
        mReverbUi->setParamValue(compressorPath, finalValue);
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

    juce::AudioProcessorValueTreeState&  getCustomParameterTree() {
        return mParameters;
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
    std::unique_ptr<DelayPedal::DelayPedal> mDelayProcessor;
    std::unique_ptr<DelayPedal::MapUI> mDelayUi;
    std::unique_ptr<ReverbPedal::ReverbPedal> mReverbProcessor;
    std::unique_ptr<ReverbPedal::MapUI> mReverbUi;

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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputPedalProcessor)
};
