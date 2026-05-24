<script setup lang="ts">
import { ref } from 'vue';



const isOpen = ref(false)
const emit = defineEmits<{
  close: []
}>()


const open = () => { console.log('Opening about modal'); isOpen.value = true }
const close = () => { isOpen.value = false; emit('close') }

defineExpose({ open })
</script>

<template>
  <div v-show="isOpen" class="modal-overlay" @click="close">
    <div class="modal-content" @click.stop>
      <div class="modal-header">
        <h2>About Delay Vue</h2>
        <button class="close-button" @click="close">✕</button>
      </div>
      <div class="modal-body">
        <div class="info-section">
          <label>Plugin Name</label>
          <p>Delay Vue</p>
        </div>

        <div class="info-section">
          <label>Version</label>
          <p>1.0.0</p>
        </div>

        <div class="info-section">
          <label>Author</label>
          <p>Marius</p>
        </div>

        <div class="info-section">
          <label>Description</label>
          <div class="markdown">
            <h3>Ballzzy Delay</h3>
            <p>Ballzzy Delay is a cross-platform delay processor designed for creative spatial and rhythmic effects. It features 4 independent delay heads, movement-based stereo panning, integrated ducking, and global parameter control for flexible sound design.</p>
            <blockquote>(Currently unavailable on macOS… unless someone buys me one.)</blockquote>

            <h4>Delay Heads</h4>
            <p>The plugin features 4 delay heads. Each head represents its own independent delay line and includes multiple parameters.</p>

            <h5>Head Parameters</h5>
            <ul class="markdown-list">
              <li><strong>On / Off</strong>: Enables or disables the current delay head.</li>
              <li><strong>Panning</strong>: Positions the head in the stereo field (left/right). When movement is enabled, this value represents the center position of the movement.</li>
              <li><strong>Feedback</strong>: Controls the feedback amount of the current head.</li>
              <li><strong>Feedback Slave</strong>: Makes the head use the global feedback value instead of its own.</li>
              <li><strong>Gain</strong>: Output gain of the current head.</li>
              <li><strong>Time</strong>: Delay time before the signal repeats.</li>
              <li><strong>LP / HP Filters</strong>: Low-pass and high-pass filters applied to the delayed signal. Useful for removing unwanted frequencies.</li>
            </ul>

            <h5>Movement Section</h5>
            <p>Each delay head can include automatic stereo movement.</p>
            <ul class="markdown-list">
              <li><strong>On / Off</strong>: Enables or disables movement.</li>
              <li><strong>Movement Function</strong>: Selects the function used to compute the pan movement.</li>
              <li><strong>Width</strong>: Controls how much the movement affects the panning.</li>
              <li><strong>0%</strong>: No pan movement.</li>
              <li><strong>100%</strong>: Full stereo movement from hard left to hard right.</li>
            </ul>

            <h4>Preset Manager</h4>
            <p>The plugin includes a preset manager for saving and loading configurations.</p>

            <h4>Slave Parameters</h4>
            <p>Slave parameters allow the bottom section values to override individual head parameters.</p>

            <h4>Bottom Section</h4>
            <p><strong>Sync</strong>: Displays delay time using note durations according to the DAW BPM.</p>
            <h5>BPM</h5>
            <ul class="markdown-list">
              <li><strong>BPM</strong>: Sets the desired BPM when host sync is disabled.</li>
              <li><strong>Host</strong>: Enables BPM synchronization with the DAW host.</li>
            </ul>

            <h4>Global Parameters</h4>
            <p>These parameters override values for enslaved heads:</p>
            <ul class="markdown-list">
              <li>Global Width</li>
              <li>Global Feedback</li>
              <li>Global Time</li>
              <li>Mix</li>
            </ul>
            <p>Controls the balance between the dry signal and the delayed signal.</p>

            <h4>Ducking Section</h4>
            <p>Ballzzy Delay includes an integrated ducking effect. The plugin uses the input signal to compute gain reduction and applies it to the delayed signal output.</p>
            <h5>Parameters</h5>
            <ul class="markdown-list">
              <li><strong>On / Off</strong>: Enables or disables ducking.</li>
              <li><strong>Attack</strong>: Compressor attack time in milliseconds.</li>
              <li><strong>Release</strong>: Compressor release time in milliseconds.</li>
              <li><strong>Ratio</strong></li>
              <li><strong>Threshold</strong></li>
              <li><strong>Input / Output Stages</strong></li>
              <li><strong>Input Gain</strong>: Adjusts the input volume.</li>
              <li><strong>Output Gain</strong>: Adjusts the final output volume.</li>
            </ul>

            <p>Ballzzy Delay is built for experimentation, movement, and creative space processing. Try combining multiple heads, movement functions, and ducking settings to create evolving stereo delays and rhythmic textures.</p>
          </div>
        </div>
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
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.modal-content {
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
  overflow-y: auto;
  max-height: 380px;
}

.info-section {
  margin-bottom: 12px;
}

.info-section label {
  display: block;
  font-size: 11px;
  font-weight: 600;
  color: #4a9eff;
  margin-bottom: 4px;
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.info-section p {
  margin: 0;
  font-size: 12px;
  color: #e0e0e0;
  line-height: 1.4;
}

.markdown {
  color: #d9e3ff;
}

.markdown h3,
.markdown h4,
.markdown h5 {
  margin: 16px 0 8px;
  color: #84b7ff;
  line-height: 1.2;
}

.markdown h3 {
  font-size: 14px;
}

.markdown h4 {
  font-size: 13px;
}

.markdown h5 {
  font-size: 12px;
}

.markdown p {
  margin: 8px 0;
  font-size: 12px;
  line-height: 1.6;
}

.markdown blockquote {
  margin: 12px 0;
  padding-left: 12px;
  border-left: 3px solid rgba(74, 158, 255, 0.5);
  color: #a8c9ff;
  font-style: italic;
}

.markdown-list {
  margin: 8px 0 0 16px;
  padding-left: 0;
  list-style: disc inside;
}

.markdown-list li {
  margin: 6px 0;
}

.markdown strong {
  color: #c5dcff;
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
