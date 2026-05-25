#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace id {
    // ============================================================================
    // Global Parameters
    // ============================================================================
    const juce::ParameterID BYPASS                {"BYPASS", 1};
    const juce::ParameterID INPUT_GAIN            {"INPUT_GAIN", 1};
    const juce::ParameterID OUTPUT_GAIN           {"OUTPUT_GAIN", 1};
    const juce::ParameterID GATE_ENABLED           {"GATE_ENABLED", 1};
    const juce::ParameterID GATE_GAIN  {     "GATE_GAIN", 1};


    // ==========================================================================
    // input pedals
    // ==========================================================================
    // Input eq
    const juce::ParameterID PEDAL_INPUT_EQ_ENABLED { "PEDAL_INPUT_EQ_ENABLED", 1};
    // TODO : remove outdated
    const juce::ParameterID PEDAL_INPUT_EQ_FREQ { "PEDAL_INPUT_EQ_FREQ", 1};
    const juce::ParameterID PEDAL_INPUT_EQ_GAIN { "PEDAL_INPUT_EQ_GAIN", 1};
    const juce::ParameterID PEDAL_INPUT_EQ_Q { "PEDAL_INPUT_EQ_Q", 1};

    // TODO : pas de param bindings pour eux !!!
    const juce:: ParameterID  PEDAL_INPUT_EQ_GAIN_BAND_1 { "PEDAL_INPUT_EQ_GAIN_BAND_1", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_2  { "PEDAL_INPUT_EQ_GAIN_BAND_2", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_3  { "PEDAL_INPUT_EQ_GAIN_BAND_3", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_4  { "PEDAL_INPUT_EQ_GAIN_BAND_4", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_5  { "PEDAL_INPUT_EQ_GAIN_BAND_5", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_6  { "PEDAL_INPUT_EQ_GAIN_BAND_6", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_7  { "PEDAL_INPUT_EQ_GAIN_BAND_7", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_8  { "PEDAL_INPUT_EQ_GAIN_BAND_8", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_9  { "PEDAL_INPUT_EQ_GAIN_BAND_9", 1};
    const juce:: ParameterID PEDAL_INPUT_EQ_GAIN_BAND_10 { "PEDAL_INPUT_EQ_GAIN_BAND_10", 1};

    // Overdrive
    const juce::ParameterID PEDAL_INPUT_OVERDRIVE_ENABLED { "PEDAL_INPUT_OVERDRIVE_ENABLED", 1};
    const juce::ParameterID PEDAL_INPUT_OVERDRIVE_TONE { "PEDAL_INPUT_OVERDRIVE_TONE", 1};
    // TODO : pas de parm binding pour lui
    const juce::ParameterID PEDAL_INPUT_OVERDRIVE_MIX { "PEDAL_INPUT_OVERDRIVE_MIX", 1};
    const juce::ParameterID PEDAL_INPUT_OVERDRIVE_DRIVE { "PEDAL_INPUT_OVERDRIVE_DRIVE", 1};
    const juce::ParameterID PEDAL_INPUT_OVERDRIVE_LEVEL { "PEDAL_INPUT_OVERDRIVE_LEVEL", 1};

    // FUZZ
    const juce::ParameterID PEDAL_INPUT_FUZZ_ENABLED { "PEDAL_INPUT_FUZZ_ENABLED", 1};
    // Todo : pas de parambinding pour lui
    const juce::ParameterID PEDAL_INPUT_FUZZ_MIX { "PEDAL_INPUT_FUZZ_MIX", 1};
    const juce::ParameterID PEDAL_INPUT_FUZZ_TONE { "PEDAL_INPUT_FUZZ_TONE", 1};
    const juce::ParameterID PEDAL_INPUT_FUZZ_LEVEL { "PEDAL_INPUT_FUZZ_LEVEL", 1};
    const juce::ParameterID PEDAL_INPUT_FUZZ_DRIVE { "PEDAL_INPUT_FUZZ_DRIVE", 1};
    // Compressor
    const juce::ParameterID PEDAL_INPUT_COMPRESSOR_ENABLED { "PEDAL_INPUT_COMPRESSOR_ENABLED", 1};
    const juce::ParameterID PEDAL_INPUT_COMPRESSOR_THRESHOLD { "PEDAL_INPUT_COMPRESSOR_THRESHOLD", 1};
    const juce::ParameterID PEDAL_INPUT_COMPRESSOR_ATTACK { "PEDAL_INPUT_COMPRESSOR_ATTACK", 1};
    const juce::ParameterID PEDAL_INPUT_COMPRESSOR_RELEASE { "PEDAL_INPUT_COMPRESSOR_RELEASE", 1};
    const juce::ParameterID PEDAL_INPUT_COMPRESSOR_RATIO { "PEDAL_INPUT_COMPRESSOR_RATIO", 1};
    const juce::ParameterID PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN { "PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN", 1};


    // AMP
    const juce::ParameterID AMP_ENABLED { "AMP_ENABLED", 1};
    const juce::ParameterID AMP_GAIN { "AMP_GAIN", 1};
    const juce::ParameterID AMP_BASS { "AMP_BASS", 1};
    const juce::ParameterID AMP_MIDDLE { "AMP_MIDDLE", 1};
    const juce::ParameterID AMP_TREBLE { "AMP_TREBLE", 1};
    const juce::ParameterID AMP_PRESENCE { "AMP_PRESENCE", 1};
    const juce::ParameterID AMP_OUTPUT_GAIN { "AMP_OUTPUT_GAIN", 1};

    const juce::ParameterID AMP_POST_EQ_ENABLED { "AMP_POST_EQ_ENABLED", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL1 { "AMP_POST_EQ_GAIN_BELL1", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL2 { "AMP_POST_EQ_GAIN_BELL2", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL3 { "AMP_POST_EQ_GAIN_BELL3", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL4 { "AMP_POST_EQ_GAIN_BELL4", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL5 { "AMP_POST_EQ_GAIN_BELL5", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL6 { "AMP_POST_EQ_GAIN_BELL6", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL7 { "AMP_POST_EQ_GAIN_BELL7", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL8 { "AMP_POST_EQ_GAIN_BELL8", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL9 { "AMP_POST_EQ_GAIN_BELL9", 1};
    const juce::ParameterID AMP_POST_EQ_GAIN_BELL10 { "AMP_POST_EQ_GAIN_BELL10", 1};

    // TODO : ADD A POST COMPRESSOR
    // TODO : ADD AN IR LOADER FOR CABINET !
    // Post effect pedals
    const juce::ParameterID PEDAL_OUTPUT_DELAY_ENABLED { "PEDAL_OUTPUT_DELAY_ENABLED", 1};
    const juce::ParameterID PEDAL_OUTPUT_DELAY_TIME_SYNC { "PEDAL_OUTPUT_DELAY_TIME_SYNC", 1};
    const juce::ParameterID PEDAL_OUTPUT_DELAY_TIME { "PEDAL_OUTPUT_DELAY_TIME", 1};
    const juce::ParameterID PEDAL_OUTPUT_DELAY_SYNC_TIME_ENABLED { "PEDAL_OUTPUT_DELAY_SYNC_TIME_ENABLED", 1};
    const juce::ParameterID PEDAL_OUTPUT_DELAY_FEEDBACK { "PEDAL_OUTPUT_DELAY_FEEDBACK", 1};
    const juce::ParameterID PEDAL_OUTPUT_DELAY_MIX { "PEDAL_OUTPUT_DELAY_MIX", 1};
    const juce::ParameterID PEDAL_OUTPUT_DELAY_PING_PONG { "PEDAL_OUTPUT_DELAY_PING_PONG", 1};

    // Reverb effect
    const juce::ParameterID PEDAL_OUTPUT_REVERB_ENABLED { "PEDAL_OUTPUT_REVERB_ENABLED", 1};
    const juce::ParameterID PEDAL_OUTPUT_REVERB_MIX { "PEDAL_OUTPUT_REVERB_MIX", 1};
    const juce::ParameterID PEDAL_OUTPUT_REVERB_DECAY { "PEDAL_OUTPUT_REVERB_DECAY", 1};
    const juce::ParameterID PEDAL_OUTPUT_REVERB_LF_FREQ { "PEDAL_OUTPUT_REVERB_LF_FREQ", 1};
    const juce::ParameterID PEDAL_OUTPUT_REVERB_HP_FREQ { "PEDAL_OUTPUT_REVERB_HP_FREQ", 1};













}