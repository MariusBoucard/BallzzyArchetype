<script setup lang="ts">
import DelayModule from './DelayModule.vue'
import ReverbModule from './ReverbModule.vue'
</script>

<template>
  <section class="post-effects-tab">
    <div class="section-title">Post Effects</div>
    <div class="effects-grid">
      <DelayModule />
      <ReverbModule />
    </div>
  </section>
</template>

<style scoped>
.post-effects-tab {
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
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 16px;
}
</style>
