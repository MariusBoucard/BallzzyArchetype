<script setup lang="ts">
import { useJuce } from '@/composables/useJuce'
import { computed, ref } from 'vue'

const { getSlider, getToggle } = useJuce()

// Get BPM controls
const userBpm = computed(() => getSlider('USER_BPM'))
const bpmFromHost = computed(() => getToggle('BPM_FROM_HOST'))

// BPM range
const BPM_MIN = 20
const BPM_MAX = 300

// Compute actual BPM value from normalized value
const bpmValue = computed(() => {
  return Math.round(BPM_MIN + (userBpm.value.state.normalised * (BPM_MAX - BPM_MIN)))
})

// State for editing
const isEditing = ref(false)
const inputValue = ref(bpmValue.value.toString())
const isDragging = ref(false)
const startY = ref(0)
const startValue = ref(0)

// Update input value when bpmValue changes (if not editing)
const updateInputFromModel = () => {
  if (!isEditing.value) {
    inputValue.value = bpmValue.value.toString()
  }
}

// Handle focus - start editing
const handleFocus = () => {
  isEditing.value = true
  inputValue.value = bpmValue.value.toString()
}

// Handle blur - save value
const handleBlur = () => {
  commitValue()
  isEditing.value = false
}

// Handle input change
const handleInput = (e: Event) => {
  const target = e.target as HTMLInputElement
  inputValue.value = target.value
}

// Handle key press
const handleKeyPress = (e: KeyboardEvent) => {
  if (e.key === 'Enter') {
    commitValue()
    isEditing.value = false
  } else if (e.key === 'Escape') {
    isEditing.value = false
    inputValue.value = bpmValue.value.toString()
  }
}

// Parse and set the value
const commitValue = () => {
  const parsed = parseFloat(inputValue.value)
  if (!isNaN(parsed)) {
    const clamped = Math.max(BPM_MIN, Math.min(BPM_MAX, Math.round(parsed)))
    const normalised = (clamped - BPM_MIN) / (BPM_MAX - BPM_MIN)
    userBpm.value.setNormalisedValue(normalised)
    inputValue.value = clamped.toString()
  }
}

// Mouse drag handling for value change
const handleMouseDown = (e: MouseEvent) => {
  if (isEditing.value) return // Don't drag while editing text
  
  isDragging.value = true
  startY.value = e.clientY
  startValue.value = bpmValue.value
  
  document.addEventListener('mousemove', handleMouseMove)
  document.addEventListener('mouseup', handleMouseUp)
  e.preventDefault()
}

const handleMouseMove = (e: MouseEvent) => {
  if (!isDragging.value) return
  
  const deltaY = startY.value - e.clientY
  const sensitivity = 0.5 // Adjust sensitivity - pixels per BPM
  let newBpm = startValue.value + (deltaY / sensitivity)
  
  // Clamp between min and max
  newBpm = Math.max(BPM_MIN, Math.min(BPM_MAX, Math.round(newBpm)))
  
  const normalised = (newBpm - BPM_MIN) / (BPM_MAX - BPM_MIN)
  userBpm.value.setNormalisedValue(normalised)
}

const handleMouseUp = () => {
  isDragging.value = false
  document.removeEventListener('mousemove', handleMouseMove)
  document.removeEventListener('mouseup', handleMouseUp)
}

// Watch for external changes to the model
const handleValueChange = () => {
  updateInputFromModel()
}
</script>

