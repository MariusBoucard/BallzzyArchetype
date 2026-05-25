

#include "PostEqProcessor.h"



PostEqProcessor::PostEqProcessor(juce::AudioProcessorValueTreeState& inParameters,
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
}

PostEqProcessor::~PostEqProcessor()
{

}

void PostEqProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels)
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

void PostEqProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer& inMidiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples  = inBuffer.getNumSamples();
    const int numChannels = inBuffer.getNumChannels();

    if (numSamples > mBlockSize) return;

    const bool isEqOn         = mParameters.getRawParameterValue(id::AMP_POST_EQ_ENABLED.getParamID())->load();
    for (int ch = 0; ch < numChannels; ++ch)
        std::copy_n(inBuffer.getReadPointer(ch), numSamples, inputs[ch]);

    FAUSTFLOAT* monoIn [1];
    FAUSTFLOAT* monoOut[1];

    // Each channel gets its OWN processor instance → no state bleed
    auto processMono = [&](std::array<std::unique_ptr<dsp>, MAX_CHANNELS>& processors) {
        for (int ch = 0; ch < numChannels; ++ch) {
            monoIn [0] = inputs[ch];
            monoOut[0] = outputs[ch];
            processors[ch]->compute(numSamples, monoIn, monoOut);     // <-- per-channel instance
            std::copy_n(outputs[ch], numSamples, inputs[ch]);
        }
    };

    if (isEqOn)        processMono(mEqPedalProcessors);

    for (int ch = 0; ch < numChannels; ++ch)
        std::copy_n(inputs[ch], numSamples, inBuffer.getWritePointer(ch));
}

