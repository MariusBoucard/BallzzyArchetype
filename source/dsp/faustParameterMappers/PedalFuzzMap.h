#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getFuzzMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_INPUT_FUZZ_ENABLED.getParamID(), "/Fuzz/enabled" },
            { id::PEDAL_INPUT_FUZZ_TONE.getParamID(),    "/Fuzz/tone" },
            { id::PEDAL_INPUT_FUZZ_LEVEL.getParamID(),   "/Fuzz/level" },
            { id::PEDAL_INPUT_FUZZ_DRIVE.getParamID(),   "/Fuzz/drive" }
        };
        return map;
    }
    inline std::string getFuzzPath(const juce::String& idStr)          { auto& m = getFuzzMap();       auto it=m.find(idStr); return it!=m.end()?it->second:""; }

}