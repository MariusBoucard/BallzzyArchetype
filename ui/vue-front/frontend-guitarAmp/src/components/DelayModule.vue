<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import EffectCard from './EffectCard.vue'

const { getSlider, getToggle } = useJuce()
const bypass = getToggle('POST_DELAY_ON')
const time = getSlider('POST_DELAY_TIME')
const feedback = getSlider('POST_DELAY_FEEDBACK')
const mix = getSlider('POST_DELAY_MIX')
const sync = getToggle('POST_DELAY_SYNC')
const pingpong = getToggle('POST_DELAY_PINGPONG')

const timeMs = computed(() => Math.round(time.state.normalised * 1000))
const feedbackPct = computed(() => Math.round(feedback.state.normalised * 100))
const mixPct = computed(() => Math.round(mix.state.normalised * 100))
</script>

<template>
  <EffectCard title="Delay" :bypass="bypass">
    <div class="delay-pedal">
      <div class="delay-header">
        <span class="pedal-label">GTR Delay</span>
        <div class="pedal-status">{{ bypass.isActive() ? 'ACTIVE' : 'BYPASSED' }}</div>
      </div>

      <div class="delay-screen">
        <div class="screen-row">
          <span>TIME</span>
          <strong>{{ timeMs }} ms</strong>
        </div>
        <div class="screen-row">
          <span>FEED</span>
          <strong>{{ feedbackPct }}%</strong>
        </div>
        <div class="screen-row">
          <span>MIX</span>
          <strong>{{ mixPct }}%</strong>
        </div>
        <div class="screen-row small">
          <span>SYNC</span>
          <strong>{{ sync.isActive() ? 'ON' : 'OFF' }}</strong>
        </div>
        <div class="screen-row small">
          <span>PING</span>
          <strong>{{ pingpong.isActive() ? 'PONG' : 'OFF' }}</strong>
        </div>
      </div>

      <div class="delay-controls">
        <div class="control-block">
          <span class="control-label">Time</span>
          <div class="control-knob">
            <input
              type="range"
              min="0"
              max="1"
              step="0.001"
              :value="time.state.normalised"
              @input="time.setNormalisedValue(Number($event.target.value))"
            />
          </div>
          <span class="control-value">{{ timeMs }}ms</span>
        </div>

        <div class="control-block">
          <span class="control-label">Feedback</span>
          <div class="control-knob">
            <input
              type="range"
              min="0"
              max="1"
              step="0.001"
              :value="feedback.state.normalised"
              @input="feedback.setNormalisedValue(Number($event.target.value))"
            />
          </div>
          <span class="control-value">{{ feedbackPct }}%</span>
        </div>

        <div class="control-block">
          <span class="control-label">Mix</span>
          <div class="control-knob">
            <input
              type="range"
              min="0"
              max="1"
              step="0.001"
              :value="mix.state.normalised"
              @input="mix.setNormalisedValue(Number($event.target.value))"
            />
          </div>
          <span class="control-value">{{ mixPct }}%</span>
        </div>
      </div>

      <div class="delay-footer">
        <button
          type="button"
          class="toggle-button"
          :class="{ active: sync.isActive() }"
          @click="sync.toggle(!sync.isActive())"
        >
          Sync
        </button>
        <button
          type="button"
          class="toggle-button"
          :class="{ active: pingpong.isActive() }"
          @click="pingpong.toggle(!pingpong.isActive())"
        >
          Ping Pong
        </button>
      </div>
    </div>
  </EffectCard>
</template>

<style scoped>
.delay-pedal {
  display: grid;
  gap: 16px;
  padding: 12px;
  background: radial-gradient(circle at top left, rgba(88, 142, 240, 0.18), transparent 40%),
              linear-gradient(180deg, #12151c 0%, #171c24 100%);
  border-radius: 18px;
  border: 1px solid rgba(255,255,255,0.08);
}

.delay-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 12px;
}

.pedal-label {
  font-size: 12px;
  font-weight: 700;
  letter-spacing: 0.16em;
  text-transform: uppercase;
  color: #d3e3ff;
}

.pedal-status {
  font-size: 11px;
  color: #98b7ff;
  background: rgba(255,255,255,0.06);
  padding: 6px 10px;
  border-radius: 999px;
}

.delay-screen {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
  padding: 14px;
  border-radius: 16px;
  background: linear-gradient(180deg, rgba(3, 12, 34, 0.85), rgba(10, 20, 40, 0.96));
  border: 1px solid rgba(255,255,255,0.06);
  box-shadow: inset 0 1px 0 rgba(255,255,255,0.08);
}

.screen-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 10px;
  color: #b8d2ff;
  font-size: 12px;
}

.screen-row.small {
  font-size: 11px;
  opacity: 0.9;
}

.screen-row strong {
  color: #f5f9ff;
  font-size: 13px;
}

.delay-controls {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 12px;
}

.control-block {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 12px;
  border-radius: 16px;
  background: rgba(255,255,255,0.03);
  border: 1px solid rgba(255,255,255,0.07);
}

.control-label {
  font-size: 11px;
  color: #cddcff;
  text-transform: uppercase;
  letter-spacing: 0.12em;
}

.control-knob {
  display: flex;
  align-items: center;
  justify-content: center;
}

.control-knob input[type='range'] {
  width: 100%;
  -webkit-appearance: none;
  appearance: none;
  height: 8px;
  border-radius: 999px;
  background: linear-gradient(90deg, rgba(95, 175, 255, 0.38), rgba(255,255,255,0.12));
  outline: none;
}

.control-knob input[type='range']::-webkit-slider-thumb {
  -webkit-appearance: none;
  height: 18px;
  width: 18px;
  border-radius: 50%;
  background: #82b0ff;
  border: 1px solid rgba(255,255,255,0.65);
  box-shadow: 0 0 12px rgba(129, 176, 255, 0.4);
}

.control-knob input[type='range']::-moz-range-thumb {
  height: 18px;
  width: 18px;
  border-radius: 50%;
  background: #82b0ff;
  border: 1px solid rgba(255,255,255,0.65);
  box-shadow: 0 0 12px rgba(129, 176, 255, 0.4);
}

.control-value {
  font-size: 12px;
  font-weight: 700;
  color: #edf4ff;
}

.delay-footer {
  display: flex;
  justify-content: space-between;
  gap: 10px;
}

.toggle-button {
  flex: 1;
  padding: 10px 14px;
  border-radius: 999px;
  border: 1px solid rgba(255,255,255,0.1);
  background: rgba(255,255,255,0.04);
  color: #c4daff;
  font-size: 12px;
  font-weight: 700;
  letter-spacing: 0.1em;
  cursor: pointer;
  transition: all 0.2s ease;
}

.toggle-button.active {
  background: linear-gradient(90deg, #56b7ff, #2a8cff);
  color: #081c2d;
  box-shadow: 0 0 12px rgba(52, 144, 255, 0.35);
}

.toggle-button:hover {
  transform: translateY(-1px);
}
</style>