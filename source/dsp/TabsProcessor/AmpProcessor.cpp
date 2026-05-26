

#include "AmpProcessor.h"

#include "../ParameterSetup.h"
#include "../paramsDeclaration.h"
#include "../../service/PresetManager.h"

AmpProcessor::AmpProcessor(juce::AudioProcessorValueTreeState& inParameters,
    ParameterSetup& inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration)
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mParametersDeclaration(inParametersDeclaration)
    , mIsNAMEnabled(false)
{
    for (auto *param: mParameters.processor.getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
    loadDefaultNAMFile();  // loads model only, no prewarm yet
    writeFaustParametersToFile();
}

AmpProcessor::~AmpProcessor()
{

}

void AmpProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels)
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

void AmpProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = inBuffer.getNumSamples();
    const int numOut = getTotalNumOutputChannels();
    const int numChannels = inBuffer.getNumChannels();

    if (numSamples > mBlockSize)
        return;
    const bool inAmpOn = mParameters.getRawParameterValue(id::AMP_ENABLED.getParamID())->load();
    // TODO pour test on bypass pas la toneStack
    //  if (!inAmpOn) return;
    const float inGain  = mParameters.getRawParameterValue(id::AMP_GAIN.getParamID())->load();
    const float outGain = mParameters.getRawParameterValue(id::AMP_OUTPUT_GAIN.getParamID())->load();

    // Apply input gain before NAM
    inBuffer.applyGain(juce::Decibels::decibelsToGain(inGain));

    if (mIsNAMEnabled && mModel && inAmpOn)
    {
        // NAM expects mono float*; use channel 0, copy result back

        const float* readPtr  = inBuffer.getReadPointer(0);
        float*       writePtr = inBuffer.getWritePointer(0);

        // Copy to a double buffer, process, copy back
        std::vector<double> inDouble(readPtr, readPtr + numSamples);
        std::vector<double> outDouble(numSamples, 0.0);

        mModel->process(inDouble.data(), outDouble.data(), numSamples);

        for (int i = 0; i < numSamples; ++i)
            writePtr[i] = static_cast<float>(outDouble[i]);

        // Mirror mono result to other channels
        for (int ch = 1; ch < numOut; ++ch)
            inBuffer.copyFrom(ch, 0, inBuffer, 0, 0, numSamples);

        for (int ch = 0; ch < numChannels; ++ch)
            std::copy_n(inBuffer.getReadPointer(ch), numSamples, inputs[ch]);

        // Compressor is already stereo — one instance, both channels at once

        mFaustToneStackProcessor->compute(numSamples, inputs, duckingInput);
        for (int ch = 0; ch < numOut;  ++ch)
            std::copy_n(duckingInput[ch], numSamples, inputs[ch]);
        for (int ch = 0; ch < numOut; ++ch)
            std::copy_n(inputs[ch], numSamples, inBuffer.getWritePointer(ch));
    }


    inBuffer.applyGain(juce::Decibels::decibelsToGain(outGain));
    // Apply output gain after NAM
   // updateMeter(true, inBuffer, numOut);
}
