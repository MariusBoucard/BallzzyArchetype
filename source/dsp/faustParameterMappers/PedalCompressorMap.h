#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getPedalCompressorMap() {
        // TODO Faust link19:
    // TODO ADD RATIO    21: /Standard_Library_Compressor/Ratio

        static const std::unordered_map<juce::String, std::string> map = {
            {id::PEDAL_INPUT_COMPRESSOR_ATTACK.getParamID() , "/Standard_Library_Compressor/Attack"},
            {id::PEDAL_INPUT_COMPRESSOR_RELEASE.getParamID() , "/Standard_Library_Compressor/Release"},
            {id::PEDAL_INPUT_COMPRESSOR_THRESHOLD.getParamID() , "/Standard_Library_Compressor/Threshold"},
            {id::PEDAL_INPUT_COMPRESSOR_MAKEUP_GAIN.getParamID() , "/Standard_Library_Compressor/Makeup_Gain"},
            {id::PEDAL_INPUT_COMPRESSOR_RATIO.getParamID() , "/Standard_Library_Compressor/Ratio"},
        };
        return map;
    }

    inline std::string getPedalCompressorPath(const juce::String& parameterID) {
        const auto& map = getPedalCompressorMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}
