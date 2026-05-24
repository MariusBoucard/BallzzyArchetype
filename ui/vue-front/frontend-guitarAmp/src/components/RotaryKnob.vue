<script setup lang="ts">
import { computed, ref, onUnmounted } from 'vue'

interface Props {
  modelValue: number  // 0 to 1
  label?: string
  size?: 'small' | 'medium' | 'large'
  min?: number
  max?: number
  step?: number
  unit?: string
  valuePosition?: 'top' | 'bottom' | 'left' | 'right' | 'hidden'
  values?: (string | number)[]
  useStrip?: boolean
  stripFrames?: number
  stripColumns?: number
  stripFrameWidth?: number
  stripFrameHeight?: number
}

const props = withDefaults(defineProps<Props>(), {
  label: '',
  size: 'medium',
  min: 0,
  unit: '',
  max: 1,
  step: 0.01,
  useStrip: false,
  stripFrames: 260,
  stripColumns: 5,
  stripFrameWidth: 135,
  stripFrameHeight: 153,
})

const emit = defineEmits<{
  'update:modelValue': [value: number]
}>()

const isDragging = ref(false)
const startY = ref(0)
const startValue = ref(0)

// Size configurations
const sizeConfig = {
  /*small: { size: 50, ringWidth: 4 },
  medium: { size: 70, ringWidth: 5 },
  large: { size: 90, ringWidth: 6 }*/

   small: { size: 30, ringWidth: 4 },
  medium: { size: 50, ringWidth: 5 },
  large: { size: 70, ringWidth: 6 }
}

const config = computed(() => sizeConfig[props.size])

// Check if we're using discrete values
const isDiscrete = computed(() => !!props.values && props.values.length > 0)

// For discrete mode: map modelValue to the closest discrete index
const discreteIndex = computed(() => {
  if (!isDiscrete.value) return 0
  const index = Math.round(props.modelValue * (props.values!.length - 1))
  return Math.max(0, Math.min(props.values!.length - 1, index))
})

// Get currently selected discrete value
const currentDiscreteValue = computed(() => {
  if (!isDiscrete.value) return null
  return props.values![discreteIndex.value]
})

// Calculate rotation angle (270 degrees range, starting from -135deg to +135deg)
const rotationAngle = computed(() => {
  if (isDiscrete.value) {
    // For discrete values, snap to specific positions
    const anglePerStep = 270 / (props.values!.length - 1)
    return -135 + (discreteIndex.value * anglePerStep)
  }
  return -135 + (props.modelValue * 270)
})

const frameCount = computed(() => props.stripFrames ?? 260)
const frameColumns = computed(() => props.stripColumns ?? 5)
const frameRows = computed(() => Math.ceil(frameCount.value / frameColumns.value))
const frameWidth = computed(() => props.stripFrameWidth ?? 135)
const frameHeight = computed(() => props.stripFrameHeight ?? 153)

const stripFrameIndex = computed(() => {
  if (!props.useStrip) return 0
  if (isDiscrete.value) {
    return Math.round(props.modelValue * (frameCount.value - 1))
  }
  return Math.round(props.modelValue * (frameCount.value - 1))
})

const stripColumn = computed(() => stripFrameIndex.value % frameColumns.value)
const stripRow = computed(() => Math.floor(stripFrameIndex.value / frameColumns.value))

const stripStyles = computed(() => {
  if (!props.useStrip) return {}

  const displaySize = config.value.size
  return {
    backgroundImage: `url('/knob_strip.png')`,
    backgroundSize: `${displaySize * frameColumns.value}px ${displaySize * frameRows.value}px`,
    backgroundPosition: `-${stripColumn.value * displaySize}px -${stripRow.value * displaySize}px`,
    backgroundRepeat: 'no-repeat',
    width: `${displaySize}px`,
    height: `${displaySize}px`,
  }
})

// Calculate the conic gradient for progress ring
// Use a simple 0-270deg gradient and rotate the container to align it
const progressGradient = computed(() => {
  const fillDeg = props.modelValue * 270 // How much to fill (0 to 270 degrees)
  
  return `conic-gradient(
    var(--primary-cyan) 0deg,
    var(--primary-blue) ${fillDeg}deg,
    transparent ${fillDeg}deg
  )`
})

// Rotate the progress ring to align with the indicator starting position
const progressRotation = computed(() => {
  return 225 // Start at 225deg to align with -135deg indicator position
})

const handleMouseDown = (e: MouseEvent) => {
  isDragging.value = true
  startY.value = e.clientY
  startValue.value = props.modelValue
  console.log("Mouse down on knob, starting value:", startValue.value)
  document.addEventListener('mousemove', handleMouseMove)
  document.addEventListener('mouseup', handleMouseUp)
  e.preventDefault()
}

