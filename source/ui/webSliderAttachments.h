#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class WebSliderAttachments {
public:
    WebSliderAttachments(SkeletonAudioProcessor &p)
        :         processorRef(p),
                                                        webBypassRelay{id::BYPASS.getParamID()},
                                                        webInputGainRelay{id::INPUT_GAIN.getParamID()},
                                                        webOutputGainRelay{id::OUTPUT_GAIN.getParamID()},
                                                        webGateEnabledRelay{id::GATE_ENABLED.getParamID()},
                                                        webGateGainRelay{id::GATE_GAIN.getParamID()},
                                                        webPedalInputEqEnabledRelay{id::PEDAL_INPUT_EQ_ENABLED.getParamID()},
                                                        webPedalInputEqFreqRelay{id::PEDAL_INPUT_EQ_FREQ.getParamID()},
                                                        webPedalInputEqGainRelay{id::PEDAL_INPUT_EQ_GAIN.getParamID()},
                                                        webPedalInputEqQRelay{id::PEDAL_INPUT_EQ_Q.getParamID()},
                                                        webPedalInputOverdriveEnabledRelay{id::PEDAL_INPUT_OVERDRIVE_ENABLED.getParamID()},
                                                        webPedalInputOverdriveToneRelay{id::PEDAL_INPUT_OVERDRIVE_TONE.getParamID()},
                                                        webPedalInputOverdriveDriveRelay{id::PEDAL_INPUT_OVERDRIVE_DRIVE.getParamID()},
                                                        webPedalInputOverdriveLevelRelay{id::PEDAL_INPUT_OVERDRIVE_LEVEL.getParamID()},
                                                        webPedalInputFuzzEnabledRelay{id::PEDAL_INPUT_FUZZ_ENABLED.getParamID()},
                                                        webPedalInputFuzzToneRelay{id::PEDAL_INPUT_FUZZ_TONE.getParamID()},
                                                        webPedalInputFuzzLevelRelay{id::PEDAL_INPUT_FUZZ_LEVEL.getParamID()},
                                                        webPedalInputFuzzDriveRelay{id::PEDAL_INPUT_FUZZ_DRIVE.getParamID()},
                                                        webPedalInputCompressorEnabledRelay{id::PEDAL_INPUT_COMPRESSOR_ENABLED.getParamID()},
                                                        webPedalInputCompressorThresholdRelay{id::PEDAL_INPUT_COMPRESSOR_THRESHOLD.getParamID()},
                                                        webPedalInputCompressorAttackRelay{id::PEDAL_INPUT_COMPRESSOR_ATTACK.getParamID()},
                                                        webPedalInputCompressorReleaseRelay{id::PEDAL_INPUT_COMPRESSOR_RELEASE.getParamID()},
                                                        webPedalInputCompressorMixRelay{id::PEDAL_INPUT_COMPRESSOR_MIX.getParamID()},
                                                        webPedalInputCompressorMakeupGainRelay{id::PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN.getParamID()},
                                                        webAmpEnabledRelay{id::AMP_ENABLED.getParamID()},
                                                        webAmpGainRelay{id::AMP_GAIN.getParamID()},
                                                        webAmpBassRelay{id::AMP_BASS.getParamID()},
                                                        webAmpMiddleRelay{id::AMP_MIDDLE.getParamID()},
                                                        webAmpTrebleRelay{id::AMP_TREBLE.getParamID()},
                                                        webAmpPresenceRelay{id::AMP_PRESENCE.getParamID()},
                                                        webAmpOutputGainRelay{id::AMP_OUTPUT_GAIN.getParamID()},
                                                        webAmpPostEqEnabledRelay{id::AMP_POST_EQ_ENABLED.getParamID()},
                                                        webAmpPostEqGainBell1Relay{id::AMP_POST_EQ_GAIN_BELL1.getParamID()},
                                                        webAmpPostEqGainBell2Relay{id::AMP_POST_EQ_GAIN_BELL2.getParamID()},
                                                        webAmpPostEqGainBell3Relay{id::AMP_POST_EQ_GAIN_BELL3.getParamID()},
                                                        webAmpPostEqGainBell4Relay{id::AMP_POST_EQ_GAIN_BELL4.getParamID()},
                                                        webAmpPostEqGainBell5Relay{id::AMP_POST_EQ_GAIN_BELL5.getParamID()},
                                                        webAmpPostEqGainBell6Relay{id::AMP_POST_EQ_GAIN_BELL6.getParamID()},
                                                        webAmpPostEqGainBell7Relay{id::AMP_POST_EQ_GAIN_BELL7.getParamID()},
                                                        webAmpPostEqGainBell8Relay{id::AMP_POST_EQ_GAIN_BELL8.getParamID()},
                                                        webAmpPostEqGainBell9Relay{id::AMP_POST_EQ_GAIN_BELL9.getParamID()},
                                                        webAmpPostEqGainBell10Relay{id::AMP_POST_EQ_GAIN_BELL10.getParamID()},
                                                        webPedalOutputDelayEnabledRelay{id::PEDAL_OUTPUT_DELAY_ENABLED.getParamID()},
                                                        webPedalOutputDelayTimeSyncRelay{id::PEDAL_OUTPUT_DELAY_TIME_SYNC.getParamID()},
                                                        webPedalOutputDelayTimeRelay{id::PEDAL_OUTPUT_DELAY_TIME.getParamID()},
                                                        webPedalOutputDelaySyncTimeEnabledRelay{id::PEDAL_OUTPUT_DELAY_SYNC_TIME_ENABLED.getParamID()},
                                                        webPedalOutputDelayFeedbackRelay{id::PEDAL_OUTPUT_DELAY_FEEDBACK.getParamID()},
                                                        webPedalOutputDelayMixRelay{id::PEDAL_OUTPUT_DELAY_MIX.getParamID()},
                                                        webPedalOutputDelayPingPongRelay{id::PEDAL_OUTPUT_DELAY_PING_PONG.getParamID()},
                                                        webPedalOutputReverbEnabledRelay{id::PEDAL_OUTPUT_REVERB_ENABLED.getParamID()},
                                                        webPedalOutputReverbMixRelay{id::PEDAL_OUTPUT_REVERB_MIX.getParamID()}
    {}

    void createAttachments() {
        auto &state = processorRef.getState();

        if (auto *param = state.getParameter(id::BYPASS.getParamID()))
            webBypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webBypassRelay, nullptr);

        if (auto *param = state.getParameter(id::INPUT_GAIN.getParamID()))
            webInputGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webInputGainRelay, nullptr);

        if (auto *param = state.getParameter(id::OUTPUT_GAIN.getParamID()))
            webOutputGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webOutputGainRelay, nullptr);

        if (auto *param = state.getParameter(id::GATE_ENABLED.getParamID()))
            webGateEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webGateEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::GATE_GAIN.getParamID()))
            webGateGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webGateGainRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_EQ_ENABLED.getParamID()))
            webPedalInputEqEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalInputEqEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_EQ_FREQ.getParamID()))
            webPedalInputEqFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputEqFreqRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_EQ_GAIN.getParamID()))
            webPedalInputEqGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputEqGainRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_EQ_Q.getParamID()))
            webPedalInputEqQSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputEqQRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_OVERDRIVE_ENABLED.getParamID()))
            webPedalInputOverdriveEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalInputOverdriveEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_OVERDRIVE_TONE.getParamID()))
            webPedalInputOverdriveToneSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputOverdriveToneRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_OVERDRIVE_DRIVE.getParamID()))
            webPedalInputOverdriveDriveSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputOverdriveDriveRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_OVERDRIVE_LEVEL.getParamID()))
            webPedalInputOverdriveLevelSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputOverdriveLevelRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_FUZZ_ENABLED.getParamID()))
            webPedalInputFuzzEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalInputFuzzEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_FUZZ_TONE.getParamID()))
            webPedalInputFuzzToneSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputFuzzToneRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_FUZZ_LEVEL.getParamID()))
            webPedalInputFuzzLevelSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputFuzzLevelRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_FUZZ_DRIVE.getParamID()))
            webPedalInputFuzzDriveSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputFuzzDriveRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_COMPRESSOR_ENABLED.getParamID()))
            webPedalInputCompressorEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalInputCompressorEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_COMPRESSOR_THRESHOLD.getParamID()))
            webPedalInputCompressorThresholdSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputCompressorThresholdRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_COMPRESSOR_ATTACK.getParamID()))
            webPedalInputCompressorAttackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputCompressorAttackRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_COMPRESSOR_RELEASE.getParamID()))
            webPedalInputCompressorReleaseSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputCompressorReleaseRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_COMPRESSOR_MIX.getParamID()))
            webPedalInputCompressorMixSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputCompressorMixRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN.getParamID()))
            webPedalInputCompressorMakeupGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalInputCompressorMakeupGainRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_ENABLED.getParamID()))
            webAmpEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webAmpEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_GAIN.getParamID()))
            webAmpGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpGainRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_BASS.getParamID()))
            webAmpBassSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpBassRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_MIDDLE.getParamID()))
            webAmpMiddleSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpMiddleRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_TREBLE.getParamID()))
            webAmpTrebleSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpTrebleRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_PRESENCE.getParamID()))
            webAmpPresenceSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPresenceRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_OUTPUT_GAIN.getParamID()))
            webAmpOutputGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpOutputGainRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_ENABLED.getParamID()))
            webAmpPostEqEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webAmpPostEqEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL1.getParamID()))
            webAmpPostEqGainBell1SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell1Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL2.getParamID()))
            webAmpPostEqGainBell2SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell2Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL3.getParamID()))
            webAmpPostEqGainBell3SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell3Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL4.getParamID()))
            webAmpPostEqGainBell4SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell4Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL5.getParamID()))
            webAmpPostEqGainBell5SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell5Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL6.getParamID()))
            webAmpPostEqGainBell6SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell6Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL7.getParamID()))
            webAmpPostEqGainBell7SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell7Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL8.getParamID()))
            webAmpPostEqGainBell8SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell8Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL9.getParamID()))
            webAmpPostEqGainBell9SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell9Relay, nullptr);

        if (auto *param = state.getParameter(id::AMP_POST_EQ_GAIN_BELL10.getParamID()))
            webAmpPostEqGainBell10SliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webAmpPostEqGainBell10Relay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_ENABLED.getParamID()))
            webPedalOutputDelayEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalOutputDelayEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_TIME_SYNC.getParamID()))
            webPedalOutputDelayTimeSyncSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalOutputDelayTimeSyncRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_TIME.getParamID()))
            webPedalOutputDelayTimeSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalOutputDelayTimeRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_SYNC_TIME_ENABLED.getParamID()))
            webPedalOutputDelaySyncTimeEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalOutputDelaySyncTimeEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_FEEDBACK.getParamID()))
            webPedalOutputDelayFeedbackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalOutputDelayFeedbackRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_MIX.getParamID()))
            webPedalOutputDelayMixSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalOutputDelayMixRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_DELAY_PING_PONG.getParamID()))
            webPedalOutputDelayPingPongSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalOutputDelayPingPongRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_REVERB_ENABLED.getParamID()))
            webPedalOutputReverbEnabledToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webPedalOutputReverbEnabledRelay, nullptr);

        if (auto *param = state.getParameter(id::PEDAL_OUTPUT_REVERB_MIX.getParamID()))
            webPedalOutputReverbMixSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webPedalOutputReverbMixRelay, nullptr);

    }

    juce::WebBrowserComponent::Options addSlidersOptions(juce::WebBrowserComponent::Options options) {
        return options

                // Global Parameters
                .withOptionsFrom(webBypassRelay)
                .withOptionsFrom(webInputGainRelay)
                .withOptionsFrom(webOutputGainRelay)

                // Gate
                .withOptionsFrom(webGateEnabledRelay)
                .withOptionsFrom(webGateGainRelay)

                // Input EQ Pedal
                .withOptionsFrom(webPedalInputEqEnabledRelay)
                .withOptionsFrom(webPedalInputEqFreqRelay)
                .withOptionsFrom(webPedalInputEqGainRelay)
                .withOptionsFrom(webPedalInputEqQRelay)

                // Overdrive Pedal
                .withOptionsFrom(webPedalInputOverdriveEnabledRelay)
                .withOptionsFrom(webPedalInputOverdriveToneRelay)
                .withOptionsFrom(webPedalInputOverdriveDriveRelay)
                .withOptionsFrom(webPedalInputOverdriveLevelRelay)

                // Fuzz Pedal
                .withOptionsFrom(webPedalInputFuzzEnabledRelay)
                .withOptionsFrom(webPedalInputFuzzToneRelay)
                .withOptionsFrom(webPedalInputFuzzLevelRelay)
                .withOptionsFrom(webPedalInputFuzzDriveRelay)

                // Compressor Pedal
                .withOptionsFrom(webPedalInputCompressorEnabledRelay)
                .withOptionsFrom(webPedalInputCompressorThresholdRelay)
                .withOptionsFrom(webPedalInputCompressorAttackRelay)
                .withOptionsFrom(webPedalInputCompressorReleaseRelay)
                .withOptionsFrom(webPedalInputCompressorMixRelay)
                .withOptionsFrom(webPedalInputCompressorMakeupGainRelay)

                // Amp
                .withOptionsFrom(webAmpEnabledRelay)
                .withOptionsFrom(webAmpGainRelay)
                .withOptionsFrom(webAmpBassRelay)
                .withOptionsFrom(webAmpMiddleRelay)
                .withOptionsFrom(webAmpTrebleRelay)
                .withOptionsFrom(webAmpPresenceRelay)
                .withOptionsFrom(webAmpOutputGainRelay)

                // Amp Post EQ
                .withOptionsFrom(webAmpPostEqEnabledRelay)
                .withOptionsFrom(webAmpPostEqGainBell1Relay)
                .withOptionsFrom(webAmpPostEqGainBell2Relay)
                .withOptionsFrom(webAmpPostEqGainBell3Relay)
                .withOptionsFrom(webAmpPostEqGainBell4Relay)
                .withOptionsFrom(webAmpPostEqGainBell5Relay)
                .withOptionsFrom(webAmpPostEqGainBell6Relay)
                .withOptionsFrom(webAmpPostEqGainBell7Relay)
                .withOptionsFrom(webAmpPostEqGainBell8Relay)
                .withOptionsFrom(webAmpPostEqGainBell9Relay)
                .withOptionsFrom(webAmpPostEqGainBell10Relay)

                // Output Delay Pedal
                .withOptionsFrom(webPedalOutputDelayEnabledRelay)
                .withOptionsFrom(webPedalOutputDelayTimeSyncRelay)
                .withOptionsFrom(webPedalOutputDelayTimeRelay)
                .withOptionsFrom(webPedalOutputDelaySyncTimeEnabledRelay)
                .withOptionsFrom(webPedalOutputDelayFeedbackRelay)
                .withOptionsFrom(webPedalOutputDelayMixRelay)
                .withOptionsFrom(webPedalOutputDelayPingPongRelay)

                // Output Reverb Pedal
                .withOptionsFrom(webPedalOutputReverbEnabledRelay)
                .withOptionsFrom(webPedalOutputReverbMixRelay)
               ;
    }

