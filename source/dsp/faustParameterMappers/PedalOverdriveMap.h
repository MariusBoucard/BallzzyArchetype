#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getOverdriveMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_INPUT_OVERDRIVE_ENABLED.getParamID(), "/Overdrive/enabled" },
            { id::PEDAL_INPUT_OVERDRIVE_TONE.getParamID(),    "/Overdrive/tone" },
            { id::PEDAL_INPUT_OVERDRIVE_DRIVE.getParamID(),   "/Overdrive/drive" },
            { id::PEDAL_INPUT_OVERDRIVE_LEVEL.getParamID(),   "/Overdrive/level" }
        };
        return map;
    }


    inline std::string getOverdrivePath(const juce::String& idStr)     { auto& m = getOverdriveMap();  auto it=m.find(idStr); return it!=m.end()?it->second:""; }

}