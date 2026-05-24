
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

#include "ui/gui.h"
#include "service/PresetManager.h"
#include "dsp/Processor.h"
#include "dsp/ParameterSetup.h"
#include "dsp/ParameterIDs.hpp"
#include "dsp/paramsDeclaration.h"



class PluginAudioProcessor final : public juce::AudioProcessor,
                                   public juce::AudioProcessorValueTreeState::Listener
{
public:

    PluginAudioProcessor();
    ~PluginAudioProcessor() override;



void addInputEffectLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                     parametersDeclaration::Parameters::inputEffectsParams& input_effects_params)
{
    auto input_eq = std::make_unique<juce::AudioParameterBool>(id::PEDAL_INPUT_EQ_ENABLED,"Input EQ enabled", true);


    auto inputEqEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_INPUT_EQ_ENABLED, "Input EQ enabled", true);
    auto inputEqFreq    = std::make_unique<juce::AudioParameterFloat    >(id::PEDAL_INPUT_EQ_FREQ,  "Input EQ freq",  1000.0f, 20000.0f, 1000.0f);
    auto inputEqGain    = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_EQ_GAIN,  "Input EQ gain", -24.0f, 24.0f, 0.0f);
    auto inputEqQ       = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_EQ_Q,     "Input EQ Q",    0.1f, 10.0f, 1.0f);

input_effects_params.inputEq.enabled = inputEqEnabled.get();
input_effects_params.inputEq.freq    = inputEqFreq.get();
input_effects_params.inputEq.gain    = inputEqGain.get();
input_effects_params.inputEq.q       = inputEqQ.get();

layout.add(std::move(inputEqEnabled));
layout.add(std::move(inputEqFreq));
layout.add(std::move(inputEqGain));
layout.add(std::move(inputEqQ));

// Overdrive
auto odEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_INPUT_OVERDRIVE_ENABLED, "Overdrive enabled", false);
auto odTone    = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_OVERDRIVE_TONE, "Overdrive tone", 0.0f, 1.0f, 0.5f);
auto odDrive   = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_OVERDRIVE_DRIVE,"Overdrive drive", 0.0f, 1.0f, 0.5f);
auto odLevel   = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_OVERDRIVE_LEVEL,"Overdrive level", 0.0f, 1.0f, 0.8f);

input_effects_params.overdrive.enabled = odEnabled.get();
input_effects_params.overdrive.tone    = odTone.get();
input_effects_params.overdrive.drive   = odDrive.get();
input_effects_params.overdrive.level   = odLevel.get();

layout.add(std::move(odEnabled));
layout.add(std::move(odTone));
layout.add(std::move(odDrive));
layout.add(std::move(odLevel));

// Fuzz
auto fuzzEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_INPUT_FUZZ_ENABLED, "Fuzz enabled", false);
auto fuzzTone    = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_FUZZ_TONE, "Fuzz tone", 0.0f, 1.0f, 0.5f);
auto fuzzLevel   = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_FUZZ_LEVEL,"Fuzz level", 0.0f, 1.0f, 0.8f);
auto fuzzDrive   = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_FUZZ_DRIVE,"Fuzz drive", 0.0f, 1.0f, 0.7f);

input_effects_params.fuzz.enabled = fuzzEnabled.get();
input_effects_params.fuzz.tone    = fuzzTone.get();
input_effects_params.fuzz.level   = fuzzLevel.get();
input_effects_params.fuzz.drive   = fuzzDrive.get();

layout.add(std::move(fuzzEnabled));
layout.add(std::move(fuzzTone));
layout.add(std::move(fuzzLevel));
layout.add(std::move(fuzzDrive));

// Compressor
auto compEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_INPUT_COMPRESSOR_ENABLED, "Compressor enabled", false);
auto compThresh  = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_COMPRESSOR_THRESHOLD, "Compressor threshold", -60.0f, 0.0f, -24.0f);
auto compAttack  = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_COMPRESSOR_ATTACK,    "Compressor attack (ms)", 0.1f, 200.0f, 10.0f);
auto compRelease = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_COMPRESSOR_RELEASE,   "Compressor release (ms)", 1.0f, 1000.0f, 100.0f);
auto compMix     = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_COMPRESSOR_MIX,       "Compressor mix", 0.0f, 1.0f, 1.0f);
auto compMakeup  = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN,"Compressor makeup", -12.0f, 12.0f, 0.0f);

