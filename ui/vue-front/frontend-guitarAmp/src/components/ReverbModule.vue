<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import EffectCard from './EffectCard.vue'

const { getSlider, getToggle } = useJuce()
const bypass = getToggle('REVERB_ON')
const decay = getSlider('REVERB_SIZE')
const mix = getSlider('REVERB_MIX')
const lowpass = getSlider('REVERB_LOWPASS')
const highpass = getSlider('REVERB_HIGHPASS')

const decaySeconds = computed(() => (decay.state.normalised * 7.5 + 0.5).toFixed(1))
const mixPct = computed(() => Math.round(mix.state.normalised * 100))
const lowpassHz = computed(() => Math.round(lowpass.state.normalised * 12000 + 200))
const highpassHz = computed(() => Math.round(highpass.state.normalised * 1200))
</script>

<template>
  <EffectCard title="Reverb" :bypass="bypass">
    <div class="reverb-pedal">
      <div class="reverb-top">
        <div>
          <span class="pedal-label">Reverb</span>
          <div class="pedal-tag">Plate/Space</div>
        </div>
        <div class="reverb-meter">
          <span>DECAY</span>
          <strong>{{ decaySeconds }}s</strong>
        </div>
      </div>

      <div class="reverb-screen">
        <div class="screen-item">
          <span>Mix</span>
          <strong>{{ mixPct }}%</strong>
        </div>
        <div class="screen-item">
          <span>LPF</span>
          <strong>{{ lowpassHz }}Hz</strong>
        </div>
        <div class="screen-item">
          <span>HPF</span>
          <strong>{{ highpassHz }}Hz</strong>
        </div>
      </div>

      <div class="reverb-knobs">
        <div class="knob-stack">
          <span class="control-label">Decay</span>
          <input
            type="range"
            min="0"
            max="1"
            step="0.001"
            :value="decay.state.normalised"
            @input="decay.setNormalisedValue(Number($event.target.value))"
          />
          <span class="control-value">{{ decaySeconds }}s</span>
        </div>

        <div class="knob-stack">
          <span class="control-label">Mix</span>
          <input
            type="range"
            min="0"
            max="1"
            step="0.001"
            :value="mix.state.normalised"
            @input="mix.setNormalisedValue(Number($event.target.value))"
          />
          <span class="control-value">{{ mixPct }}%</span>
        </div>

        <div class="knob-stack">
          <span class="control-label">Low Pass</span>
          <input
            type="range"
            min="0"
            max="1"
            step="0.001"
            :value="lowpass.state.normalised"
            @input="lowpass.setNormalisedValue(Number($event.target.value))"
          />
          <span class="control-value">{{ lowpassHz }}Hz</span>
        </div>

        <div class="knob-stack">
          <span class="control-label">High Pass</span>
          <input
            type="range"
            min="0"
            max="1"
            step="0.001"
            :value="highpass.state.normalised"
            @input="highpass.setNormalisedValue(Number($event.target.value))"
          />
          <span class="control-value">{{ highpassHz }}Hz</span>
        </div>
      </div>
    </div>
  </EffectCard>
</template>

<style scoped>
.reverb-pedal {
  display: grid;
  gap: 16px;
  padding: 14px;
  background: radial-gradient(circle at top left, rgba(190, 132, 255, 0.14), transparent 42%),
              linear-gradient(180deg, #161b24 0%, #10131a 100%);
  border-radius: 18px;
  border: 1px solid rgba(255,255,255,0.08);
}

.reverb-top {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 12px;
}

.pedal-label {
  font-size: 12px;
  font-weight: 700;
  color: #e4d8ff;
  text-transform: uppercase;
  letter-spacing: 0.16em;
}

.pedal-tag {
  margin-top: 6px;
  font-size: 10px;
  color: #b5a6ff;
  background: rgba(255,255,255,0.05);
  padding: 6px 10px;
  border-radius: 999px;
}

.reverb-meter {
  display: grid;
  gap: 4px;
  text-align: right;
}

.reverb-meter span {
  font-size: 11px;
  color: #9fa8d4;
  text-transform: uppercase;
  letter-spacing: 0.12em;
}

.reverb-meter strong {
  font-size: 14px;
  color: #f4f3ff;
}

.reverb-screen {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 10px;
  padding: 14px;
  border-radius: 16px;
  background: rgba(255,255,255,0.04);
  border: 1px solid rgba(255,255,255,0.08);
}

.screen-item {
  display: flex;
  flex-direction: column;
  gap: 6px;
  color: #cdd2ff;
}

.screen-item span {
  font-size: 10px;
  text-transform: uppercase;
  letter-spacing: 0.12em;
  opacity: 0.9;
}

.screen-item strong {
  font-size: 13px;
  color: #f8f8ff;
}

.reverb-knobs {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  gap: 12px;
}

.knob-stack {
  display: grid;
  gap: 10px;
  padding: 12px;
  border-radius: 16px;
  background: rgba(255,255,255,0.03);
  border: 1px solid rgba(255,255,255,0.07);
}

.control-label {
  font-size: 10px;
  letter-spacing: 0.12em;
  text-transform: uppercase;
  color: #c8d2ff;
}

.knob-stack input[type='range'] {
  width: 100%;
  height: 8px;
  appearance: none;
  background: linear-gradient(90deg, rgba(190, 132, 255, 0.35), rgba(255,255,255,0.12));
  border-radius: 999px;
  outline: none;
}

.knob-stack input[type='range']::-webkit-slider-thumb {
  appearance: none;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #d3b7ff;
  border: 1px solid rgba(255,255,255,0.7);
  box-shadow: 0 0 12px rgba(211,183,255,0.4);
}

.knob-stack input[type='range']::-moz-range-thumb {
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #d3b7ff;
  border: 1px solid rgba(255,255,255,0.7);
  box-shadow: 0 0 12px rgba(211,183,255,0.4);
}

.control-value {
  font-size: 12px;
  color: #eef0ff;
  font-weight: 700;
}
</style>
