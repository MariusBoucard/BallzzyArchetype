<script setup lang="ts">
import { useJuce } from '@/composables/useJuce'
import { computed, ref } from 'vue'
import RotaryKnob from './RotaryKnob.vue'
import DuckingComponent from './DuckingComponent.vue'
import BpmComponent from './BpmComponent.vue'

const { getSlider, getToggle } = useJuce()

// Get global controls
const sync = computed(() => getToggle('SYNC_TEMPO'))
const globalWidth = computed(() => getSlider('WIDTH'))
const globalTime = computed(() => getSlider('GLOBAL_TIME'))
const globalTimeNoSync = computed(() => getSlider('GLOBAL_TIME_NO_SYNC'))
const syncTimeValues = ['1/64', '1/32T', '1/64D', '1/32', '1/16T', '1/32D', '1/16', '1/8T', '1/16D', '1/8', '1/4T', '1/8D', '1/4', '1/2T', '1/4D', '1/2', '1T', '1/2D', '1']

const globalFeedback = computed(() => getSlider('FEEDBACK'))
const globalPeriodDuration = computed(() => getSlider('MOVEMENT_PERIOD_DURATION'))

const globalMix = computed(() => getSlider('MIX'))
const globalLpFilter = computed(() => getSlider('LP_FILTER_FREQ'))
const globalHpFilter = computed(() => getSlider('HP_FILTER_FREQ'))

const selectedMode = ref('normal')

const setTimeValue = (value: string) => {
  const index = syncTimeValues.indexOf(value)
  if (index !== -1) {
    globalTime.value.setNormalisedValue(index / (syncTimeValues.length - 1))
  }
}
// Filter bypass toggles
const lpFilterBypass = computed(() => getToggle('LP_FILTER_BYPASS'))
const hpFilterBypass = computed(() => getToggle('HP_FILTER_BYPASS'))
const selectedTimeValue = computed(() => {
  const index = Math.round(globalTime.value.state.normalised * (syncTimeValues.length - 1))
  return syncTimeValues[Math.max(0, Math.min(syncTimeValues.length - 1, index))]
})
const setMode = (mode: string) => {
  selectedMode.value = mode
}
</script>

<template>
  <div class="bottom-section">
    <!-- Left: SYNC Toggle -->
    <div class="control-group sync-group"      data-tooltip="Sync time timings to tempo">
      <label class="toggle-label"       
>
        <input 
          type="checkbox" 
          class="modern-toggle"
          :checked="sync.isActive()" 
          @change="e => sync.toggle((e.target as HTMLInputElement).checked)"
        />
        <span>SYNC</span>
      </label>
    </div>

    <!-- BPM Component (right of SYNC toggle) -->
    <div class="control-group bpm-group">
      <BpmComponent />
    </div>

    <div class="control-group">
      <RotaryKnob
        data-tooltip="Set the movement width for all the enslaved heads"
        :model-value="globalWidth.state.normalised"
        @update:model-value="globalWidth.setNormalisedValue"
        label="Width"
        :min="0"
        :max="100"
        size="small"
        unit="%"
      />
    </div>

    <!-- Feedback & Time Stacked -->
    <div class="control-group stacked-group">
      <RotaryKnob
        :model-value="globalFeedback.state.normalised"
        @update:model-value="globalFeedback.setNormalisedValue"
        data-tooltip="Set the feedback level for all the enslaved heads"
        label="Feedback"
        size="small"
        value-position="right"
        :min="0"
        :max="95"
        unit="%"
      />
      <RotaryKnob
        v-if="!sync.isActive()"
        :model-value="globalTimeNoSync.state.normalised"
        @update:model-value="globalTimeNoSync.setNormalisedValue"
        data-tooltip="Set the time for all the enslaved heads"
        label="Time"
        size="small"
        value-position="right"
        :min="0"
        :max="4"
        unit="s"
      />  
       <div v-if="sync.isActive()" class="time-selector-wrapper">
      <label class="time-selector-label">Time</label>
      <select
        data-tooltip="Set the time for delay according to tempo and notes values"
        class="time-selector"
        :value="selectedTimeValue"
        @change="e => setTimeValue((e.target as HTMLSelectElement).value)"
      >
        <option v-for="val in syncTimeValues" :key="val" :value="val">{{ val }}</option>
      </select>

    </div>
    </div>

    <!-- Center: Global MIX Knob -->
    <div class="control-group mix-group">
      <RotaryKnob
        :model-value="globalMix.state.normalised"
        @update:model-value="globalMix.setNormalisedValue"
        label="Mix"
        data-tooltip="Set the global mix of the Delay effect (0% = dry, 100% = wet)"
        size="medium"
        :min="0"
        :max="100"
        unit="%"
      />
    </div>

    <!-- LP & HP Filter Stacked -->
    <div class="control-group stacked-group">
      <div class="filter-control-wrapper">
        <RotaryKnob
          :model-value="globalLpFilter.state.normalised"
          @update:model-value="globalLpFilter.setNormalisedValue"
          data-tooltip="Set the global cutoff frequency for the Low-Pass Filter (20Hz - 20kHz)"
          label="LP Filter"
          size="small"
          :min="20"
          :max="20000"
          unit="Hz"
          value-position="right"
        />
        <label class="filter-bypass-toggle">
          <input 
            data-tooltip="Set the LP Filter On"
            type="checkbox" 
            class="bypass-checkbox"
            :checked="lpFilterBypass.isActive()" 
            @change="e => lpFilterBypass.toggle((e.target as HTMLInputElement).checked)"
          />
        </label>
      </div>
      <div class="filter-control-wrapper">
        <RotaryKnob
          :model-value="globalHpFilter.state.normalised"
          @update:model-value="globalHpFilter.setNormalisedValue"
          data-tooltip="Set the global cutoff frequency for the High-Pass Filter (20Hz - 20kHz)"
          label="HP Filter"
          :min="20"
          :max="20000"
          size="small"
          unit="Hz"
          value-position="right"
        />
        <label class="filter-bypass-toggle">
          <input 
            data-tooltip="Set the HP filter ON"
            type="checkbox" 
            class="bypass-checkbox"
            :checked="hpFilterBypass.isActive()" 
            @change="e => hpFilterBypass.toggle((e.target as HTMLInputElement).checked)"
          />
        </label>
      </div>
    </div>

    <!-- Ducking Component -->
    <DuckingComponent />

    <!-- Right: Mode Selection (3D Radio Buttons) -->
   
  </div>