const handleMouseMove = (e: MouseEvent) => {
  if (!isDragging.value) return
  
  const deltaY = startY.value - e.clientY
  const sensitivity = 0.003 // Adjust sensitivity
  let newValue = startValue.value + (deltaY * sensitivity)
  
  // Clamp between 0 and 1
  newValue = Math.max(0, Math.min(1, newValue))
  
  // If discrete mode, snap to nearest discrete value
  if (isDiscrete.value) {
    const numSteps = props.values!.length - 1
    const snappedIndex = Math.round(newValue * numSteps)
    newValue = snappedIndex / numSteps
  }
  
  console.log("Mouse move, deltaY:", deltaY, "new value:", newValue)
  emit('update:modelValue', newValue)
}

const handleMouseUp = () => {
  isDragging.value = false
  document.removeEventListener('mousemove', handleMouseMove)
  document.removeEventListener('mouseup', handleMouseUp)
}

const handleWheel = (e: WheelEvent) => {
  e.preventDefault()
  let newValue = props.modelValue
  
  if (isDiscrete.value) {
    // For discrete mode, move to next/previous value
    const delta = e.deltaY > 0 ? -1 : 1
    const newIndex = Math.max(0, Math.min(props.values!.length - 1, discreteIndex.value + delta))
    newValue = newIndex / (props.values!.length - 1)
  } else {
    // For continuous mode, use small increments
    const delta = e.deltaY > 0 ? -0.01 : 0.01
    newValue = props.modelValue + delta
    newValue = Math.max(0, Math.min(1, newValue))
  }
  
  emit('update:modelValue', newValue)
}

// Cleanup on unmount
onUnmounted(() => {
  document.removeEventListener('mousemove', handleMouseMove)
  document.removeEventListener('mouseup', handleMouseUp)
})

// Calculate position of discrete values around the knob
const getDiscreteValuePosition = (index: number): Record<string, string> => {
  if (!isDiscrete.value) return {}
  
  const numValues = props.values!.length
  const anglePerValue = 270 / (numValues - 1) // 270 degrees total range
  const startAngle = -270 // Starting at -135 degrees
  const angleOffset = anglePerValue / 2 // Center values within their arc
  const angle = startAngle + angleOffset + (index * anglePerValue)
  
  // Convert angle to position around a circle
  const radius = config.value.size * 0.8 // Position well outside the knob
  const radians = (angle * Math.PI) / 180
  const x = Math.cos(radians) * radius
  const y = Math.sin(radians) * radius
  
  return {
    left: '50%',
    top: '50%',
    transform: `translate(calc(-50% + ${x}px), calc(-50% + ${y}px))`
  }
}

// Format the display value intelligently based on unit type
const formatValue = computed(() => {
  const value = props.min + (props.modelValue * (props.max - props.min))
  
  // Handle percentage - show full precision
  if (props.unit === '%') {
    return { formatted: value.toFixed(2), unit: props.unit }
  }
  
  // Handle Hz - convert to kHz if >= 1000
  if (props.unit === 'Hz') {
    if (value >= 1000) {
      return { formatted: (value / 1000).toFixed(2), unit: 'kHz' }
    }
    return { formatted: value.toFixed(0), unit: 'Hz' }
  }
  
  // Handle seconds - convert to ms if < 1
  if (props.unit === 's') {
    if (value < 1) {
      return { formatted: (value * 1000).toFixed(0), unit: 'ms' }
    }
    return { formatted: value.toFixed(2), unit: 's' }
  }
  if (props.unit === 'ms') {
    if (value >= 1000) {
      return { formatted: (value / 1000).toFixed(2), unit: 's' }
    }
    return { formatted: value.toFixed(0), unit: 'ms' }
  }
  
  // Handle dB - show 2 decimals
  if (props.unit === 'dB') {
    return { formatted: value.toFixed(2), unit: 'dB' }
  }
  
  // Default
  return { formatted: value.toFixed(2), unit: props.unit }
})

const displayValue = computed(() => formatValue.value.formatted)
</script>

