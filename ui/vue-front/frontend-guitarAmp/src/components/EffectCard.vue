<script lang="ts">
import { defineComponent, type PropType } from 'vue'

export default defineComponent({
  name: 'EffectCard',
  props: {
    title: {
      type: String,
      required: true,
    },
    bypass: {
      type: Object as PropType<{ isActive: () => boolean; toggle: (value: boolean) => void }> ,
      required: false,
    },
  },
  computed: {
    isActive(): boolean {
      return this.bypass?.isActive?.() ?? false
    },
  },
  methods: {
    toggleBypass(): void {
      if (this.bypass) {
        this.bypass.toggle(!this.isActive)
      }
    },
  },
})
</script>

<template>
  <div class="effect-card">
    <div class="effect-card-header">
      <span class="effect-card-title">{{ title }}</span>
      <button
        class="effect-card-bypass"
        :class="{ active: isActive }"
        @click="toggleBypass"
      >
        {{ isActive ? 'On' : 'Bypass' }}
      </button>
    </div>
    <div class="effect-card-body">
      <slot />
    </div>
  </div>
</template>

<style scoped>
.effect-card {
  background: linear-gradient(180deg, #23272f 0%, #181b21 100%);
  border: 1px solid rgba(74, 158, 255, 0.15);
  border-radius: 12px;
  box-shadow: inset 0 1px 1px rgba(255,255,255,0.03), 0 12px 30px rgba(0,0,0,0.25);
  display: flex;
  flex-direction: column;
  gap: 14px;
  padding: 16px;
}

.effect-card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 12px;
}

.effect-card-title {
  font-size: 12px;
  font-weight: 700;
  color: #d7e3ff;
  text-transform: uppercase;
  letter-spacing: 0.08em;
}

.effect-card-bypass {
  border: none;
  padding: 8px 12px;
  border-radius: 999px;
  font-size: 10px;
  font-weight: 700;
  letter-spacing: 0.08em;
  color: #fff;
  background: rgba(255,255,255,0.06);
  cursor: pointer;
  transition: background 0.2s ease, transform 0.2s ease;
}

.effect-card-bypass:hover {
  transform: translateY(-1px);
  background: rgba(255,255,255,0.12);
}

.effect-card-bypass.active {
  background: linear-gradient(90deg, #4a9eff, #00d4ff);
  color: #08121f;
}

.effect-card-body {
  display: grid;
  gap: 12px;
}
</style>
