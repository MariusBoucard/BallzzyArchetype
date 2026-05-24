
<script setup lang="ts">
import { ref } from 'vue';

const isOpen = ref(false)

const emit = defineEmits<{
  close: []
}>()


const open = () => { isOpen.value = true }
const close = () => { isOpen.value = false; emit('close') }

defineExpose({ open, close })
</script>

<template>
  <div v-if="isOpen" class="modal-overlay" @click="close">
    <div class="modal-content" @click.stop>
      <div class="modal-header">
        <h2>Settings</h2>
          <button class="close-button" @click="close">✕</button>
      </div>

      <div class="modal-body">
        <p class="empty-message">No settings available at this time</p>
      </div>

      <div class="modal-footer">
        <button class="btn-close" @click="close">Close</button>
      </div>
    </div>
  </div>
</template>

<style scoped>
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background: rgba(0, 0, 0, 0.65);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999;
}

.modal-content {
  position: relative;
  background: linear-gradient(135deg, #2a2d3a 0%, #1f2129 100%);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 12px;
  width: 90%;
  max-width: 400px;
  box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.37);
  backdrop-filter: blur(4px);
  display: flex;
  flex-direction: column;
}

.modal-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 16px;
  border-bottom: 1px solid rgba(74, 158, 255, 0.2);
}

.modal-header h2 {
  margin: 0;
  font-size: 16px;
  font-weight: 700;
  color: #4a9eff;
}

.close-button {
  background: none;
  border: none;
  color: #999;
  font-size: 18px;
  cursor: pointer;
  padding: 0;
  width: 24px;
  height: 24px;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: color 0.2s ease;
}

.close-button:hover {
  color: #4a9eff;
}

.modal-body {
  padding: 16px;
  min-height: 100px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.empty-message {
  margin: 0;
  font-size: 12px;
  color: #999;
}

.modal-footer {
  padding: 12px 16px;
  border-top: 1px solid rgba(74, 158, 255, 0.2);
  display: flex;
  justify-content: flex-end;
  gap: 8px;
}

.btn-close {
  padding: 6px 16px;
  background: linear-gradient(135deg, #4a9eff 0%, #357abd 100%);
  border: none;
  border-radius: 4px;
  color: #fff;
  font-size: 11px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s ease;
}

.btn-close:hover {
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(74, 158, 255, 0.3);
}
</style>
