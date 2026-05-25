#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getInputEqMap()
    {
        static const std::unordered_map<juce::String, std::string> map = {
            { id::PEDAL_INPUT_EQ_ENABLED.getParamID(),           "/InputEQ/enabled" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_1.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/31_Hz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_2.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/62_Hz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_3.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/125_Hz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_4.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/250_Hz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_5.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/500_Hz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_6.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/1_kHz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_7.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/2_kHz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_8.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/4_kHz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_9.getParamID(),      "/EqPedalEngine/10-Band_Proportional_Q_EQ/8_kHz" },
            { id::PEDAL_INPUT_EQ_GAIN_BAND_10.getParamID(),     "/EqPedalEngine/10-Band_Proportional_Q_EQ/16_kHz" }
        };
        return map;
    }


    inline std::string getInputEqPath(const juce::String& idStr)       { auto& m = getInputEqMap();    auto it=m.find(idStr); return it!=m.end()?it->second:""; }

}