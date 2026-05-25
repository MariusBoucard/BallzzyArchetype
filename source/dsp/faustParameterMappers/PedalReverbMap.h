
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getReverbMap() {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_OUTPUT_REVERB_MIX.getParamID(),     "/Filtered_Room_Reverb/1._Mix" },
            { id::PEDAL_OUTPUT_REVERB_DECAY.getParamID(),   "/Filtered_Room_Reverb/2._Decay" },

            // TODO : hp and lp are broken for now
            //   { id::PEDAL_OUTPUT_REVERB_LF_FREQ.getParamID(), "/Filtered_Room_Reverb/4._Low_Pass" },
         //   { id::PEDAL_OUTPUT_REVERB_HP_FREQ.getParamID(), "/Filtered_Room_Reverb/3._High_Pass" }
        };
        return map;
    }

    inline std::string getReverbPath(const juce::String& idStr)        { auto& m = getReverbMap();     auto it=m.find(idStr); return it!=m.end()?it->second:""; }
}