private:
    SkeletonAudioProcessor &processorRef;


    // ==========================================================================
    // Global Parameters
    // ==========================================================================

    juce::WebToggleButtonRelay webBypassRelay;
    juce::WebSliderRelay webInputGainRelay;
    juce::WebSliderRelay webOutputGainRelay;

    // ==========================================================================
    // Gate
    // ==========================================================================

    juce::WebToggleButtonRelay webGateEnabledRelay;
    juce::WebSliderRelay webGateGainRelay;

    // ==========================================================================
    // Input EQ Pedal
    // ==========================================================================

    juce::WebToggleButtonRelay webPedalInputEqEnabledRelay;
    juce::WebSliderRelay webPedalInputEqFreqRelay;
    juce::WebSliderRelay webPedalInputEqGainRelay;
    juce::WebSliderRelay webPedalInputEqQRelay;

    // ==========================================================================
    // Overdrive Pedal
    // ==========================================================================

    juce::WebToggleButtonRelay webPedalInputOverdriveEnabledRelay;
    juce::WebSliderRelay webPedalInputOverdriveToneRelay;
    juce::WebSliderRelay webPedalInputOverdriveDriveRelay;
    juce::WebSliderRelay webPedalInputOverdriveLevelRelay;

    // ==========================================================================
    // Fuzz Pedal
    // ==========================================================================

    juce::WebToggleButtonRelay webPedalInputFuzzEnabledRelay;
    juce::WebSliderRelay webPedalInputFuzzToneRelay;
    juce::WebSliderRelay webPedalInputFuzzLevelRelay;
    juce::WebSliderRelay webPedalInputFuzzDriveRelay;

    // ==========================================================================
    // Compressor Pedal
    // ==========================================================================

    juce::WebToggleButtonRelay webPedalInputCompressorEnabledRelay;
    juce::WebSliderRelay webPedalInputCompressorThresholdRelay;
    juce::WebSliderRelay webPedalInputCompressorAttackRelay;
    juce::WebSliderRelay webPedalInputCompressorReleaseRelay;
    juce::WebSliderRelay webPedalInputCompressorMixRelay;
    juce::WebSliderRelay webPedalInputCompressorMakeupGainRelay;

    // ==========================================================================
    // Amp
    // ==========================================================================

    juce::WebToggleButtonRelay webAmpEnabledRelay;
    juce::WebSliderRelay webAmpGainRelay;
    juce::WebSliderRelay webAmpBassRelay;
    juce::WebSliderRelay webAmpMiddleRelay;
    juce::WebSliderRelay webAmpTrebleRelay;
    juce::WebSliderRelay webAmpPresenceRelay;
    juce::WebSliderRelay webAmpOutputGainRelay;

    // ==========================================================================
    // Amp Post EQ
    // ==========================================================================

    juce::WebToggleButtonRelay webAmpPostEqEnabledRelay;
    juce::WebSliderRelay webAmpPostEqGainBell1Relay;
    juce::WebSliderRelay webAmpPostEqGainBell2Relay;
    juce::WebSliderRelay webAmpPostEqGainBell3Relay;
    juce::WebSliderRelay webAmpPostEqGainBell4Relay;
    juce::WebSliderRelay webAmpPostEqGainBell5Relay;
    juce::WebSliderRelay webAmpPostEqGainBell6Relay;
    juce::WebSliderRelay webAmpPostEqGainBell7Relay;
    juce::WebSliderRelay webAmpPostEqGainBell8Relay;
    juce::WebSliderRelay webAmpPostEqGainBell9Relay;
    juce::WebSliderRelay webAmpPostEqGainBell10Relay;

    // ==========================================================================
    // Output Delay Pedal
    // ==========================================================================

    juce::WebToggleButtonRelay webPedalOutputDelayEnabledRelay;
    juce::WebSliderRelay webPedalOutputDelayTimeSyncRelay;
    juce::WebSliderRelay webPedalOutputDelayTimeRelay;
    juce::WebToggleButtonRelay webPedalOutputDelaySyncTimeEnabledRelay;
    juce::WebSliderRelay webPedalOutputDelayFeedbackRelay;
    juce::WebSliderRelay webPedalOutputDelayMixRelay;
    juce::WebSliderRelay webPedalOutputDelayPingPongRelay;

    // ==========================================================================
    // Output Reverb Pedal
    // ==========================================================================

    juce::WebToggleButtonRelay webPedalOutputReverbEnabledRelay;
    juce::WebSliderRelay webPedalOutputReverbMixRelay;


    // ==========================================================================
    // Global Parameters
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webBypassToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webInputGainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webOutputGainSliderAttachment;

    // ==========================================================================
    // Gate
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webGateEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webGateGainSliderAttachment;

    // ==========================================================================
    // Input EQ Pedal
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalInputEqEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputEqFreqSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputEqGainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputEqQSliderAttachment;

    // ==========================================================================
    // Overdrive Pedal
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalInputOverdriveEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputOverdriveToneSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputOverdriveDriveSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputOverdriveLevelSliderAttachment;

    // ==========================================================================
    // Fuzz Pedal
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalInputFuzzEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputFuzzToneSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputFuzzLevelSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputFuzzDriveSliderAttachment;

    // ==========================================================================
    // Compressor Pedal
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalInputCompressorEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputCompressorThresholdSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputCompressorAttackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputCompressorReleaseSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputCompressorMixSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalInputCompressorMakeupGainSliderAttachment;

    // ==========================================================================
    // Amp
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webAmpEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpGainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpBassSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpMiddleSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpTrebleSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPresenceSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpOutputGainSliderAttachment;

    // ==========================================================================
    // Amp Post EQ
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webAmpPostEqEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell1SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell2SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell3SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell4SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell5SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell6SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell7SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell8SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell9SliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webAmpPostEqGainBell10SliderAttachment;

    // ==========================================================================
    // Output Delay Pedal
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalOutputDelayEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalOutputDelayTimeSyncSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalOutputDelayTimeSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalOutputDelaySyncTimeEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalOutputDelayFeedbackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalOutputDelayMixSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalOutputDelayPingPongSliderAttachment;

    // ==========================================================================
    // Output Reverb Pedal
    // ==========================================================================

    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webPedalOutputReverbEnabledToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webPedalOutputReverbMixSliderAttachment;
};