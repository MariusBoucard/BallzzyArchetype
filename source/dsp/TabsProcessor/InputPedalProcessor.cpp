

#include "InputPedalProcessor.h"




InputPedalProcessor::InputPedalProcessor(juce::AudioProcessorValueTreeState& inParameters,
    ParameterSetup& inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration)
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mParametersDeclaration(inParametersDeclaration)
{   // TODO : currate pour pas listen tout le monde
    for (auto *param: mParameters.processor.getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
    writeFaustParametersToFile();
}

InputPedalProcessor::~InputPedalProcessor()
{

}

void InputPedalProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels)
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

void InputPedalProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer& inMidiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples  = inBuffer.getNumSamples();
    const int numChannels = inBuffer.getNumChannels();

    if (numSamples > mBlockSize) return;

    const bool isEqOn         = mParameters.getRawParameterValue(id::PEDAL_INPUT_EQ_ENABLED.getParamID())->load();
    const bool isCompressorOn = mParameters.getRawParameterValue(id::PEDAL_INPUT_COMPRESSOR_ENABLED.getParamID())->load();
    const bool isFuzzOn       = mParameters.getRawParameterValue(id::PEDAL_INPUT_FUZZ_ENABLED.getParamID())->load();
    const bool isOverdriveOn  = mParameters.getRawParameterValue(id::PEDAL_INPUT_OVERDRIVE_ENABLED.getParamID())->load();

    for (int ch = 0; ch < numChannels; ++ch)
        std::copy_n(inBuffer.getReadPointer(ch), numSamples, inputs[ch]);

    FAUSTFLOAT* monoIn [1];
    FAUSTFLOAT* monoOut[1];

    // Each channel gets its OWN processor instance → no state bleed
    auto processMono = [&](std::array<std::unique_ptr<dsp>, MAX_CHANNELS>& processors) {
        for (int ch = 0; ch < numChannels; ++ch) {
            monoIn [0] = inputs[ch];
            monoOut[0] = postCompressor[ch];
            processors[ch]->compute(numSamples, monoIn, monoOut);     // <-- per-channel instance
            std::copy_n(postCompressor[ch], numSamples, inputs[ch]);
        }
    };

    if (isFuzzOn)      processMono(mFaustFuzzProcessors);
    if (isOverdriveOn) processMono(mFaustOverdriveProcessors);
    if (isEqOn)        processMono(mEqPedalProcessors);

    // Compressor is already stereo — one instance, both channels at once
    if (isCompressorOn) {
        mFaustCompressorProcessor->compute(numSamples, inputs, postCompressor);
        for (int ch = 0; ch < numChannels; ++ch)
            std::copy_n(postCompressor[ch], numSamples, inputs[ch]);
    }

    for (int ch = 0; ch < numChannels; ++ch)
        std::copy_n(inputs[ch], numSamples, inBuffer.getWritePointer(ch));
}