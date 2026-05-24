<script setup lang="ts">
import CompressorModule from './CompressorModule.vue'
import FuzzModule from './FuzzModule.vue'
import OverdriveModule from './OverdriveModule.vue'
import OneBandEqModule from './OneBandEqModule.vue'
</script>

<template>
  <section class="input-effects-tab">
    <div class="section-title">Input Effects</div>
    <div class="effects-grid">
      <CompressorModule />
      <FuzzModule />
      <OverdriveModule />
      <OneBandEqModule />
    </div>
  </section>
</template>

<style scoped>
.input-effects-tab {
  display: flex;
  flex-direction: column;
  gap: 16px;
  width: 100%;
}

.section-title {
  font-size: 13px;
  font-weight: 700;
  color: #d7e3ff;
  text-transform: uppercase;
  letter-spacing: 0.2em;
}

.effects-grid {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  gap: 16px;
  width: 100%;
}
</style>
