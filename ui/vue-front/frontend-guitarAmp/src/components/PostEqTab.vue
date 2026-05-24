<script setup lang="ts">
import { computed, ref } from 'vue'
import { useJuce } from '@/composables/useJuce'
import EffectCard from './EffectCard.vue'

const { getToggle } = useJuce()
const bypass = computed(() => getToggle('POST_EQ_ON'))

const bands = ref([
  { id: 1, label: '60Hz', gain: 0.5 },
  { id: 2, label: '125Hz', gain: 0.5 },
  { id: 3, label: '250Hz', gain: 0.5 },
  { id: 4, label: '500Hz', gain: 0.5 },
  { id: 5, label: '1kHz', gain: 0.5 },
  { id: 6, label: '2kHz', gain: 0.5 },
  { id: 7, label: '4kHz', gain: 0.5 },
  { id: 8, label: '8kHz', gain: 0.5 },
  { id: 9, label: '12kHz', gain: 0.5 },
  { id: 10, label: '16kHz', gain: 0.5 },
])

const bandAverage = computed(() => {
  const total = bands.value.reduce((sum, band) => sum + band.gain, 0)
  return total / bands.value.length
})

const inputLevel = computed(() => Math.min(1, Math.max(0.12, 0.35 + (bandAverage.value - 0.5) * 0.9)))
const outputLevel = computed(() => {
  const effectFactor = bypass.value.isActive() ? 0.28 : 1
  return Math.min(1, Math.max(0.08, inputLevel.value * effectFactor + (bandAverage.value - 0.5) * 0.05))
})

const formatGain = (value: number) => {
  const dB = (value - 0.5) * 24
  return `${dB >= 0 ? '+' : ''}${dB.toFixed(1)} dB`
}
</script>

<template>
  <section class="post-eq-tab">
    <div class="section-title">Post EQ</div>
    <EffectCard title="Post EQ" :bypass="bypass">
      <div class="post-eq-board">
        <div class="meter-panel meter-input">
          <span class="meter-title">Input</span>
          <div class="meter-gauge">
            <div class="meter-fill" :style="{ height: `${inputLevel * 100}%` }"></div>
          </div>
          <span class="meter-value">{{ Math.round(inputLevel * 100) }}%</span>
        </div>

        <div class="bands-shell">
          <div class="bands-headline">
            <span class="bands-label">10-Band Bell EQ</span>
            <span class="bands-subtitle">Analog-style bell banks with signal meters</span>
          </div>

          <div class="bands-grid">
            <div v-for="band in bands" :key="band.id" class="band-column">
              <div class="band-label">{{ band.label }}</div>
              <div class="vertical-slider">
                <input
                  type="range"
                  min="0"
                  max="1"
                  step="0.005"
                  v-model.number="band.gain"
                />
              </div>
              <div class="band-value">{{ formatGain(band.gain) }}</div>
            </div>
          </div>
        </div>

        <div class="meter-panel meter-output">
          <span class="meter-title">Output</span>
          <div class="meter-gauge">
            <div class="meter-fill" :style="{ height: `${outputLevel * 100}%` }"></div>
          </div>
          <span class="meter-value">{{ Math.round(outputLevel * 100) }}%</span>
        </div>
      </div>
    </EffectCard>
  </section>
</template>

