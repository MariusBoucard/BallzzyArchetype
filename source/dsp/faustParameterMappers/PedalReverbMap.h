
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getReverbMap() {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_OUTPUT_REVERB_ENABLED.getParamID(), "/Reverb/enabled" },
            { id::PEDAL_OUTPUT_REVERB_MIX.getParamID(),     "/Reverb/mix" },
            { id::PEDAL_OUTPUT_REVERB_DECAY.getParamID(),   "/Reverb/decay" },
            { id::PEDAL_OUTPUT_REVERB_LF_FREQ.getParamID(), "/Reverb/lfFreq" },
            { id::PEDAL_OUTPUT_REVERB_HP_FREQ.getParamID(), "/Reverb/hpFreq" }
        };
        return map;
    }

    inline std::string getReverbPath(const juce::String& idStr)        { auto& m = getReverbMap();     auto it=m.find(idStr); return it!=m.end()?it->second:""; }
}