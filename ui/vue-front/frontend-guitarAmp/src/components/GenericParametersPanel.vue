<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import RotaryKnob from './RotaryKnob.vue'

const { getSlider, getToggle } = useJuce()

const inputGain = computed(() => getSlider('INPUT_GAIN'))
const outputGain = computed(() => getSlider('OUTPUT_GAIN'))
const gateToggle = computed(() => getToggle('GATE_ON'))
</script>

<template>
  <section class="generic-parameters-panel">
    <div class="generic-header">
      <div>
        <div class="generic-label">GLOBAL</div>
        <div class="generic-description">Input gain, output gain and gate</div>
      </div>
      <button class="gate-button" :class="{ active: gateToggle.isActive() }" @click="gateToggle.toggle(!gateToggle.isActive())">
        {{ gateToggle.isActive() ? 'Gate On' : 'Gate Off' }}
      </button>
    </div>

    <div class="generic-controls-grid">
      <RotaryKnob
        :model-value="inputGain.state.normalised"
        @update:model-value="inputGain.setNormalisedValue"
        label="Input"
        size="medium"
        :min="-24"
        :max="12"
        unit="dB"
      />
      <RotaryKnob
        :model-value="outputGain.state.normalised"
        @update:model-value="outputGain.setNormalisedValue"
        label="Output"
        size="medium"
        :min="-24"
        :max="12"
        unit="dB"
      />
      <div class="parameter-summary">
        <span class="summary-title">Gate</span>
        <span class="summary-value">{{ gateToggle.isActive() ? 'Opened' : 'Closed' }}</span>
      </div>
    </div>
  </section>
</template>

<style scoped>
.generic-parameters-panel {
  width: 100%;
  padding: 18px;
  border-radius: 16px;
  background: linear-gradient(180deg, rgba(38, 43, 60, 0.95), rgba(24, 26, 34, 0.95));
  border: 1px solid rgba(255,255,255,0.08);
  box-shadow: inset 0 1px 1px rgba(255,255,255,0.05);
  margin-bottom: 16px;
}

.generic-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 16px;
  margin-bottom: 16px;
}

.generic-label {
  font-size: 11px;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 0.2em;
  color: #72b6ff;
}

.generic-description {
  font-size: 12px;
  color: #b1becf;
}

.gate-button {
  padding: 10px 16px;
  border-radius: 999px;
  border: 1px solid rgba(255,255,255,0.12);
  background: rgba(255,255,255,0.05);
  color: #dce8ff;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.2s ease;
}

.gate-button.active {
  background: linear-gradient(90deg, #4a9eff, #00d4ff);
  border-color: rgba(0,212,255,0.35);
  color: #08121f;
}

.generic-controls-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 18px;
  align-items: center;
}

.parameter-summary {
  display: flex;
  flex-direction: column;
  gap: 8px;
  padding: 14px;
  border-radius: 14px;
  background: rgba(255,255,255,0.03);
  border: 1px solid rgba(255,255,255,0.06);
  min-height: 110px;
  justify-content: center;
}

.summary-title {
  font-size: 10px;
  font-weight: 700;
  color: #9bb4d6;
  text-transform: uppercase;
}

.summary-value {
  font-size: 18px;
  font-weight: 800;
  color: #f7fbff;
}
</style>