</template>

<style scoped>
/* Use the same CSS variables from our design system */
:root {
  --primary-blue: #4a9eff;
  --primary-cyan: #00d4ff;
  --blue-glow: rgba(74, 158, 255, 0.6);
  --cyan-glow: rgba(0, 212, 255, 0.5);
  --metal-dark: #1a1d24;
  --metal-medium: #252830;
  --metal-light: #2f3239;
  --metal-lighter: #3a3d45;
  --text-primary: #e8eaed;
  --text-secondary: #9aa0a6;
  --text-dim: #6b7075;
  --shadow-deep: 0 4px 16px rgba(0, 0, 0, 0.6);
  --inset-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.5);
}

.bottom-section {
  width: 100%;
  height: 120px;
  background: linear-gradient(145deg, var(--metal-medium) 0%, var(--metal-dark) 100%);
  border-top: 1px solid rgba(74, 158, 255, 0.3);
  box-shadow: 0 -2px 8px rgba(0, 0, 0, 0.4), var(--inset-shadow);
  display: flex;
  align-items: center;
  padding: 8px 16px;
  flex-shrink: 0;
  justify-content: space-between;
}

.control-group {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 4px;
  flex: 1;
  height: 100%;
}

.sync-group {
  flex: 0.8;
  min-width: 60px;
}

.bpm-group {
  flex: 0.9;
  min-width: 80px;
}

.stacked-group {
  display: flex;
  flex-direction: column;
  gap: 4px;
  flex: 1;
  align-items: center;
  justify-content: center;
}

.filter-control-wrapper {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0;
  width: 100%;
  position: relative;
}

.filter-bypass-toggle {
  display: flex;
  align-items: center;
  gap: 0;
  cursor: pointer;
  user-select: none;
  position: absolute;
  bottom: 0;
  right: 0;
  margin: 0;
}

.bypass-checkbox {
  appearance: none;
  width: 10px;
  height: 10px;
  border: 1px solid rgba(74, 158, 255, 0.5);
  border-radius: 2px;
  background: var(--metal-dark);
  cursor: pointer;
  transition: all 0.2s ease;
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.4);
  padding: 0;
  flex-shrink: 0;
}

.bypass-checkbox:hover {
  border-color: var(--primary-cyan);
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.4), 0 0 4px rgba(0, 212, 255, 0.3);
}

.bypass-checkbox:checked {
  background: linear-gradient(135deg, var(--primary-blue), var(--primary-cyan));
  border-color: var(--primary-cyan);
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.3), 0 0 6px var(--cyan-glow);
}

.mix-group {
  flex: 1;
  justify-content: center;
}

.mode-group {
  flex: 1.2;
  gap: 6px;
}

