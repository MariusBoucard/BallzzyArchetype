

#include "Processor.h"
#include "ParameterIDs.hpp"


SkeletonAudioProcessor::SkeletonAudioProcessor(juce::AudioProcessorValueTreeState& inParameters,
    ParameterSetup& inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration, PresetManager& inPresetManager)
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
, mInputPedalProcessor(inParameters,inParameterSetup,inParametersDeclaration)
, mOutputPedalProcessor(inParameters,inParameterSetup,inParametersDeclaration)
, mAmpProcessor(inParameters,inParameterSetup,inParametersDeclaration)
, mPostEqProcessor(inParameters,inParameterSetup,inParametersDeclaration)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mParametersDeclaration(inParametersDeclaration)
    , mPresetManager(inPresetManager)
{
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
}

SkeletonAudioProcessor::~SkeletonAudioProcessor()
{

}

void SkeletonAudioProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels)
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

void SkeletonAudioProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer& inMidiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = inBuffer.getNumSamples();
    const int numIn = getTotalNumInputChannels();
    const int numOut = getTotalNumOutputChannels();

    if (numSamples > mBlockSize) {
        return;
    }

    const float inGain = mParameters.getRawParameterValue(id::INPUT_GAIN.getParamID())->load();
    const float outGain = mParameters.getRawParameterValue(id::OUTPUT_GAIN.getParamID())->load();


    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(inBuffer);
    inBuffer.applyGain(juce::Decibels::decibelsToGain ((float)inGain));
    updateMeter(false, inBuffer, numIn);

    inBuffer.applyGain(juce::Decibels::decibelsToGain ((float)outGain));
    updateMeter(true, inBuffer, numOut);
}

