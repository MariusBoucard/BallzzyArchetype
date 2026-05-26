#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getPedalDelayMap() {
    // Attention au passage en mode sync comme on map sur le temps TODO !!!
        static const std::unordered_map<juce::String, std::string> map = {
            {id::PEDAL_OUTPUT_DELAY_FEEDBACK.getParamID() , "/Stereo_Delay/Feedback"},
            {id::PEDAL_OUTPUT_DELAY_MIX.getParamID() , "/Stereo_Delay/Mix"},
            {id::PEDAL_OUTPUT_DELAY_PING_PONG.getParamID() , "/Stereo_Delay_Effect/Ping_Pong"},
            {id::PEDAL_OUTPUT_DELAY_TIME.getParamID() , "/Stereo_Delay/Time"},
        };
        return map;
    }

    inline std::string getPedalDelayPath(const juce::String& parameterID) {
        const auto& map = getPedalDelayMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}
