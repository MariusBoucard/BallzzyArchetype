#include "ParameterSetup.h"

ParameterSetup::ParameterSetup(juce::AudioProcessorValueTreeState &inApvts)
    : juce::Thread("MappersProcessingThread")
      , mParameters(inApvts)
{

    startTimerHz(60);
}

ParameterSetup::~ParameterSetup() {
    stopTimer();
    stopThread(10);
}


void ParameterSetup::parameterChanged(const juce::String& parameterID, float newValue) {

}

void ParameterSetup::initParametersListener(juce::AudioProcessor& inProcessor) {

    startThread();
}


void ParameterSetup::setPlayTime(double timeInSeconds, double bpm) {
    mCurrentBpm.store(bpm, std::memory_order_relaxed);
    mPlayTimeInSeconds.store(timeInSeconds, std::memory_order_relaxed);
}

void ParameterSetup::timerCallback() {

}

void ParameterSetup::run() {
    // Pas de run ici ?
}
