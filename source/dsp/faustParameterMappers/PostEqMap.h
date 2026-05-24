

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
            { id::AMP_POST_EQ_GAIN_BELL1.getParamID(),    "/PostEQ/bell1/gain" },
            { id::AMP_POST_EQ_GAIN_BELL2.getParamID(),    "/PostEQ/bell2/gain" },
            { id::AMP_POST_EQ_GAIN_BELL3.getParamID(),    "/PostEQ/bell3/gain" },
            { id::AMP_POST_EQ_GAIN_BELL4.getParamID(),    "/PostEQ/bell4/gain" },
            { id::AMP_POST_EQ_GAIN_BELL5.getParamID(),    "/PostEQ/bell5/gain" },
            { id::AMP_POST_EQ_GAIN_BELL6.getParamID(),    "/PostEQ/bell6/gain" },
            { id::AMP_POST_EQ_GAIN_BELL7.getParamID(),    "/PostEQ/bell7/gain" },
            { id::AMP_POST_EQ_GAIN_BELL8.getParamID(),    "/PostEQ/bell8/gain" },
            { id::AMP_POST_EQ_GAIN_BELL9.getParamID(),    "/PostEQ/bell9/gain" },
            { id::AMP_POST_EQ_GAIN_BELL10.getParamID(),   "/PostEQ/bell10/gain" }
        };
        return map;
    }
}


}


