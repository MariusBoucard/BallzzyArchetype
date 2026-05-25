#pragma once
#include <filesystem>
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>
#include "NAM/dsp.h"
#include "BinaryData.h"
#include "../Bones/CompressorPedalFaust.h"
#include "../ParameterSetup.h"
#include "../paramsDeclaration.h"
#include "../../service/PresetManager.h"
#include "../Bones/AmpToneStackFaust.h"
#include "../faustParameterMappers/AmpStackMap.h"

//==============================================================================
class AmpProcessor final : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener {
public:
    //==============================================================================
    AmpProcessor(juce::AudioProcessorValueTreeState &inParameters, ParameterSetup &inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration);

    ~AmpProcessor() override;
void writeFaustParametersToFile()
{
    std::vector<juce::String> params;

    // Use smart pointers for automatic cleanup
    std::unique_ptr<AmpToneStack::AmpToneStack> odDsp(new AmpToneStack::AmpToneStack());
    std::unique_ptr<AmpToneStack::MapUI> odUI(new AmpToneStack::MapUI());

    // Build UIs
    odDsp->buildUserInterface(odUI.get());

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

        mergeMap(odUI->getFullpathMap());

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
       // mParameterSetup.initParametersListener(*this);
        if (!mFaustToneStackUI.get())        mFaustToneStackUI        = std::make_unique<AmpToneStack::MapUI>();
            mModel->ResetAndPrewarm(mSampleRate, mBlockSize);

        mFaustToneStackProcessor =  std::make_unique<AmpToneStack::AmpToneStack>();
        mFaustToneStackProcessor->init(mSampleRate);
        mFaustToneStackProcessor->buildUserInterface(mFaustToneStackUI.get());
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
        auto faustPath = FaustParameterMapping::getAmpStackPath(parameterID);
        if (!faustPath.empty()) {
            float finalValue = newValue;
            mFaustToneStackUI->setParamValue(faustPath, finalValue);
        }
    }
    void setDirectNAMPath(const juce::File& path)
    {
        mDirectNAMPath = path;
        mParameters.state.setProperty("directNAMPath", path.getFullPathName(), nullptr);
    }
    void loadNAMFile(const juce::File& inNAMFile)
    {
        try
        {
            mIsNAMEnabled = false;
            mModel = nam::get_dsp(std::filesystem::path(inNAMFile.getFullPathName().toStdString()));
            setDirectNAMPath(inNAMFile);
            mIsNAMEnabled = true;  // ResetAndPrewarm intentionally NOT called here
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading NAM file: " << e.what() << std::endl;
        }
    }

    juce::File createTemporaryFileFromMemory(const void* binaryData, size_t dataSize, const juce::String& fileName)
    {
        juce::File tempFile = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile(fileName);

        tempFile.deleteFile();
        tempFile.create();

        juce::FileOutputStream outputStream(tempFile);
        if (outputStream.openedOk())
        {
            outputStream.write(binaryData, dataSize);
            outputStream.flush();
        }
        else
        {
            DBG("Failed to create temporary file.");
        }

        return tempFile;
    }


    void loadDefaultNAMFile()
    {
        juce::File tempFile = createTemporaryFileFromMemory(BinaryData::Marshall_nam, BinaryData::Marshall_namSize, "Marshall.nam");

        if (tempFile.existsAsFile())
        {
            loadNAMFile(tempFile);
        }
        else
        {
            DBG("Failed to create temporary file for impulse response.");
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
    std::unique_ptr<nam::DSP> mModel;
    juce::File mDirectNAMPath;

private:
    std::atomic<float> mRmsLevelLeft{0.0f};
    std::atomic<float> mRmsLevelRight{0.0f};
    std::atomic<float> mRmsOutputLevelLeft{0.0f};
    std::atomic<float> mRmsOutputLevelRight{0.0f};
    std::unique_ptr<AmpToneStack::AmpToneStack> mFaustToneStackProcessor;
    std::unique_ptr<AmpToneStack::MapUI> mFaustToneStackUI;
    juce::AudioPlayHead* mParentPlayHead;
    using AudioInputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using AudioOutputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;

    float** inputs;
    float** postHpLp;
    float** outputs;
    float** duckingInput;
    float** duckingOutput;
    bool mIsNAMEnabled;

    int mBlockSize;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpProcessor)
};
