<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import RotaryKnob from './RotaryKnob.vue'
import EffectCard from './EffectCard.vue'

const { getSlider, getToggle } = useJuce()
const bypass = computed(() => getToggle('OVERDRIVE_ON'))
const drive = computed(() => getSlider('OVERDRIVE_DRIVE'))
const tone = computed(() => getSlider('OVERDRIVE_TONE'))
const level = computed(() => getSlider('OVERDRIVE_LEVEL'))
</script>

<template>
  <EffectCard title="Overdrive" :bypass="bypass">
    <div class="effect-knob-row">
      <RotaryKnob
        :model-value="drive.state.normalised"
        @update:model-value="drive.setNormalisedValue"
        label="Drive"
        size="small"
        :min="0"
        :max="100"
        unit="%"
      />
      <RotaryKnob
        :model-value="tone.state.normalised"
        @update:model-value="tone.setNormalisedValue"
        label="Tone"
        size="small"
        :min="0"
        :max="100"
        unit="%"
      />
      <RotaryKnob
        :model-value="level.state.normalised"
        @update:model-value="level.setNormalisedValue"
        label="Level"
        size="small"
        :min="-12"
        :max="12"
        unit="dB"
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