input_effects_params.compressor.enabled    = compEnabled.get();
input_effects_params.compressor.threshold  = compThresh.get();
input_effects_params.compressor.attack     = compAttack.get();
input_effects_params.compressor.release    = compRelease.get();
input_effects_params.compressor.mix        = compMix.get();
input_effects_params.compressor.makeupGain = compMakeup.get();

layout.add(std::move(compEnabled));
layout.add(std::move(compThresh));
layout.add(std::move(compAttack));
layout.add(std::move(compRelease));
layout.add(std::move(compMix));
layout.add(std::move(compMakeup));
}

// Helper for Movement Function structures
void addAmpLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                       parametersDeclaration::Parameters::amplifierParams& amp)
{
    auto on = std::make_unique<juce::AudioParameterBool>(id::AMP_ENABLED, "Amp enabled" , true);
    amp.enabled = on.get();
    layout.add(std::move(on));

    auto ampGain = std::make_unique<juce::AudioParameterFloat>(id::AMP_GAIN, "Amp Gain", -12.0f, 12.0f, 0.0f);
    amp.gain = ampGain.get();
    layout.add(std::move(ampGain));

    // EQ controls
    auto bass = std::make_unique<juce::AudioParameterFloat>(id::AMP_BASS, "Amp Bass", -12.0f, 12.0f, 0.0f);
    amp.bass = bass.get();
    layout.add(std::move(bass));

    auto middle = std::make_unique<juce::AudioParameterFloat>(id::AMP_MIDDLE, "Amp Middle", -12.0f, 12.0f, 0.0f);
    amp.middle = middle.get();
    layout.add(std::move(middle));

    auto treble = std::make_unique<juce::AudioParameterFloat>(id::AMP_TREBLE, "Amp Treble", -12.0f, 12.0f, 0.0f);
    amp.treble = treble.get();
    layout.add(std::move(treble));

    auto presence = std::make_unique<juce::AudioParameterFloat>(id::AMP_PRESENCE, "Amp Presence", -12.0f, 12.0f, 0.0f);
    amp.presence = presence.get();
    layout.add(std::move(presence));

    // Amp output gain
    auto ampOut = std::make_unique<juce::AudioParameterFloat>(id::AMP_OUTPUT_GAIN, "Amp Output Gain", -24.0f, 24.0f, 0.0f);
    amp.outputGain = ampOut.get();
    layout.add(std::move(ampOut));



}

    void addPostEqLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
        parametersDeclaration::Parameters::postEqParams& postEq) {

    // Post EQ enable
    auto postEqOn = std::make_unique<juce::AudioParameterBool>(id::AMP_POST_EQ_ENABLED, "Amp Post EQ enabled", false);
    postEq.enabled = postEqOn.get();
    layout.add(std::move(postEqOn));


    // 10-band post EQ gains
    auto makeBell = [&](const juce::ParameterID& pid, const char* name, float def)->std::unique_ptr<juce::AudioParameterFloat>
    {
        return std::make_unique<juce::AudioParameterFloat>(pid, name, -12.0f, 12.0f, def);
    };

    auto b1 = makeBell(id::AMP_POST_EQ_GAIN_BELL1,  "Post EQ Bell 1", 0.0f); postEq.bellGain1  = b1.get();  layout.add(std::move(b1));
    auto b2 = makeBell(id::AMP_POST_EQ_GAIN_BELL2,  "Post EQ Bell 2", 0.0f); postEq.bellGain2  = b2.get();  layout.add(std::move(b2));
    auto b3 = makeBell(id::AMP_POST_EQ_GAIN_BELL3,  "Post EQ Bell 3", 0.0f); postEq.bellGain3  = b3.get();  layout.add(std::move(b3));
    auto b4 = makeBell(id::AMP_POST_EQ_GAIN_BELL4,  "Post EQ Bell 4", 0.0f); postEq.bellGain4  = b4.get();  layout.add(std::move(b4));
    auto b5 = makeBell(id::AMP_POST_EQ_GAIN_BELL5,  "Post EQ Bell 5", 0.0f); postEq.bellGain5  = b5.get();  layout.add(std::move(b5));
    auto b6 = makeBell(id::AMP_POST_EQ_GAIN_BELL6,  "Post EQ Bell 6", 0.0f); postEq.bellGain6  = b6.get();  layout.add(std::move(b6));
    auto b7 = makeBell(id::AMP_POST_EQ_GAIN_BELL7,  "Post EQ Bell 7", 0.0f); postEq.bellGain7  = b7.get();  layout.add(std::move(b7));
    auto b8 = makeBell(id::AMP_POST_EQ_GAIN_BELL8,  "Post EQ Bell 8", 0.0f); postEq.bellGain8  = b8.get();  layout.add(std::move(b8));
    auto b9 = makeBell(id::AMP_POST_EQ_GAIN_BELL9,  "Post EQ Bell 9", 0.0f); postEq.bellGain9  = b9.get();  layout.add(std::move(b9));
    auto b10 = makeBell(id::AMP_POST_EQ_GAIN_BELL10,"Post EQ Bell 10", 0.0f); postEq.bellGain10 = b10.get(); layout.add(std::move(b10));

}

    void addPostEffectsLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
        parametersDeclaration::Parameters::outputEffectsParams& outputEffectsParams) {
    auto delayEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_OUTPUT_DELAY_ENABLED, "Delay enabled", false);
    auto delayTimeSync = std::make_unique<juce::AudioParameterBool>(id::PEDAL_OUTPUT_DELAY_TIME_SYNC, "Delay time sync", true);
    auto delayTimeMs = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_DELAY_TIME, "Delay time (ms)", 1.0f, 2000.0f, 500.0f);
    auto delaySyncTimeEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_OUTPUT_DELAY_SYNC_TIME_ENABLED, "Delay sync time enabled", true);
    auto delayFeedback = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_DELAY_FEEDBACK, "Delay feedback", 0.0f, 0.98f, 0.4f);
    auto delayMix = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_DELAY_MIX, "Delay mix", 0.0f, 1.0f, 0.25f);
    auto delayPingPong = std::make_unique<juce::AudioParameterBool>(id::PEDAL_OUTPUT_DELAY_PING_PONG, "Delay ping-pong", false);
    outputEffectsParams.delay.enabled = delayEnabled.get();
    outputEffectsParams.delay.timeSyncEnabled = delayTimeSync.get();
    outputEffectsParams.delay.timeMs = delayTimeMs.get();
    outputEffectsParams.delay.feedback = delayFeedback.get();
    outputEffectsParams.delay.mix = delayMix.get();
    outputEffectsParams.delay.pingPong = delayPingPong.get();

    layout.add(std::move(delayEnabled));
    layout.add(std::move(delayTimeSync));
    layout.add(std::move(delayTimeMs));
    layout.add(std::move(delaySyncTimeEnabled));
    layout.add(std::move(delayFeedback));
    layout.add(std::move(delayMix));
    layout.add(std::move(delayPingPong));

    // Reverb
    auto reverbEnabled = std::make_unique<juce::AudioParameterBool>(id::PEDAL_OUTPUT_REVERB_ENABLED, "Reverb enabled", false);
    auto reverbMix = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_REVERB_MIX, "Reverb mix", 0.0f, 1.0f, 0.2f);
    auto reverbDecay = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_REVERB_DECAY, "Reverb decay", 0.1f, 10.0f, 2.5f);
    auto reverbLf = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_REVERB_LF_FREQ, "Reverb low-frequency cutoff (Hz)", 20.0f, 1000.0f, 200.0f);
    auto reverbHp = std::make_unique<juce::AudioParameterFloat>(id::PEDAL_OUTPUT_REVERB_HP_FREQ, "Reverb high-frequency cutoff (Hz)", 1000.0f, 20000.0f, 12000.0f);

    outputEffectsParams.reverb.enabled = reverbEnabled.get();
    outputEffectsParams.reverb.mix = reverbMix.get();
    outputEffectsParams.reverb.decay = reverbDecay.get();
    outputEffectsParams.reverb.lfFreq = reverbLf.get();
    outputEffectsParams.reverb.hpFreq = reverbHp.get();

    layout.add(std::move(reverbEnabled));
    layout.add(std::move(reverbMix));
    layout.add(std::move(reverbDecay));
    layout.add(std::move(reverbLf));
    layout.add(std::move(reverbHp));
}

