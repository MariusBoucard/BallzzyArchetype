<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { useJuce } from '@/composables/useJuce'

const { getPresetList, savePreset, loadPreset, deletePreset } = useJuce()

// ── State ──────────────────────────────────────────────────────────────────
const isOpen         = ref(false)
const presets        = ref<string[]>([])
const currentPreset  = ref('')
const isLoading      = ref(false)

// Create flow
const showCreateForm = ref(false)
const newPresetName  = ref('')
const createError    = ref('')

// Delete flow
const pendingDelete  = ref<string | null>(null)

// ── Helpers ────────────────────────────────────────────────────────────────
const currentIndex = computed(() => presets.value.indexOf(currentPreset.value))
const hasPresets   = computed(() => presets.value.length > 0)
const canGoPrev    = computed(() => currentIndex.value > 0)
const canGoNext    = computed(() => currentIndex.value < presets.value.length - 1)

const refresh = async () => {
  presets.value = await getPresetList()
}

const handleLoad = async (name: string) => {
  if (isLoading.value) return
  isLoading.value = true
  await loadPreset(name)
  currentPreset.value = name
  isLoading.value = false
}

const handlePrev = () => {
  if (!canGoPrev.value) return
  const name = presets.value[currentIndex.value - 1]
  if (name) handleLoad(name)
}

const handleNext = () => {
  if (!canGoNext.value) return
  const name = presets.value[currentIndex.value + 1]
  if (name) handleLoad(name)
}

// ── Create ─────────────────────────────────────────────────────────────────
const openCreate = () => {
  showCreateForm.value = true
  newPresetName.value  = ''
  createError.value    = ''
}

const cancelCreate = () => {
  showCreateForm.value = false
  newPresetName.value  = ''
  createError.value    = ''
}

const handleSave = async () => {
  const name = newPresetName.value.trim()
  if (!name) { createError.value = 'Please enter a name.'; return }
  if (presets.value.includes(name)) { createError.value = 'Name already exists.'; return }
  isLoading.value = true
  await savePreset(name)
  await refresh()
  currentPreset.value  = name
  isLoading.value      = false
  showCreateForm.value = false
  newPresetName.value  = ''
  createError.value    = ''
}

// ── Delete ─────────────────────────────────────────────────────────────────
const askDelete    = (name: string) => { pendingDelete.value = name }
const cancelDelete = () => { pendingDelete.value = null }

const confirmDelete = async () => {
  if (!pendingDelete.value) return
  const name = pendingDelete.value
  await deletePreset(name)
  if (currentPreset.value === name) currentPreset.value = ''
  pendingDelete.value = null
  await refresh()
}

// ── Close on outside click ─────────────────────────────────────────────────
const wrapperRef = ref<HTMLElement | null>(null)
const onDocClick = (e: MouseEvent) => {
  if (wrapperRef.value && !wrapperRef.value.contains(e.target as Node)) {
    isOpen.value        = false
    pendingDelete.value = null
    cancelCreate()
  }
}

onMounted(() => {
  refresh()
  document.addEventListener('click', onDocClick)
})

watch(isOpen, (v) => {
  if (v) refresh()
  else { pendingDelete.value = null; cancelCreate() }
})
</script>

