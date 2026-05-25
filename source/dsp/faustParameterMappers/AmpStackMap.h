

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getAmpStackMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::AMP_BASS.getParamID(), "/Simple_Proportional-Q_EQ/1._Low"},
            {id::AMP_MIDDLE.getParamID(), "/Simple_Proportional-Q_EQ/2._Mid"},
            {id::AMP_TREBLE.getParamID(),"/Simple_Proportional-Q_EQ/3._Presence" },
            {id::AMP_PRESENCE.getParamID(),"/Simple_Proportional-Q_EQ/4._High" },
        };
        return map;
    }
    inline std::string getAmpStackPath(const juce::String& idStr)        { auto& m = getAmpStackMap();     auto it=m.find(idStr); return it!=m.end()?it->second:""; }



}


