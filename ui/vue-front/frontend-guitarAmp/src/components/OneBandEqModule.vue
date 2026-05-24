<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import RotaryKnob from './RotaryKnob.vue'
import EffectCard from './EffectCard.vue'

const { getSlider, getToggle } = useJuce()
const bypass = computed(() => getToggle('INPUT_EQ_ON'))
const freq = computed(() => getSlider('INPUT_EQ_FREQ'))
const gain = computed(() => getSlider('INPUT_EQ_GAIN'))
const q = computed(() => getSlider('INPUT_EQ_Q'))
</script>

<template>
  <EffectCard title="Input EQ" :bypass="bypass">
    <div class="effect-knob-row">
      <RotaryKnob
        :model-value="freq.state.normalised"
        @update:model-value="freq.setNormalisedValue"
        label="Freq"
        size="small"
        :min="20"
        :max="20000"
        unit="Hz"
      />
      <RotaryKnob
        :model-value="gain.state.normalised"
        @update:model-value="gain.setNormalisedValue"
        label="Gain"
        size="small"
        :min="-18"
        :max="18"
        unit="dB"
      />
      <RotaryKnob
        :model-value="q.state.normalised"
        @update:model-value="q.setNormalisedValue"
        label="Q"
        size="small"
        :min="0.5"
        :max="10"
      />
    </div>
  </EffectCard>
</template>

<style scoped>
.effect-knob-row {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 12px;
}
</style>
