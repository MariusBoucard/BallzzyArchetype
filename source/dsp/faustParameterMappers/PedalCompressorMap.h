#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getPedalCompressorMap() {
        // TODO Faust link
        static const std::unordered_map<juce::String, std::string> map = {
            {id::PEDAL_INPUT_COMPRESSOR_ATTACK.getParamID() , "/HpLp/Filters_BypassHp"},
            {id::PEDAL_INPUT_COMPRESSOR_RELEASE.getParamID() , "/HpLp/Filters_BypassHp"},
            {id::PEDAL_INPUT_COMPRESSOR_THRESHOLD.getParamID() , "/HpLp/Filters_BypassHp"},
            {id::PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN.getParamID() , "/HpLp/Filters_BypassHp"},
            {id::PEDAL_INPUT_COMPRESSOR_ENABLED.getParamID() , "/HpLp/Filters_BypassHp"},
        };
        return map;
    }

    inline std::string getPedalCompressorPath(const juce::String& parameterID) {
        const auto& map = getPedalCompressorMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}
