#pragma once

#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getFuzzMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_INPUT_FUZZ_MIX.getParamID(), "/FuzzzzPedalengine/Mix__Dry_Wet_" },
            { id::PEDAL_INPUT_FUZZ_TONE.getParamID(),    "/FuzzzzPedalengine/Tone" },
            { id::PEDAL_INPUT_FUZZ_LEVEL.getParamID(),   "/FuzzzzPedalengine/Level"},
            { id::PEDAL_INPUT_FUZZ_DRIVE.getParamID(),   "/FuzzzzPedalengine/Drive"  }
        };
        return map;
    }
    inline std::string getFuzzPath(const juce::String& idStr)          { auto& m = getFuzzMap();       auto it=m.find(idStr); return it!=m.end()?it->second:""; }

}