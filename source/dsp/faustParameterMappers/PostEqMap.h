

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getAmpPostEqMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::AMP_POST_EQ_ENABLED.getParamID(),       "/PostEQ/enabled" },
{ id::PEDAL_INPUT_EQ_GAIN_BAND_1.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/31_Hz" },
         { id::AMP_POST_EQ_GAIN_BELL2.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/62_Hz" },
         { id::AMP_POST_EQ_GAIN_BELL3.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/125_Hz" },
         { id::AMP_POST_EQ_GAIN_BELL4.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/250_Hz" },
         { id::AMP_POST_EQ_GAIN_BELL5.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/500_Hz" },
         { id::AMP_POST_EQ_GAIN_BELL6.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/1_kHz" },
         { id::AMP_POST_EQ_GAIN_BELL7.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/2_kHz" },
         { id::AMP_POST_EQ_GAIN_BELL8.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/4_kHz" },
         { id::AMP_POST_EQ_GAIN_BELL9.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/8_kHz" },
         { id::AMP_POST_EQ_GAIN_BELL10.getParamID(),     "/EqPedalEngine/10-Band_Proportional_Q_EQ/16_kHz" }
        };
        return map;
    }
    inline std::string getPostAmpEqPath(const juce::String& idStr)        { auto& m = getAmpPostEqMap();     auto it=m.find(idStr); return it!=m.end()?it->second:""; }



}