<template>
  <div class="bpm-component">
    <!-- BPM Value Control -->
    <div class="bpm-control-wrapper">
      <input
        data-tooltip="set Bpm for sync timings"
        :value="isEditing ? inputValue : bpmValue"
        type="text"
        class="bpm-input"
        :class="{ 'is-editing': isEditing, 'is-dragging': isDragging }"
        @focus="handleFocus"
        @blur="handleBlur"
        @input="handleInput"
        @keypress="handleKeyPress"
        @mousedown="handleMouseDown"
        placeholder="BPM"
      />
      <div class="bpm-unit">BPM</div>
    </div>

    <!-- BPM From Host Toggle -->
     <template         data-tooltip="use bpm from the host instehead of defined value">


    <label class="bpm-toggle-label">
      <input 
        type="checkbox" 
        class="bpm-toggle"
        :checked="bpmFromHost.isActive()" 
        @change="e => bpmFromHost.toggle((e.target as HTMLInputElement).checked)"
      />
      <span>HOST</span>
    </label>
         </template>
  </div>
</template>

<style scoped>
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

.bpm-component {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 8px;
  height: 100%;
}

.bpm-control-wrapper {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 4px;
  position: relative;
}

.bpm-input {
  width: 50px;
  padding: 6px 8px;
  background: var(--metal-dark);
  border: 1px solid rgba(74, 158, 255, 0.5);
  border-radius: 4px;
  color: var(--text-primary);
  font-size: 14px;
  font-weight: 600;
  text-align: center;
  cursor: grab;
  transition: all 0.2s ease;
  box-shadow: var(--inset-shadow);
  user-select: none;
  font-family: 'Courier New', monospace;
  letter-spacing: 1px;
}

.bpm-input:hover:not(.is-editing) {
  border-color: var(--primary-cyan);
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.4), 0 0 4px rgba(0, 212, 255, 0.3);
  background: var(--metal-medium);
}

.bpm-input:focus {
  outline: none;
  background: var(--metal-medium);
  border-color: var(--primary-blue);
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.4), 0 0 6px rgba(74, 158, 255, 0.5);
  cursor: text;
  color: var(--primary-cyan);
}

.bpm-input.is-dragging {
  cursor: grabbing;
  border-color: var(--primary-blue);
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.4), 0 0 8px var(--blue-glow);
}

.bpm-unit {
  font-size: 11px;
  font-weight: 600;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-left: 2px;
}

/* Toggle Label for BPM From Host */
.bpm-toggle-label {
  display: flex;
  align-items: center;
  gap: 6px;
  cursor: pointer;
  font-size: 9px;
  font-weight: 600;
  color: var(--text-secondary);
  user-select: none;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  transition: color 0.2s ease;
}

.bpm-toggle-label:hover {
  color: var(--text-primary);
}

.bpm-toggle {
  appearance: none;
  width: 36px;
  height: 18px;
  background: var(--metal-dark);
  border-radius: 9px;
  border: 1px solid rgba(255, 255, 255, 0.1);
  cursor: pointer;
  position: relative;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  box-shadow: var(--inset-shadow);
  padding: 0;
  flex-shrink: 0;
}

.bpm-toggle::before {
  content: '';
  position: absolute;
  width: 14px;
  height: 14px;
  border-radius: 50%;
  top: 1px;
  left: 1px;
  background: linear-gradient(145deg, var(--metal-lighter), var(--metal-light));
  box-shadow: 
    0 2px 4px rgba(0, 0, 0, 0.3),
    0 0 0 1px rgba(255, 255, 255, 0.1);
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

.bpm-toggle:checked {
  background: linear-gradient(90deg, var(--primary-blue), var(--primary-cyan));
  border-color: var(--primary-cyan);
  box-shadow: 
    var(--inset-shadow),
    0 0 8px var(--blue-glow);
}

.bpm-toggle:checked::before {
  left: 20px;
  background: linear-gradient(145deg, #ffffff, #e8eaed);
  box-shadow: 
    0 2px 4px rgba(0, 0, 0, 0.4),
    0 0 6px rgba(0, 212, 255, 0.6);
}

.bpm-toggle:hover:not(:checked) {
  border-color: rgba(74, 158, 255, 0.7);
}

.bpm-toggle:hover:not(:checked)::before {
  background: linear-gradient(145deg, var(--metal-lighter), var(--metal-medium));
}
</style>
