#include "PluginAudioProcessor.h"


PluginAudioProcessor::PluginAudioProcessor()
    : juce::AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
          .withOutput("Output", juce::AudioChannelSet::stereo()))
      , mParameters{*this, nullptr, "PARAMETERS", createParameterLayout(parametersDeclaration)}
      , mPresetManager{std::make_unique<PresetManager>(mParameters)}
      , mParameterSetup(mParameters)
      , mSkeletonProcessor(mParameters, mParameterSetup, parametersDeclaration, *mPresetManager) {
    for (auto *param: mParameters.processor.getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }

    mParameterSetup.initParametersListener(*this);
}


PluginAudioProcessor::~PluginAudioProcessor() {
    for (auto *param: getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.removeParameterListener(paramID, this);;
    }
}

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &a) {
    auto *playHead = getPlayHead();
    if (playHead) {
        auto postion = playHead->getPosition();
        if (postion.hasValue() && postion->getBpm().hasValue()) {
            currentBpm.store(*postion->getBpm());
        }
    }
    mSkeletonProcessor.processBlock(buffer, a);
}