<template>
  <div class="preset-manager" ref="wrapperRef">

    <!-- ── Trigger bar ─────────────────────────────────────────── -->
    <div class="trigger-row">
      <button
        class="nav-btn"
        :disabled="!canGoPrev || isLoading"
        title="Previous preset"
        @click.stop="handlePrev"
      >‹</button>

      <button
        class="preset-button"
        :class="{ active: isOpen }"
        @click.stop="isOpen = !isOpen"
      >
        <span class="preset-icon">📋</span>
        <span class="preset-label">{{ currentPreset || 'Presets' }}</span>
        <span class="chevron" :class="{ rotated: isOpen }">▾</span>
      </button>

      <button
        class="nav-btn"
        :disabled="!canGoNext || isLoading"
        title="Next preset"
        @click.stop="handleNext"
      >›</button>
    </div>

    <!-- ── Dropdown ────────────────────────────────────────────── -->
    <Transition name="dropdown">
      <div v-if="isOpen" class="preset-dropdown" @click.stop>

        <!-- Create form -->
        <Transition name="slide">
          <div v-if="showCreateForm" class="create-form">
            <input
              v-model="newPresetName"
              class="name-input"
              placeholder="Preset name…"
              autofocus
              @keyup.enter="handleSave"
              @keyup.esc="cancelCreate"
            />
            <p v-if="createError" class="error-msg">{{ createError }}</p>
            <div class="form-actions">
              <button class="preset-button small" @click="cancelCreate">Cancel</button>
              <button class="preset-button small accent" @click="handleSave">Save</button>
            </div>
          </div>
        </Transition>

        <!-- New preset button (when form is hidden) -->
        <button v-if="!showCreateForm" class="preset-button full" @click="openCreate">
          <span class="preset-icon">＋</span>
          <span class="preset-label">Save Current Preset</span>
        </button>

        <div class="separator" v-if="hasPresets" />

        <!-- Empty state -->
        <p v-if="!hasPresets" class="empty-message">No presets available</p>

        <!-- Preset list -->
        <ul v-else class="preset-list">
          <li
            v-for="name in presets"
            :key="name"
            class="preset-item"
            :class="{ 'is-active': name === currentPreset, 'is-deleting': name === pendingDelete }"
          >
            <!-- Normal row -->
            <template v-if="pendingDelete !== name">
              <button class="item-name" @click="handleLoad(name)">{{ name }}</button>
              <button class="delete-btn" title="Delete" @click="askDelete(name)">✕</button>
            </template>

            <!-- Confirm-delete inline -->
            <template v-else>
              <span class="confirm-msg">Delete "{{ name }}"?</span>
              <div class="confirm-actions">
                <button class="preset-button small" @click="cancelDelete">No</button>
                <button class="preset-button small danger" @click="confirmDelete">Yes</button>
              </div>
            </template>
          </li>
        </ul>

      </div>
    </Transition>

  </div>
</template>

<style scoped>
.preset-manager {
  position: relative;
}

/* ── Shared button — your original style ────────────────────────────────── */
.preset-button {
  display: flex;
  align-items: center;
  gap: 6px;
  padding: 6px 12px;
  background: rgba(74, 158, 255, 0.1);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 6px;
  color: #e0e0e0;
  font-size: 11px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s ease;
  backdrop-filter: blur(10px);
}

.preset-button:hover {
  background: rgba(74, 158, 255, 0.15);
  border-color: rgba(74, 158, 255, 0.5);
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(74, 158, 255, 0.2);
}

.preset-button:active { transform: translateY(0); }

.preset-button.active {
  background: rgba(74, 158, 255, 0.18);
  border-color: rgba(74, 158, 255, 0.5);
}

.preset-button.full  { width: 100%; box-sizing: border-box; }
.preset-button.small { padding: 4px 10px; font-size: 10px; }

.preset-button.accent {
  background: rgba(74, 158, 255, 0.2);
  border-color: rgba(74, 158, 255, 0.5);
  color: #7ac0ff;
}

.preset-button.danger {
  background: rgba(255, 80, 80, 0.1);
  border-color: rgba(255, 80, 80, 0.35);
  color: #ff8080;
}

.preset-button.danger:hover {
  background: rgba(255, 80, 80, 0.2);
  border-color: rgba(255, 80, 80, 0.6);
  box-shadow: 0 4px 12px rgba(255, 80, 80, 0.2);
}

.preset-button:disabled {
  opacity: 0.35;
  cursor: not-allowed;
  transform: none;
  box-shadow: none;
}

/* ── Trigger row ────────────────────────────────────────────────────────── */
.trigger-row {
  display: flex;
  align-items: center;
  gap: 4px;
}

/* Nav arrows reuse the same button style, just narrower */
.nav-btn {
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 6px 8px;
  background: rgba(74, 158, 255, 0.1);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 6px;
  color: #e0e0e0;
  font-size: 14px;
  line-height: 1;
  cursor: pointer;
  transition: all 0.2s ease;
  backdrop-filter: blur(10px);
}

.nav-btn:hover:not(:disabled) {
  background: rgba(74, 158, 255, 0.15);
  border-color: rgba(74, 158, 255, 0.5);
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(74, 158, 255, 0.2);
}

.nav-btn:active:not(:disabled) { transform: translateY(0); }

