<script setup lang="ts">
import { useJuce } from '@/composables/useJuce'
import { computed } from 'vue'
import RotaryKnob from './RotaryKnob.vue'
import { ref } from 'vue'

const { getSlider , onBackendEvent} = useJuce()
//const meterLevel = ref(0.7)
onBackendEvent('inputLevelEvent', (data) => {
 //   console.log('Received input level from C++:', data)
  meterLevel.value = data
})
// Hook into the JUCE parameter
const inputGain = computed(() => getSlider('INPUT_GAIN'))
// Example fixed value as requested
let meterLevel =ref(0.7)
</script>

<template>
  <div class="input-container">
    <div class="input-header">
      <h3>INPUT</h3>
    </div>
    
    <div class="input-content">
      <div class="meter-area">
        <div class="meter-track">
          <div class="meter-fill" :style="{ height: (meterLevel * 100) + '%' }"></div>
        </div>
        <div class="meter-track">
          <div class="meter-fill" :style="{ height: (meterLevel * 100) + '%' }"></div>
        </div>
      </div>

      <div class="gain-area">
        <RotaryKnob
            data-tooltip="Set input gain for the delay effect"
          :model-value="inputGain.state.normalised"
          @update:model-value="inputGain.setNormalisedValue"
          label="GAIN"
          size="small"
          :min="-12"
          :max="12"
          unit="dB"
        />
      </div>
    </div>
  </div>
</template>

<style scoped>
.input-container {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
  padding: 10px 6px;
  background: linear-gradient(180deg, #2a2d3a 0%, #16181e 100%);
  border-radius: 4px;
  border: 1px solid #444;
  box-shadow: inset 0 1px 1px rgba(255,255,255,0.05);
}

.input-header {
  text-align: center;
  margin-bottom: 8px;
  flex-shrink: 0;
}

.input-header h3 {
  margin: 0;
  font-size: 9px;
  letter-spacing: 1.5px;
  color: #4a9eff;
  text-transform: uppercase;
}

.input-content {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 12px;
}

/* Meter Area (80% Height) */
.meter-area {
  flex: 0 0 75%; /* Giving meters the majority of the vertical space */
  display: flex;
  justify-content: center;
  gap: 6px;
  padding: 0 4px;
}

.meter-track {
  flex: 1; /* Stretch to fill available width */
  height: 100%;
  background: #000;
  border-radius: 2px;
  position: relative;
  overflow: hidden;
  border: 1px solid #333;
}

.meter-fill {
  position: absolute;
  bottom: 0;
  width: 100%;
  /* Audio-style gradient: Green -> Yellow -> Red at the top */
  background: linear-gradient(to top, 
    #2ecc71 0%, 
    #2ecc71 70%, 
    #f1c40f 85%, 
    #e74c3c 100%
  );
  transition: height 0.05s ease-out;
}

/* Gain Area */
.gain-area {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  border-top: 1px solid #333;
  padding-top: 8px;
}
</style>