<script setup lang="ts">
import { computed } from 'vue'
import { useJuce } from '@/composables/useJuce'
import RotaryKnob from './RotaryKnob.vue'
import EffectCard from './EffectCard.vue'

const { getSlider, getToggle } = useJuce()
const bypass = computed(() => getToggle('COMPRESSOR_ON'))
const threshold = computed(() => getSlider('COMPRESSOR_THRESHOLD'))
const ratio = computed(() => getSlider('COMPRESSOR_RATIO'))
const attack = computed(() => getSlider('COMPRESSOR_ATTACK'))
const release = computed(() => getSlider('COMPRESSOR_RELEASE'))
const makeup = computed(() => getSlider('COMPRESSOR_MAKEUP'))
</script>

<template>
  <EffectCard title="Compressor" :bypass="bypass">
    <div class="effect-knob-row">
      <RotaryKnob
        :model-value="threshold.state.normalised"
        @update:model-value="threshold.setNormalisedValue"
        label="Threshold"
        size="small"
        :min="-60"
        :max="0"
        unit="dB"
      />
      <RotaryKnob
        :model-value="ratio.state.normalised"
        @update:model-value="ratio.setNormalisedValue"
        label="Ratio"
        size="small"
        :min="1"
        :max="20"
      />
      <RotaryKnob
        :model-value="attack.state.normalised"
        @update:model-value="attack.setNormalisedValue"
        label="Attack"
        size="small"
        :min="0"
        :max="100"
        unit="ms"
      />
      <RotaryKnob
        :model-value="release.state.normalised"
        @update:model-value="release.setNormalisedValue"
        label="Release"
        size="small"
        :min="10"
        :max="500"
        unit="ms"
      />
      <RotaryKnob
        :modelValue="makeup.state.normalised"
        @update:modelValue="makeup.setNormalisedValue"
        label="Makeup"
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
  grid-template-columns: repeat(5, minmax(0, 1fr));
  gap: 12px;
}
</style>