.nav-btn:disabled { opacity: 0.3; cursor: not-allowed; }

/* ── Label / chevron ────────────────────────────────────────────────────── */
.preset-icon { font-size: 12px; }

.preset-label {
  white-space: nowrap;
  max-width: 110px;
  overflow: hidden;
  text-overflow: ellipsis;
}

.chevron {
  font-size: 10px;
  color: rgba(224, 224, 224, 0.5);
  transition: transform 0.2s ease;
  margin-left: 2px;
}

.chevron.rotated { transform: rotate(180deg); }

/* ── Dropdown — your original style ─────────────────────────────────────── */
.preset-dropdown {
  position: absolute;
  top: calc(100% + 6px);
  left: 0;
  min-width: 190px;
  background: linear-gradient(135deg, #2a2d3a 0%, #1f2129 100%);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 6px;
  padding: 8px;
  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.3);
  backdrop-filter: blur(10px);
  z-index: 100;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

/* ── Create form ────────────────────────────────────────────────────────── */
.create-form {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.name-input {
  width: 100%;
  box-sizing: border-box;
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(74, 158, 255, 0.3);
  border-radius: 6px;
  color: #e0e0e0;
  font-size: 11px;
  padding: 5px 9px;
  outline: none;
  transition: border-color 0.2s ease;
}

.name-input:focus { border-color: rgba(74, 158, 255, 0.7); }

.error-msg {
  margin: 0;
  font-size: 10px;
  color: #ff8080;
}

.form-actions {
  display: flex;
  gap: 6px;
  justify-content: flex-end;
}

/* ── Separator ──────────────────────────────────────────────────────────── */
.separator {
  height: 1px;
  background: rgba(74, 158, 255, 0.15);
  margin: 0 -2px;
}

/* ── Preset list ────────────────────────────────────────────────────────── */
.empty-message {
  margin: 0;
  padding: 4px 8px;
  font-size: 11px;
  color: #999;
  text-align: center;
}

.preset-list {
  list-style: none;
  margin: 0;
  padding: 0;
  display: flex;
  flex-direction: column;
  gap: 2px;
  max-height: 200px;
  overflow-y: auto;
  scrollbar-width: thin;
  scrollbar-color: rgba(74, 158, 255, 0.2) transparent;
}

.preset-item {
  display: flex;
  align-items: center;
  gap: 4px;
  border-radius: 5px;
  padding: 2px 4px;
  min-height: 28px;
  transition: background 0.15s;
}

.preset-item:hover     { background: rgba(74, 158, 255, 0.07); }
.preset-item.is-active { background: rgba(74, 158, 255, 0.12); }
.preset-item.is-deleting { background: rgba(255, 80, 80, 0.07); }

.item-name {
  flex: 1;
  background: none;
  border: none;
  color: #e0e0e0;
  font-size: 11px;
  font-weight: 600;
  text-align: left;
  cursor: pointer;
  padding: 4px;
  border-radius: 4px;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  transition: color 0.15s;
}

.preset-item.is-active .item-name { color: #7ac0ff; }

.delete-btn {
  flex-shrink: 0;
  background: none;
  border: none;
  color: #666;
  font-size: 10px;
  cursor: pointer;
  padding: 3px 5px;
  border-radius: 4px;
  opacity: 0;
  transition: all 0.15s;
}

.preset-item:hover .delete-btn { opacity: 1; }

.delete-btn:hover {
  color: #ff8080;
  background: rgba(255, 80, 80, 0.1);
}

/* ── Inline delete confirm ──────────────────────────────────────────────── */
.confirm-msg {
  flex: 1;
  font-size: 10px;
  font-weight: 600;
  color: #ff8080;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.confirm-actions { display: flex; gap: 4px; }

/* ── Transitions ────────────────────────────────────────────────────────── */
.dropdown-enter-active,
.dropdown-leave-active { transition: opacity 0.15s ease, transform 0.15s ease; }
.dropdown-enter-from,
.dropdown-leave-to     { opacity: 0; transform: translateY(-4px); }

.slide-enter-active,
.slide-leave-active {
  transition: opacity 0.15s ease, max-height 0.2s ease;
  overflow: hidden;
  max-height: 120px;
}
.slide-enter-from,
.slide-leave-to { opacity: 0; max-height: 0; }
</style>