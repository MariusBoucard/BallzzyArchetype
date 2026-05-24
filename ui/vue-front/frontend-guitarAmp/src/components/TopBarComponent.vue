<script setup lang="ts">
import { useJuce } from '@/composables/useJuce'
import { ref } from 'vue'
import AboutModal from './AboutModal.vue'
import SettingsModal from './SettingsModal.vue'
import PresetManagerComponent from './PresetManagerComponent.vue'
import { nextTick } from 'vue'
const { initData } = useJuce()
const showAboutModal = ref(false)
const showSettingsModal = ref(false)
const aboutModalRef = ref<InstanceType<typeof AboutModal> | null>(null)
const settingsModalRef = ref<InstanceType<typeof SettingsModal> | null>(null)
const openAbout = () => { console.log('Opening about modal'); aboutModalRef.value?.open() }
const openSettings = async () => {

  settingsModalRef.value?.open()

}


</script>

<template>
  <div class="topbar">
    <div class="topbar-left">
      <button class="btn-about" @click="openAbout">
        <span class="btn-icon">ℹ</span>
        <span class="btn-text">About</span>
      </button>
      <PresetManagerComponent />
    </div>

    <div class="topbar-center">
      <h1 class="app-title">Guitar Amp</h1>
    </div>

    <div class="topbar-right">
      <button class="btn-settings" @click="openSettings">
        <span class="settings-icon">⚙</span>
      </button>
    </div>
  </div>

  <AboutModal ref="aboutModalRef"   />
  <SettingsModal ref="settingsModalRef"  />
</template>

<style scoped>
.topbar {
  width: 100%;
  height: 35px;
  background: linear-gradient(135deg, #1a1d2e 0%, #16192b 100%);
  border-bottom: 1px solid #4a9eff;
  box-shadow: 0 2px 6px rgba(0, 0, 0, 0.4);
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 8px;
  flex-shrink: 0;
  gap: 8px;
}

.topbar-left {
  display: flex;
  align-items: center;
  gap: 8px;
  flex-shrink: 0;
}

.topbar-center {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
}

.topbar-right {
  display: flex;
  align-items: center;
  flex-shrink: 0;
}

.btn-about {
  display: flex;
  align-items: center;
  gap: 4px;
  padding: 6px 10px;
  background: rgba(74, 158, 255, 0.1);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 6px;
  color: #e0e0e0;
  font-size: 11px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s ease;
  backdrop-filter: blur(10px);
  white-space: nowrap;
}

.btn-about:hover {
  background: rgba(74, 158, 255, 0.15);
  border-color: rgba(74, 158, 255, 0.5);
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(74, 158, 255, 0.2);
}

.btn-about:active {
  transform: translateY(0);
}

.btn-icon {
  font-size: 12px;
}

.btn-text {
  white-space: nowrap;
}

.app-title {
  margin: 0;
  font-size: 13px;
  font-weight: 700;
  color: #e0e0e0;
  letter-spacing: 0.3px;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.btn-settings {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 28px;
  height: 28px;
  padding: 0;
  background: rgba(74, 158, 255, 0.1);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 6px;
  color: #e0e0e0;
  font-size: 14px;
  cursor: pointer;
  transition: all 0.2s ease;
  backdrop-filter: blur(10px);
}

.btn-settings:hover {
  background: rgba(74, 158, 255, 0.15);
  border-color: rgba(74, 158, 255, 0.5);
  transform: translateY(-1px) rotate(15deg);
  box-shadow: 0 4px 12px rgba(74, 158, 255, 0.2);
}

.btn-settings:active {
  transform: translateY(0) rotate(0deg);
}

.settings-icon {
  display: flex;
  align-items: center;
  justify-content: center;
}
</style>
