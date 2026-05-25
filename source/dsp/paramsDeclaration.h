#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

namespace parametersDeclaration{
    struct Parameters {
        struct inputEffectsParams {
            struct eqPedal
            {
                juce::AudioParameterBool* enabled { nullptr };
                juce::AudioParameterFloat* freq   { nullptr };
                juce::AudioParameterFloat* gain   { nullptr };
                juce::AudioParameterFloat* q      { nullptr };

                juce::AudioParameterFloat* freqBell1   { nullptr };
                juce::AudioParameterFloat* freqBell2   { nullptr };
                juce::AudioParameterFloat* freqBell3   { nullptr };
                juce::AudioParameterFloat* freqBell4   { nullptr };
                juce::AudioParameterFloat* freqBell5   { nullptr };
                juce::AudioParameterFloat* freqBell6   { nullptr };
                juce::AudioParameterFloat* freqBell7   { nullptr };
                juce::AudioParameterFloat* freqBell8   { nullptr };
                juce::AudioParameterFloat* freqBell9   { nullptr };
                juce::AudioParameterFloat* freqBell10   { nullptr };

            } inputEq;
 struct overdrivePedal
        {
            juce::AudioParameterBool* enabled { nullptr };
            juce::AudioParameterFloat* tone   { nullptr };
            juce::AudioParameterFloat* drive  { nullptr };
            juce::AudioParameterFloat* level  { nullptr };
            juce::AudioParameterFloat* mix   { nullptr };
        } overdrive;

        struct fuzzPedal
        {
            juce::AudioParameterBool* enabled { nullptr };
            juce::AudioParameterFloat* tone   { nullptr };
            juce::AudioParameterFloat* level  { nullptr };
            juce::AudioParameterFloat* drive  { nullptr };
        } fuzz;

        struct compressorPedal
        {
            juce::AudioParameterBool* enabled   { nullptr };
            juce::AudioParameterFloat* threshold { nullptr };
            juce::AudioParameterFloat* attack    { nullptr };
            juce::AudioParameterFloat* release   { nullptr };
            juce::AudioParameterFloat* ratio       { nullptr };
            juce::AudioParameterFloat* makeupGain{ nullptr };
        } compressor;
    } inputEffects;

    struct amplifierParams
    {
        juce::AudioParameterBool* enabled     { nullptr };
        juce::AudioParameterFloat* gain       { nullptr }; // amp gain / drive
        juce::AudioParameterFloat* bass       { nullptr };
        juce::AudioParameterFloat* middle     { nullptr };
        juce::AudioParameterFloat* treble     { nullptr };
        juce::AudioParameterFloat* presence   { nullptr };
        juce::AudioParameterFloat* outputGain { nullptr };
    } amplifier;

    struct postEqParams
    {
        juce::AudioParameterBool* enabled { nullptr };
        // 10-band post EQ gains (bell filters)
        juce::AudioParameterFloat* bellGain1  { nullptr };
        juce::AudioParameterFloat* bellGain2  { nullptr };
        juce::AudioParameterFloat* bellGain3  { nullptr };
        juce::AudioParameterFloat* bellGain4  { nullptr };
        juce::AudioParameterFloat* bellGain5  { nullptr };
        juce::AudioParameterFloat* bellGain6  { nullptr };
        juce::AudioParameterFloat* bellGain7  { nullptr };
        juce::AudioParameterFloat* bellGain8  { nullptr };
        juce::AudioParameterFloat* bellGain9  { nullptr };
        juce::AudioParameterFloat* bellGain10 { nullptr };
    } postEq;

    struct outputEffectsParams
    {
        struct delayPedal
        {
            juce::AudioParameterBool* enabled           { nullptr };
            juce::AudioParameterBool* timeSyncEnabled   { nullptr }; // sync controls
            juce::AudioParameterFloat* timeMs           { nullptr };
            juce::AudioParameterFloat* feedback         { nullptr };
            juce::AudioParameterFloat* mix              { nullptr };
            juce::AudioParameterBool* pingPong         { nullptr };
        } delay;

        struct reverbPedal
        {
            juce::AudioParameterBool* enabled { nullptr };
            juce::AudioParameterFloat* mix    { nullptr };
            juce::AudioParameterFloat* decay  { nullptr };
            juce::AudioParameterFloat* lfFreq { nullptr };
            juce::AudioParameterFloat* hpFreq { nullptr };
        } reverb;
    } outputEffects;

    // Global / top-level params
    juce::AudioParameterFloat* inputGain    { nullptr };
    juce::AudioParameterBool* bypass        { nullptr };
    juce::AudioParameterFloat* outputGain   { nullptr };

    // Gate-related
    juce::AudioParameterBool* gateEnabled   { nullptr };
    juce::AudioParameterFloat* gateGain     { nullptr };

    // BPM / sync flag (host-derived)
    juce::AudioParameterBool* bpmFromHost   { nullptr };
};
}
