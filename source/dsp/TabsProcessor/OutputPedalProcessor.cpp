

#include "OutputPedalProcessor.h"


OutputPedalProcessor::OutputPedalProcessor(juce::AudioProcessorValueTreeState& inParameters,
    ParameterSetup& inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration)
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mParametersDeclaration(inParametersDeclaration)
{
    for (auto *param: mParameters.processor.getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
    writeFaustParametersToFile();
}

OutputPedalProcessor::~OutputPedalProcessor()
{

}

void OutputPedalProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels)
{
    if (isOutput)
    {
        if (numChannels > 0)
            mRmsOutputLevelLeft.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (numChannels > 1)
            mRmsOutputLevelRight.store(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    }
    else
    {
        if (numChannels > 0)
            mRmsLevelLeft.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (numChannels > 1)
            mRmsLevelRight.store(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    }
}

void OutputPedalProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer& inMidiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = inBuffer.getNumSamples();

    const int numChannels = inBuffer.getNumChannels();

    if (numSamples > mBlockSize) {
        return;
    }
    const bool isDelayOn         = mParameters.getRawParameterValue(id::PEDAL_OUTPUT_DELAY_ENABLED.getParamID())->load();
    const bool isReverbOn = mParameters.getRawParameterValue(id::PEDAL_OUTPUT_REVERB_ENABLED.getParamID())->load();

    for (int ch = 0; ch < numChannels; ++ch)
        std::copy_n(inBuffer.getReadPointer(ch), numSamples, inputs[ch]);

    if (isDelayOn) {
        mDelayProcessor->compute(numSamples, inputs, duckingInput);
        for (int ch = 0; ch < numChannels; ++ch)
            std::copy_n(duckingInput[ch], numSamples, inputs[ch]);
    }
    if (isReverbOn) {
        mReverbProcessor->compute(numSamples, inputs, duckingInput);
        for (int ch = 0; ch < numChannels; ++ch)
            std::copy_n(duckingInput[ch], numSamples, inputs[ch]);
    }
    for (int ch = 0; ch < numChannels; ++ch)
        std::copy_n(inputs[ch], numSamples, inBuffer.getWritePointer(ch));

}