<template>
  <div class="rotary-knob-container">
    <div 
      class="knob-wrapper"
      :class="{ 'is-dragging': isDragging, 'has-strip': props.useStrip, [`size-${size}`]: true }"
      :style="{ width: config.size + 'px', height: config.size + 'px' }"
      @mousedown="handleMouseDown"
      @wheel="handleWheel"
    >
      <!-- Background ring (inactive portion) -->
      <div v-if="props.useStrip" class="knob-strip" :style="stripStyles"></div>
      <div v-else class="ring-background" :style="{ transform: `rotate(${progressRotation}deg)` }"></div>
      
      <!-- Progress ring (active portion) -->
      <div v-if="!props.useStrip" 
        class="ring-progress"
        :style="{ 
          background: progressGradient,
          transform: `rotate(${progressRotation}deg)`
        }"
      ></div>
      
      <!-- Inner space to create ring effect -->
      <div v-if="!props.useStrip" class="ring-space"></div>
      
      <!-- Central knob with indicator -->
      <div v-if="!props.useStrip" class="knob-center">
        <div 
          class="knob-indicator-track"
          :style="{ transform: `rotate(${rotationAngle}deg)` }"
        >   

          <div class="knob-indicator"></div>
        </div>

      </div>
      <div v-if="valuePosition === 'left'" class="knob-value" style="margin-right: 56px;">{{ displayValue }} {{ formatValue.unit }}</div>
      <div v-if="valuePosition === 'right'" class="knob-value" style="margin-left: 56px;">{{ displayValue }} {{ formatValue.unit }}</div>

      <!-- Discrete values displayed around knob -->
      <div v-if="isDiscrete" class="discrete-values-container">
        <div 
          v-for="(value, index) in values" 
          :key="index"
          class="discrete-value"
          :class="{ active: index === discreteIndex }"
          :style="getDiscreteValuePosition(index)"
        >
          {{ value }}
        </div>
      </div>
    </div>
    
    <!-- Label -->
    <div v-if="label" class="knob-label">{{ label }}</div>
    
    <!-- Value display -->
    <div v-if="valuePosition !== 'hidden' && valuePosition !== 'left' && valuePosition !== 'right'" class="knob-value">{{ displayValue }}
      <span v-if="unit" class="knob-unit">{{ formatValue.unit }}</span>

    </div>
  </div>
</template>

<style scoped>
.rotary-knob-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 3px;
  user-select: none;
}

.knob-wrapper {
  position: relative;
  cursor: pointer;
  transition: transform 0.1s ease;
  display: flex;
  align-items: center;
  justify-content: center;
}

.knob-strip {
  position: absolute;
  inset: 0;
  border-radius: 50%;
}

.knob-wrapper.has-strip .ring-background,
.knob-wrapper.has-strip .ring-progress,
.knob-wrapper.has-strip .ring-space,
.knob-wrapper.has-strip .knob-center,
.knob-wrapper.has-strip .knob-indicator-track {
  display: none;
}

.knob-wrapper:hover {
  transform: scale(1.02);
}

.knob-wrapper.is-dragging {
  cursor: grabbing;
}

.knob-wrapper:active {
  transform: scale(0.98);
}

/* Background ring - the inactive track */
.ring-background {
  position: absolute;
  width: 100%;
  height: 100%;
  border-radius: 50%;
  background: conic-gradient(
    rgba(255, 255, 255, 0.05) 0deg,
    rgba(255, 255, 255, 0.05) 270deg,
    transparent 270deg
  );
}

/* Progress ring - the active portion */
.ring-progress {
  position: absolute;
  width: 100%;
  height: 100%;
  border-radius: 50%;
  transition: background 0.05s ease;
  filter: drop-shadow(0 0 6px var(--blue-glow));
}

/* Inner space - creates the ring effect */
.ring-space {
  position: absolute;
  width: 70%;
  height: 70%;
  border-radius: 50%;
  background: linear-gradient(145deg, var(--metal-medium), var(--metal-dark));
  box-shadow: var(--inset-shadow);
}

/* Central knob body */
.knob-center {
  position: absolute;
  width: 60%;
  height: 60%;
  border-radius: 50%;
  background: radial-gradient(circle at 30% 30%, var(--metal-lighter), var(--metal-light));
  box-shadow: 
    inset 0 2px 4px rgba(0, 0, 0, 0.6),
    inset 0 -1px 2px rgba(255, 255, 255, 0.05),
    0 2px 8px rgba(0, 0, 0, 0.5);
}

/* Indicator track (rotates around center) */
.knob-indicator-track {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  transition: transform 0.05s ease;
}

/* Indicator dot */
.knob-indicator {
  position: absolute;
  top: 8%;
  left: 50%;
  transform: translateX(-50%);
  width: 8px; 
  height: 8px;
  background: radial-gradient(circle, var(--primary-cyan), var(--primary-blue));
  border-radius: 50%;
  box-shadow: 
    0 0 8px var(--cyan-glow),
    0 0 4px var(--blue-glow);
}

.size-small .knob-indicator {
  width: 5px;
  height: 5px;
}

.size-medium .knob-indicator {
  width: 7px;
  height: 7px;
}

.size-large .knob-indicator {
  width: 9px;
  height: 9px;
}

/* Label */
.knob-label {
  font-size: 9px;
  font-weight: 600;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.5px;
  text-align: center;
}

/* Value Display */
.knob-value {
  font-size: 8px;
  font-weight: 600;
  color: var(--text-dim);
  font-family: 'Courier New', monospace;
  text-align: center;
}

/* Discrete Values Around Knob */
.discrete-values-container {
  position: absolute;
  width: 100%;
  height: 100%;
  top: 0;
  left: 0;
  pointer-events: none;
}

.discrete-value {
  position: absolute;
  font-size: 9px;
  font-weight: 600;
  color: var(--text-secondary);
  white-space: nowrap;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  transition: all 0.15s ease;
}

.discrete-value.active {
  color: var(--primary-cyan);
  text-shadow: 0 0 8px var(--cyan-glow);
  font-weight: 700;
}
</style>