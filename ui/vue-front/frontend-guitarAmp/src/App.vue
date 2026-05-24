<script setup lang="ts">
import { ref } from 'vue'
import TopBarComponent from './components/TopBarComponent.vue'
import GenericParametersPanel from './components/GenericParametersPanel.vue'
import InputEffectsTab from './components/InputEffectsTab.vue'
import AmpTab from './components/AmpTab.vue'
import PostEqTab from './components/PostEqTab.vue'
import PostEffectsTab from './components/PostEffectsTab.vue'

const tabs = ['Input Effects', 'Amplifier', 'Post EQ', 'Post Effects']
const activeTab = ref(0)
</script>

<template>
  <div id="app" class="tooltips-active">
    <TopBarComponent />

    <main class="plugin-shell">
      <div class="tabs-row">
        <button
          v-for="(tab, index) in tabs"
          :key="tab"
          :class="['tab-button', { active: index === activeTab }]"
          @click="activeTab = index"
        >
          {{ tab }}
        </button>
      </div>

      <GenericParametersPanel />

      <div class="tab-content">
        <InputEffectsTab v-if="activeTab === 0" />
        <AmpTab v-if="activeTab === 1" />
        <PostEqTab v-if="activeTab === 2" />
        <PostEffectsTab v-if="activeTab === 3" />
      </div>
    </main>
  </div>
</template>

<style scoped>
html,
body,
#app {
  margin: 0;
  padding: 0;
  min-height: 100%;
  width: 100%;
}

* {
  box-sizing: border-box;
}

#app {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
  background: radial-gradient(circle at top left, rgba(74, 158, 255, 0.14), transparent 25%),
    linear-gradient(180deg, #0f121a 0%, #141920 100%);
}

.plugin-shell {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 16px;
  padding: 18px;
  width: 100%;
  max-width: 1536px;
  margin: 0 auto;
}

.tabs-row {
  display: flex;
  flex-wrap: wrap;
  gap: 12px;
}

.tab-button {
  border: 1px solid rgba(255, 255, 255, 0.08);
  background: rgba(255, 255, 255, 0.04);
  color: #bcc9ed;
  padding: 12px 18px;
  border-radius: 999px;
  font-size: 12px;
  font-weight: 700;
  letter-spacing: 0.14em;
  cursor: pointer;
  transition: all 0.18s ease;
}

.tab-button:hover {
  background: rgba(74, 158, 255, 0.12);
}

.tab-button.active {
  background: linear-gradient(90deg, #4a9eff, #00d4ff);
  color: #08121f;
  border-color: rgba(0, 212, 255, 0.45);
}

.tab-content {
  display: flex;
  flex-direction: column;
  gap: 16px;
  min-height: 0;
}

 
    .toggle-switch {
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
 
    .toggle-switch::before {
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
 
    .toggle-switch:checked {
      background: linear-gradient(90deg, var(--primary-blue), var(--primary-cyan));
      border-color: var(--primary-cyan);
      box-shadow: 
        var(--inset-shadow),
        0 0 12px var(--blue-glow);
    }
 
    .toggle-switch:checked::before {
      left: 26px;
      background: linear-gradient(145deg, #ffffff, #e8eaed);
      box-shadow: 
        0 2px 6px rgba(0, 0, 0, 0.4),
        0 0 8px rgba(0, 212, 255, 0.6);
    }
 
    .toggle-label-text {
      font-size: 11px;
      font-weight: 500;
      color: var(--text-secondary);
      transition: color 0.3s ease;
    }
 
    .toggle-switch:checked + .toggle-label-text {
      color: var(--text-primary);
    }
 
    /* Slider Styles */
    .slider-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 8px;
      width: 200px;
    }
 
    .slider-wrapper {
      width: 100%;
      padding: 12px;
      background: linear-gradient(145deg, var(--metal-light), var(--metal-dark));
      border: 1px solid rgba(255, 255, 255, 0.08);
      border-radius: 8px;
      box-shadow: var(--inset-shadow), var(--shadow-soft);
    }
 
    .slider {
      width: 100%;
      height: 4px;
      -webkit-appearance: none;
      appearance: none;
      background: var(--metal-dark);
      border-radius: 2px;
      outline: none;
      cursor: pointer;
      box-shadow: var(--inset-shadow);
    }
 
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 14px;
      height: 14px;
      border-radius: 50%;
      background: linear-gradient(145deg, var(--primary-cyan), var(--primary-blue));
      cursor: pointer;
      box-shadow: 
        0 2px 4px rgba(0, 0, 0, 0.4),
        0 0 8px var(--blue-glow);
      transition: all 0.2s ease;
    }
 
    .slider::-webkit-slider-thumb:hover {
      transform: scale(1.1);
      box-shadow: 
        0 2px 6px rgba(0, 0, 0, 0.5),
        0 0 12px var(--cyan-glow);
    }
 
    .slider::-moz-range-thumb {
      width: 14px;
      height: 14px;
      border-radius: 50%;
      background: linear-gradient(145deg, var(--primary-cyan), var(--primary-blue));
      cursor: pointer;
      border: none;
      box-shadow: 
        0 2px 4px rgba(0, 0, 0, 0.4),
        0 0 8px var(--blue-glow);
      transition: all 0.2s ease;
    }
 
    .color-palette {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));
      gap: 15px;
      margin-top: 20px;
    }
 
    .color-swatch {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 8px;
    }
 
    .color-box {
      width: 80px;
      height: 80px;
      border-radius: 8px;
      box-shadow: var(--shadow-medium);
      border: 1px solid rgba(255, 255, 255, 0.1);
    }
 
    .color-name {
      font-size: 9px;
      color: var(--text-secondary);
      text-align: center;
    }
 
    .color-code {
      font-size: 8px;
      color: var(--text-dim);
      font-family: 'Courier New', monospace;
    }

[data-tooltip]::before,
[data-tooltip]::after {
  position: absolute;
  left: 50%;
  pointer-events: none;
  opacity: 0;
  transition: all 0.15s ease;
  z-index: 999;
  
  /* FIX: Ensure these don't take up space or affect parent width */
  visibility: hidden;
  width: 0;
  height: 0;
  overflow: visible;
}

/* 2. The Tooltip Box */
[data-tooltip]::before {
  content: attr(data-tooltip);
  /* FIX: Change 'bottom' to 'top' if the top of your component is getting clipped */
  bottom: 100%; 
  margin-bottom: 8px;
  
  transform: translateX(-50%) translateY(5px);
  background-color: #2a2a2a;
  color: #fff;
  padding: 6px 10px;
  border-radius: 4px;
  font-size: 11px;
  border: 1px solid #444;
  
  /* FIX: Use max-content instead of nowrap to prevent extreme stretching */
  width: max-content;
  max-width: 150px; /* Adjust this to fit your narrowest container */
  white-space: normal; /* Allows text to wrap if the tooltip is long */
  line-height: 1.2;
  text-align: center;
}

/* 3. The Arrow */
[data-tooltip]::after {
  content: '';
  bottom: 100%;
  margin-bottom: -4px;
  transform: translateX(-50%);
  border: 6px solid transparent;
  border-top-color: #2a2a2a;
}

/* 4. Show Logic */
.tooltips-active [data-tooltip]:hover::before,
.tooltips-active [data-tooltip]:hover::after {
  opacity: 1;
  visibility: visible;
  width: auto;
  height: auto;
  transform: translateX(-50%) translateY(0);
}

/* 5. Trigger utility */
[data-tooltip] {
  position: relative;
  /* Ensure the trigger itself doesn't stretch things */
  display: inline-block; 
}
</style>