/* Modern Toggle Switch */
.toggle-label {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 8px;
  cursor: pointer;
  font-size: 9px;
  font-weight: 600;
  color: var(--text-secondary);
  user-select: none;
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.toggle-label:hover {
  color: var(--text-primary);
}

.modern-toggle {
  appearance: none;
  width: 48px;
  height: 24px;
  background: var(--metal-dark);
  border-radius: 12px;
  border: 1px solid rgba(255, 255, 255, 0.1);
  cursor: pointer;
  position: relative;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  box-shadow: var(--inset-shadow);
}

.modern-toggle::before {
  content: '';
  position: absolute;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  top: 2px;
  left: 2px;
  background: linear-gradient(145deg, var(--metal-lighter), var(--metal-light));
  box-shadow: 
    0 2px 4px rgba(0, 0, 0, 0.3),
    0 0 0 1px rgba(255, 255, 255, 0.1);
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

.modern-toggle:checked {
  background: linear-gradient(90deg, var(--primary-blue), var(--primary-cyan));
  border-color: var(--primary-cyan);
  box-shadow: 
    var(--inset-shadow),
    0 0 12px var(--blue-glow);
}

.modern-toggle:checked::before {
  left: 26px;
  background: linear-gradient(145deg, #ffffff, #e8eaed);
  box-shadow: 
    0 2px 6px rgba(0, 0, 0, 0.4),
    0 0 8px rgba(0, 212, 255, 0.6);
}

.modern-toggle:hover:not(:checked)::before {
  background: linear-gradient(145deg, var(--metal-lighter), var(--metal-medium));
}

/* Mode Selection - 3D Buttons */
.mode-label {
  font-size: 8px;
  font-weight: 700;
  color: var(--text-dim);
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-bottom: 2px;
}

.mode-buttons {
  display: flex;
  flex-direction: column;
  gap: 4px;
  width: 100%;
}

.mode-button {
  padding: 6px 12px;
  font-size: 8px;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  transition: all 0.15s ease;
  user-select: none;
  color: var(--text-secondary);
  
  /* 3D raised effect */
  background: linear-gradient(145deg, var(--metal-lighter), var(--metal-light));
  box-shadow: 
    0 3px 0 var(--metal-dark),
    0 4px 6px rgba(0, 0, 0, 0.4),
    inset 0 1px 0 rgba(255, 255, 255, 0.1);
  border-top: 1px solid rgba(255, 255, 255, 0.15);
  border-left: 1px solid rgba(255, 255, 255, 0.1);
  border-right: 1px solid rgba(0, 0, 0, 0.2);
  border-bottom: 1px solid rgba(0, 0, 0, 0.3);
}

.mode-button:hover:not(.active) {
  background: linear-gradient(145deg, var(--metal-lighter), var(--metal-medium));
  transform: translateY(-1px);
  box-shadow: 
    0 4px 0 var(--metal-dark),
    0 5px 8px rgba(0, 0, 0, 0.5),
    inset 0 1px 0 rgba(255, 255, 255, 0.15);
}

.mode-button.active {
  /* 3D pressed effect */
  background: linear-gradient(145deg, var(--primary-blue), var(--primary-cyan));
  color: #ffffff;
  transform: translateY(3px);
  box-shadow: 
    0 0 0 var(--metal-dark),
    inset 0 2px 4px rgba(0, 0, 0, 0.5),
    0 0 12px var(--blue-glow);
  border-top: 1px solid rgba(0, 0, 0, 0.3);
  border-left: 1px solid rgba(0, 0, 0, 0.2);
  border-right: 1px solid rgba(255, 255, 255, 0.1);
  border-bottom: 1px solid rgba(255, 255, 255, 0.05);
  text-shadow: 0 0 6px var(--cyan-glow);
}

.mode-button:active:not(.active) {
  transform: translateY(2px);
  box-shadow: 
    0 1px 0 var(--metal-dark),
    inset 0 1px 2px rgba(0, 0, 0, 0.3);
}


/* Time Selector */
.time-selector-wrapper {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 6px;
}

.time-selector-label {
  font-size: 9px;
  font-weight: 600;
  color: var(--text-dim);
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.time-selector {
  padding: 6px 8px;
  font-size: 9px;
  font-weight: 600;
  background: var(--metal-dark);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 3px;
  color: var(--text-primary);
  cursor: pointer;
  transition: all 0.2s ease;
  appearance: none;
  text-align: center;
  min-width: 60px;
}

.time-selector:hover {
  border-color: var(--primary-cyan);
  box-shadow: 0 0 6px rgba(0, 212, 255, 0.3);
}

.time-selector:focus {
  outline: none;
  border-color: var(--primary-cyan);
  box-shadow: 
    0 0 6px rgba(0, 212, 255, 0.3),
    inset 0 1px 2px rgba(0, 0, 0, 0.3);
}

.time-selector option {
  background: var(--metal-medium);
  color: var(--text-primary);
}


</style>