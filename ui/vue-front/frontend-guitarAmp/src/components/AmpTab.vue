<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import RotaryKnob from './RotaryKnob.vue'

const { getSlider, getToggle } = useJuce()
const inputGain = computed(() => getSlider('INPUT_GAIN'))
const outputGain = computed(() => getSlider('OUTPUT_GAIN'))
const gate = getToggle('GATE_ON')
const ampVoice = computed(() => getSlider('AMP_NAM_FILE'))

const ampVoiceLabels = ['Clean', 'Crunch', 'Classic', 'HiGain']
const ampVoiceIndex = computed(() => {
  const slider = ampVoice.value
  if (!slider?.state) return 0
  return Math.round(slider.state.normalised * (ampVoiceLabels.length - 1))
})

const ampVoiceValue = computed({
  get: () => ampVoice.value?.state?.normalised ?? 0,
  set: (value: number) => ampVoice.value?.setNormalisedValue?.(value),
})

const gateValue = computed({
  get: () => gate.isActive() ? 1 : 0,
  set: (value: number) => gate.toggle(value >= 0.5),
})

const inputLevel = computed(() => 0.35 + inputGain.value.state.normalised * 0.45)
const outputLevel = computed(() => 0.35 + outputGain.value.state.normalised * 0.45)
</script>

<template>
  <section class="amp-tab">
    <div class="amp-frame">
      <div class="amp-row">
        <div class="amp-meter-block">
          <div class="meter-label">Input</div>
          <div class="meter-body">
            <div class="meter-fill" :style="{ height: `${inputLevel * 100}%` }" />
          </div>
          <div class="meter-value">{{ Math.round(inputLevel * 100) }}%</div>
        </div>

        <div class="amp-control-block">
          <span class="control-title">Input Gain</span>
          <RotaryKnob
            :model-value="inputGain.value.state.normalised"
            @update:model-value="inputGain.value.setNormalisedValue"
            label="Input"
            size="medium"
            :use-strip="true"
            :min="-24"
            :max="12"
            unit="dB"
          />
        </div>

        <div class="amp-control-block gate-toggle-block">
          <span class="control-title">Gate Toggle</span>
          <div class="gate-toggle-pill" :class="{ active: gate.isActive() }">
            {{ gate.isActive() ? 'On' : 'Off' }}
          </div>
        </div>

        <div class="amp-control-block">
          <span class="control-title">Gate</span>
          <RotaryKnob
            :model-value="gateValue"
            @update:model-value="gateValue = $event"
            label="Gate"
            size="medium"
            :values="['Off', 'On']"
          />
        </div>

        <div class="amp-title-block">
          <span class="amp-name">Ballzzy Amp</span>
          <span class="amp-subtitle">Guitar Plugin</span>
        </div>

        <div class="amp-control-block">
          <span class="control-title">Output Gain</span>
          <RotaryKnob
            :model-value="outputGain.value.state.normalised"
            @update:model-value="outputGain.value.setNormalisedValue"
            label="Output"
            size="medium"
            :use-strip="true"
            :min="-24"
            :max="12"
            unit="dB"
          />
        </div>

        <div class="amp-meter-block">
          <div class="meter-label">Output</div>
          <div class="meter-body">
            <div class="meter-fill" :style="{ height: `${outputLevel * 100}%` }" />
          </div>
          <div class="meter-value">{{ Math.round(outputLevel * 100) }}%</div>
        </div>
      </div>

      <div class="voice-block">
        <RotaryKnob
          :model-value="ampVoiceValue"
          @update:model-value="ampVoiceValue = $event"
          label="Voice"
          size="medium"
          :use-strip="true"
          :values="ampVoiceLabels"
        />
        <div class="voice-label">{{ ampVoiceLabels[ampVoiceIndex] }}</div>
      </div>
    </div>
  </section>
</template>

<style scoped>
.amp-tab {
  width: 100%;
  display: flex;
  justify-content: center;
}

.amp-frame {
  width: 100%;
  max-width: 1260px;
  padding: 20px;
  border-radius: 26px;
  background: linear-gradient(180deg, rgba(14, 19, 28, 0.95), rgba(20, 24, 34, 0.98));
  border: 1px solid rgba(255, 255, 255, 0.1);
  box-shadow: 0 18px 60px rgba(0, 0, 0, 0.28);
}

.amp-row {
  display: grid;
  grid-template-columns: minmax(90px, 110px) minmax(120px, 150px) minmax(130px, 170px) minmax(130px, 170px) minmax(220px, 1fr) minmax(120px, 150px) minmax(90px, 110px);
  align-items: center;
  gap: 16px;
  width: 100%;
}

.amp-meter-block,
.amp-control-block {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
}

.meter-label,
.control-title {
  font-size: 10px;
  font-weight: 700;
  letter-spacing: 0.18em;
  text-transform: uppercase;
  color: #aab7d1;
}

.meter-body {
  width: 28px;
  height: 180px;
  position: relative;
  border-radius: 24px;
  background: linear-gradient(180deg, #0b1018 0%, #151c27 100%);
  border: 1px solid rgba(255,255,255,0.08);
  overflow: hidden;
  box-shadow: inset 0 2px 10px rgba(0,0,0,0.35);
}

.meter-fill {
  position: absolute;
  bottom: 0;
  left: 0;
  width: 100%;
  border-radius: 999px 999px 0 0;
  background: linear-gradient(180deg, #60ffb6 0%, #64d7ff 55%, #ffcb4f 85%, #ff5d5d 100%);
  transition: height 0.08s ease;
}

.meter-value {
  font-size: 12px;
  font-weight: 700;
  color: #e8f4ff;
}

.gate-toggle-block {
  justify-content: center;
}

.gate-toggle-pill {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  min-width: 92px;
  padding: 10px 14px;
  border-radius: 999px;
  border: 1px solid rgba(255,255,255,0.1);
  background: rgba(255,255,255,0.05);
  color: #c2d5ff;
  font-size: 11px;
  font-weight: 700;
  transition: all 0.2s ease;
}

.gate-toggle-pill.active {
  background: linear-gradient(90deg, #61b5ff, #2b8dff);
  border-color: rgba(35, 120, 255, 0.45);
  color: #081a28;
  box-shadow: 0 0 16px rgba(42, 141, 255, 0.25);
}

.amp-title-block {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 10px;
  padding: 18px 16px;
  border-radius: 24px;
  background: rgba(255,255,255,0.03);
  border: 1px solid rgba(255,255,255,0.08);
  box-shadow: inset 0 1px 0 rgba(255,255,255,0.08);
}

.amp-name {
  font-size: 18px;
  font-weight: 900;
  letter-spacing: 0.28em;
  text-transform: uppercase;
  color: #e7fbff;
}

.amp-subtitle {
  font-size: 11px;
  color: #9bb5d2;
  text-transform: uppercase;
  letter-spacing: 0.24em;
}

.voice-block {
  margin-top: 18px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 10px;
}

.voice-label {
  font-size: 11px;
  font-weight: 700;
  color: #c7e5ff;
  letter-spacing: 0.16em;
  text-transform: uppercase;
}

.amp-control-block {
  min-width: 120px;
}

.amp-control-block > .knob-label {
  margin-top: 0;
}
</style>