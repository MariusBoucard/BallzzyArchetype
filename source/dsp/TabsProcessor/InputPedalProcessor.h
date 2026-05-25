#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

#include "../ParameterSetup.h"
#include "../paramsDeclaration.h"
#include "../../service/PresetManager.h"
#include "../Bones/CompressorPedalFaust.h"
#include "../Bones/EqPedalFaust.h"
#include "../Bones/OverdrivePedalFaust.h"
#include "../Bones/FuzzPedalFaust.h"
#include "../faustParameterMappers/PedalCompressorMap.h"
#include "../faustParameterMappers/PedalEqMap.h"
#include "../faustParameterMappers/PedalFuzzMap.h"
#include "../faustParameterMappers/PedalOverdriveMap.h"


//==============================================================================
class InputPedalProcessor final : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    InputPedalProcessor(juce::AudioProcessorValueTreeState &inParameters, ParameterSetup &inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration);

    ~InputPedalProcessor() override;
    // TODO : Set parameterCallback to set the faust UI.

    void prepareToPlay(double inSampleRate, int inBlockSize) override {
        mSampleRate = inSampleRate;
        mBlockSize = inBlockSize;
       // mParameterSetup.initParametersListener(*this);
        if (!mFuzzUi.get())        mFuzzUi        = std::make_unique<FuzzPedal::MapUI>();
        if (!mFaustOverdriveUi.get())        mFaustOverdriveUi        = std::make_unique<OverdrivePedal::MapUI>();
        if (!mFaustCompressorUi.get())        mFaustCompressorUi        = std::make_unique<MapUI>();
        if (!mEqUi.get())        mEqUi        = std::make_unique<EqPedal::MapUI>();


        for (int ch = 0; ch < MAX_CHANNELS; ++ch)
        {
            mFaustFuzzProcessors[ch] = std::make_unique<FuzzPedal::FuzzPedalEngine>();
            mFaustFuzzProcessors[ch]->init(mSampleRate);
            mFaustFuzzProcessors[ch]->buildUserInterface(mFuzzUi.get());

            mFaustOverdriveProcessors[ch] = std::make_unique<OverdrivePedal::OverdrivePedalEngine>();
            mFaustOverdriveProcessors[ch]->init(mSampleRate);
            mFaustOverdriveProcessors[ch]->buildUserInterface(mFaustOverdriveUi.get());

            mEqPedalProcessors[ch] = std::make_unique<EqPedal::EqPedalEngine>();
            mEqPedalProcessors[ch]->init(mSampleRate);
            mEqPedalProcessors[ch]->buildUserInterface(mEqUi.get());
        }


        mFaustCompressorProcessor =  std::make_unique<CompressorPedalEngine>();
        mFaustCompressorProcessor->init(mSampleRate);
        mFaustCompressorProcessor->buildUserInterface(mFaustCompressorUi.get());


        inputs = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            inputs[channel] = new float[mBlockSize];
        }
        outputs = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            outputs[channel] = new float[mBlockSize];
        }
        postCompressor  = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            postCompressor[channel] = new float[mBlockSize];
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
        auto faustPath = FaustParameterMapping::getInputEqPath(parameterID);
        if (!faustPath.empty()) {
            float finalValue = newValue;
            mEqUi->setParamValue(faustPath, finalValue);
        }
        auto compressorPath = FaustParameterMapping::getPedalCompressorPath(parameterID);
        if (!compressorPath.empty()) {
            float finalValue = newValue;
            mFaustCompressorUi->setParamValue(compressorPath, finalValue);
        }
        auto fuzzPath = FaustParameterMapping::getFuzzPath(parameterID);
        if (!fuzzPath.empty()) {
            float finalValue = newValue;
            mFuzzUi->setParamValue(fuzzPath, finalValue);
        }
        auto overdrivePath = FaustParameterMapping::getOverdrivePath(parameterID);
        if (!overdrivePath.empty()) {
            float finalValue = newValue;
            mFaustOverdriveUi->setParamValue(overdrivePath, finalValue);
        }
    }

void writeFaustParametersToFile()
{
    std::vector<juce::String> params;

    // Use smart pointers for automatic cleanup
    std::unique_ptr<OverdrivePedal::OverdrivePedalEngine> odDsp(new OverdrivePedal::OverdrivePedalEngine());
    std::unique_ptr<OverdrivePedal::MapUI> odUI(new OverdrivePedal::MapUI());

    std::unique_ptr<FuzzPedal::FuzzPedalEngine> fuzzDsp(new FuzzPedal::FuzzPedalEngine());
    std::unique_ptr<FuzzPedal::MapUI> fuzzUI(new FuzzPedal::MapUI());

    std::unique_ptr<EqPedal::EqPedalEngine> eqDsp(new EqPedal::EqPedalEngine());
    std::unique_ptr<EqPedal::MapUI> eqUI(new EqPedal::MapUI());

    std::unique_ptr<CompressorPedalEngine> compDsp(new CompressorPedalEngine());
    std::unique_ptr<MapUI> compUI(new MapUI());

    // Build UIs
    odDsp->buildUserInterface(odUI.get());
    fuzzDsp->buildUserInterface(fuzzUI.get());
    eqDsp->buildUserInterface(eqUI.get());
    compDsp->buildUserInterface(compUI.get());

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
        mergeMap(eqUI->getFullpathMap());
        mergeMap(compUI->getFullpathMap());

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
            delete[] postCompressor[channel];
        }
        delete[] postCompressor;

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

    void parameterChanged() {


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
public:
    // TODO : elle est là mais devrait moov en parametre qq part
    static constexpr int MAX_CHANNELS = 2;
private:

    //==============================================================================
    juce::AudioProcessorValueTreeState &mParameters;
    ParameterSetup &mParameterSetup;
    parametersDeclaration::Parameters mParametersDeclaration;

private:
    std::atomic<float> mRmsLevelLeft{0.0f};
    std::atomic<float> mRmsLevelRight{0.0f};
    std::atomic<float> mRmsOutputLevelLeft{0.0f};
    std::atomic<float> mRmsOutputLevelRight{0.0f};

    juce::AudioPlayHead* mParentPlayHead;
    using AudioInputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using AudioOutputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;

    // TODO : first one to be include add dsp to namespace
    std::unique_ptr<CompressorPedalEngine> mFaustCompressorProcessor;
    std::unique_ptr<MapUI> mFaustCompressorUi;


    std::array<std::unique_ptr<dsp>, MAX_CHANNELS> mFaustFuzzProcessors;
    std::array<std::unique_ptr<dsp>, MAX_CHANNELS> mFaustOverdriveProcessors;
    std::array<std::unique_ptr<dsp>, MAX_CHANNELS> mEqPedalProcessors;
    std::unique_ptr<FuzzPedal::MapUI> mFuzzUi;
    std::unique_ptr<OverdrivePedal::MapUI> mFaustOverdriveUi;
    std::unique_ptr<EqPedal::MapUI> mEqUi;




    float** inputs;
    float** postCompressor;
    float** outputs;
    float** duckingInput;
    float** duckingOutput;

    int mBlockSize;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputPedalProcessor)
};
