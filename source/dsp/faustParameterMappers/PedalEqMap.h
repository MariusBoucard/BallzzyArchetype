#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getInputEqMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_INPUT_EQ_ENABLED.getParamID(), "/InputEQ/enabled" },
            { id::PEDAL_INPUT_EQ_FREQ.getParamID(),    "/InputEQ/freq" },
            { id::PEDAL_INPUT_EQ_GAIN.getParamID(),    "/InputEQ/gain" },
            { id::PEDAL_INPUT_EQ_Q.getParamID(),       "/InputEQ/q" }
        };
        return map;
    }



    inline std::string getInputEqPath(const juce::String& idStr)       { auto& m = getInputEqMap();    auto it=m.find(idStr); return it!=m.end()?it->second:""; }

}