<style scoped>
.post-eq-tab {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.section-title {
  font-size: 13px;
  font-weight: 700;
  color: #d7e3ff;
  text-transform: uppercase;
  letter-spacing: 0.2em;
}

.post-eq-board {
  display: grid;
  grid-template-columns: minmax(90px, 110px) 1fr minmax(90px, 110px);
  gap: 18px;
  align-items: stretch;
  padding: 10px;
}

.meter-panel {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 14px;
  padding: 16px 10px;
  border-radius: 18px;
  background: radial-gradient(circle at top, rgba(255,255,255,0.06), rgba(255,255,255,0.01));
  border: 1px solid rgba(255, 255, 255, 0.08);
  box-shadow: inset 0 1px 0 rgba(255,255,255,0.08), inset 0 -1px 0 rgba(0,0,0,0.2);
}

.meter-title {
  font-size: 11px;
  font-weight: 700;
  letter-spacing: 0.2em;
  color: #c7d7ff;
  text-transform: uppercase;
}

.meter-gauge {
  position: relative;
  width: 28px;
  height: 240px;
  border-radius: 18px;
  background: linear-gradient(180deg, #1f242c 0%, #12151a 100%);
  border: 1px solid rgba(255,255,255,0.08);
  box-shadow: inset 0 0 10px rgba(0,0,0,0.35);
  overflow: hidden;
}

.meter-gauge::before {
  content: '';
  position: absolute;
  inset: 12px;
  border-radius: 12px;
  background: repeating-linear-gradient(
    to top,
    rgba(255,255,255,0.06),
    rgba(255,255,255,0.06) 2px,
    transparent 2px,
    transparent 8px
  );
}

.meter-fill {
  position: absolute;
  bottom: 0;
  left: 0;
  width: 100%;
  border-radius: 18px 18px 0 0;
  background: linear-gradient(180deg, #7bff9c 0%, #16c5ff 100%);
  box-shadow: 0 0 18px rgba(22, 197, 255, 0.55);
  transition: height 0.12s ease;
}

.meter-value {
  font-size: 12px;
  font-weight: 700;
  letter-spacing: 0.1em;
  color: #e9f5ff;
}

.bands-shell {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.bands-headline {
  display: flex;
  justify-content: space-between;
  align-items: flex-end;
  gap: 12px;
  padding: 0 14px;
}

.bands-label {
  font-size: 12px;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 0.18em;
  color: #d5e1ff;
}

.bands-subtitle {
  font-size: 10px;
  color: #9aa8c8;
}

.bands-grid {
  display: grid;
  grid-template-columns: repeat(10, minmax(0, 1fr));
  gap: 10px;
  padding: 8px 10px 10px;
  background: linear-gradient(180deg, rgba(255,255,255,0.03), rgba(255,255,255,0.01));
  border: 1px solid rgba(255,255,255,0.08);
  border-radius: 18px;
  box-shadow: inset 0 1px 0 rgba(255,255,255,0.04);
}

.band-column {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
  padding: 10px 4px 8px;
  background: rgba(22, 28, 36, 0.86);
  border: 1px solid rgba(255,255,255,0.06);
  border-radius: 16px;
}

.band-label {
  font-size: 10px;
  color: #b8caea;
  text-transform: uppercase;
  letter-spacing: 0.14em;
}

.vertical-slider {
  position: relative;
  width: 100%;
  height: 180px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.vertical-slider input[type='range'] {
  width: 180px;
  height: 18px;
  -webkit-appearance: none;
  appearance: none;
  transform: rotate(-90deg);
  transform-origin: center;
  cursor: pointer;
  background: transparent;
}

.vertical-slider input[type='range']::-webkit-slider-runnable-track {
  width: 100%;
  height: 8px;
  border-radius: 999px;
  background: linear-gradient(90deg, rgba(255,255,255,0.08), rgba(255,255,255,0.16));
  border: 1px solid rgba(255,255,255,0.12);
  box-shadow: inset 0 0 6px rgba(0,0,0,0.35);
}

.vertical-slider input[type='range']::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: linear-gradient(135deg, #cfdfff, #8aa7ff);
  border: 1px solid rgba(255,255,255,0.35);
  box-shadow: 0 2px 10px rgba(0,0,0,0.35);
  margin-top: -5px;
}

.vertical-slider input[type='range']::-moz-range-track {
  width: 100%;
  height: 8px;
  border-radius: 999px;
  background: linear-gradient(90deg, rgba(255,255,255,0.08), rgba(255,255,255,0.16));
  border: 1px solid rgba(255,255,255,0.12);
  box-shadow: inset 0 0 6px rgba(0,0,0,0.35);
}

.vertical-slider input[type='range']::-moz-range-thumb {
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: linear-gradient(135deg, #cfdfff, #8aa7ff);
  border: 1px solid rgba(255,255,255,0.35);
  box-shadow: 0 2px 10px rgba(0,0,0,0.35);
}

.band-value {
  font-size: 10px;
  font-weight: 700;
  color: #d4e1ff;
}
</style>
