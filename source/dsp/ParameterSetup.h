#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>
#include <mutex>
#include <deque>
#include <functional>

#include "ParameterIDs.hpp"
#include "Bones/FaustMultiheadFeedback.h"



class ParameterSetup : public juce::Thread,
                       public juce::AudioProcessorValueTreeState::Listener,
                       public juce::Timer
{
public:
    using MapperTask = std::function<void()>;

    ParameterSetup(juce::AudioProcessorValueTreeState& inApvts);
    ~ParameterSetup() override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    void initParametersListener(juce::AudioProcessor& inProcessor);

    void setPlayTime(double playTime, double bpm);
private:
    void run() override;
    void timerCallback() override;


private:
    juce::AudioProcessorValueTreeState& mParameters;

    std::atomic<double> mPlayTimeInSeconds { 0.0 };
    std::atomic<double> mCurrentBpm { 100.0 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSetup)
};