void writeFaustParametersToFile() {

    std::vector<juce::String> params;

/*

    juce::File outputFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory)
                                .getChildFile("faust_params.txt");
    juce::FileOutputStream stream(outputFile);

    if (stream.openedOk())
    {
        int i = 0;
        for (const auto& [key, value] : fullPathMap)
        {
            auto name = key;
            params.push_back(name);

            stream.writeText(juce::String(i) + ": " + name + "\n", false, false, nullptr);
            i++;
        }
        stream.flush();
    }
    delete tempDsp;
    delete tempUI;
    */

}

        void addMainParametersLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout, parametersDeclaration::Parameters& parameters) {
    auto bypassParam = std::make_unique<juce::AudioParameterBool>(id::BYPASS, "Bypass", false);
    parameters.bypass = bypassParam.get();
    layout.add(std::move(bypassParam));

    // Input Gain
    auto inGain = std::make_unique<juce::AudioParameterFloat>(id::INPUT_GAIN, "Input Gain", -24.0f, 24.0f, 0.0f);
    parameters.inputGain = inGain.get();
    layout.add(std::move(inGain));

    // Output Gain
    auto outGain = std::make_unique<juce::AudioParameterFloat>(id::OUTPUT_GAIN, "Output Gain", -24.0f, 24.0f, 0.0f);
    parameters.outputGain = outGain.get();
    layout.add(std::move(outGain));

    // Gate Enabled
    auto gateOn = std::make_unique<juce::AudioParameterBool>(id::GATE_ENABLED, "Gate Enabled", false);
    parameters.gateEnabled = gateOn.get();
    layout.add(std::move(gateOn));

    // Gate Gain (threshold/level)
    auto gateG = std::make_unique<juce::AudioParameterFloat>(id::GATE_GAIN, "Gate Gain", -60.0f, 12.0f, 0.0f);
    parameters.gateGain = gateG.get();
    layout.add(std::move(gateG));

    // BPM From Host


    }

juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout(parametersDeclaration::Parameters& parameters)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    //writeFaustParametersToFile();
    addMainParametersLayout(layout, parameters);
    addInputEffectLayout(layout, parameters.inputEffects);
    addAmpLayout(layout, parameters.amplifier);
    addPostEqLayout(layout, parameters.postEq);
    addPostEffectsLayout(layout, parameters.outputEffects);


    return layout;
}
float getTimeFromIndex(float index);



void parameterChanged(const juce::String& parameterID, float newValue) override
{


    }


    void prepareToPlay (double sampleRate, int blockSize) override
{

    mSkeletonProcessor.prepareToPlay(sampleRate, blockSize);



    for (auto* param : getParameters()) {
        // INIT FAUST VALUES !
        // Triggers responsabilities to childrens !
    }
}

    void releaseResources() override {
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override {
        auto editor = new VueProcessorEditor(mSkeletonProcessor);
        return editor;
    }
    bool hasEditor() const override                        { return true;   }

    const juce::String getName() const override                  { return "Template"; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }

    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const juce::String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const juce::String&) override   {}


    void getStateInformation (juce::MemoryBlock& destData) override
    {
        juce::MemoryOutputStream stream(destData, true);
        mParameters.state.writeToStream(stream);  
    }

    void setStateInformation(const void* data, int sizeInBytes) override
    {
        const auto xml = getXmlFromBinary(data, sizeInBytes); // JUCE helper
        if (xml == nullptr) return;

        const auto newState = juce::ValueTree::fromXml(*xml);
        if (!newState.isValid()) return;

        // Restore the preset name if present
        if (newState.hasProperty("currentPreset"))
            mPresetManager->loadPreset(newState["currentPreset"].toString());
        else
            mParameters.replaceState(newState);
    }

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        const auto& mainInLayout  = layouts.getChannelSet (true,  0);
        const auto& mainOutLayout = layouts.getChannelSet (false, 0);

        return (mainInLayout == mainOutLayout && (! mainInLayout.isDisabled()));
    }

    juce::AudioProcessorValueTreeState& getCustomParameterTree()
    {
        return mParameters;
    }

private:
    void updateMovmentHeadPosition(int inHeadNumber, juce::AudioPlayHead* playHead);
    void updateMovmentPosition() ;

private:
    parametersDeclaration::Parameters parametersDeclaration;
    std::atomic<double> currentBpm { 120.0 };
    // Use unique_ptr for automatic memory management

    juce::AudioProcessorValueTreeState mParameters;
    std::unique_ptr<PresetManager> mPresetManager;
    SkeletonAudioProcessor mSkeletonProcessor;
    ParameterSetup mParameterSetup;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
