#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

#include "../ParameterIDs.hpp"

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getPedalDelayMap() {
        // TODO Faust link
        static const std::unordered_map<juce::String, std::string> map = {
            {id::PEDAL_OUTPUT_DELAY_ENABLED.getParamID() , "/HpLp/Filters_BypassHp"},
{id::PEDAL_OUTPUT_DELAY_FEEDBACK.getParamID() , "/HpLp/Filters_BypassHp"},
{id::PEDAL_OUTPUT_DELAY_MIX.getParamID() , "/HpLp/Filters_BypassHp"},
{id::PEDAL_OUTPUT_DELAY_PING_PONG.getParamID() , "/HpLp/Filters_BypassHp"},
{id::PEDAL_OUTPUT_DELAY_TIME.getParamID() , "/HpLp/Filters_BypassHp"},
{id::PEDAL_OUTPUT_DELAY_SYNC_TIME_ENABLED.getParamID() , "/HpLp/Filters_BypassHp"},
{id::PEDAL_OUTPUT_DELAY_TIME_SYNC.getParamID() , "/HpLp/Filters_BypassHp"},
        };
        return map;
    }

    inline std::string getPedalDelayPath(const juce::String& parameterID) {
        const auto& map = getPedalDelayMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}
