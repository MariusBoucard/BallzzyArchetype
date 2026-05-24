import { ref, reactive, onMounted, onUnmounted } from 'vue';
// Adjust this path if your folder structure is slightly different
import * as Juce from "@/assets/juce/index.js";

export interface JuceInitData {
  vendor: string;
  pluginName: string;
  pluginVersion: string;
}

export interface SliderState {
  value: number;
  normalised: number;
  numSteps: number;
}

export function useJuce() {
  const isInitialized = ref(false);
  const backendAvailable = ref(typeof window.__JUCE__ !== 'undefined');
  const initData = ref<JuceInitData | null>(null);

  // Reactive storage for UI binding
  const sliders = reactive(new Map<string, SliderState>());
  const toggles = reactive(new Map<string, boolean>());

  /**
   * Initialize metadata from the C++ backend
   */
  const initialize = () => {
    if (!backendAvailable.value) {
      console.warn('JUCE backend not found - Demo Mode active');
      initData.value = {
        vendor: 'Demo Vendor',
        pluginName: 'Vue Plugin Demo',
        pluginVersion: '1.0.0',
      };
      return;
    }

    const data = window.__JUCE__?.initialisationData;
    initData.value = {
      vendor: data?.vendor ?? 'Unknown Vendor',
      pluginName: data?.pluginName ?? 'Unknown Plugin',
      pluginVersion: data?.pluginVersion ?? '0.0.0',
    };
    isInitialized.value = true;
  };

  /**
   * Helper for Sliders (mimicking Juce.getSliderState)
   */
  const getSlider = (paramId: string) => {
    // 1. Handle Demo Mode
    if (!backendAvailable.value) {
      if (!sliders.has(paramId)) {
        sliders.set(paramId, { value: 0.5, normalised: 0.5, numSteps: 100 });
      }
      return {
        state: sliders.get(paramId)!,
        setNormalisedValue: (v: number) => {
          console.log("paramId",paramId,
            "set to",v
          );
          const s = sliders.get(paramId);
          if (s) s.normalised = s.value = v;
        }
      };
    }

    // 2. Handle Live Backend
    const sliderState = Juce.getSliderState(paramId);

    if (!sliders.has(paramId)) {
      // Sync initial state
      sliders.set(paramId, {
        value: sliderState.getNormalisedValue(),
        normalised: sliderState.getNormalisedValue(),
        numSteps: sliderState.properties.numSteps || 100,
      });

      // Listen for changes from C++ (DAW automation or physical slider move)
      sliderState.valueChangedEvent.addListener(() => {
        const val = sliderState.getNormalisedValue();
        const s = sliders.get(paramId);
        if (s) {
          s.normalised = val;
          s.value = val;
        }
      });
    }

    return {
      state: sliders.get(paramId)!,
      setNormalisedValue: (v: number) => sliderState.setNormalisedValue(v),
      properties: sliderState.properties
    };
  };

  /**
   * Helper for Toggles (mimicking Juce.getToggleState)
   */
  const getToggle = (paramId: string) => {
    if (!backendAvailable.value) {
      if (!toggles.has(paramId)) toggles.set(paramId, false);
      return {
        isActive: () => toggles.get(paramId),
        toggle: () => toggles.set(paramId, !toggles.get(paramId))
      };
    }

    const toggleState = Juce.getToggleState(paramId);

    if (!toggles.has(paramId)) {
      toggles.set(paramId, toggleState.getValue());
      
      toggleState.valueChangedEvent.addListener(() => {
        toggles.set(paramId, toggleState.getValue());
      });
    }

    return {
      isActive: () => toggles.get(paramId),
      toggle: (newValue: boolean) => toggleState.setValue(newValue)
    };
  };

  /**
   * Native Function Calls (C++ withNativeFunction)
   */
  const callNativeFunction = async (funcName: string, ...args: any[]) => {
    if (!backendAvailable.value) return null;
    try {
      const func = Juce.getNativeFunction(funcName);
      return await func(...args);
    } catch (e) {
      console.error(`Native call ${funcName} failed:`, e);
    }
  };

  /**
   * Event Handling (window.__JUCE__.backend)
   */
  const onBackendEvent = (name: string, cb: (data: any) => void) => {
    if (backendAvailable.value) {
      window.__JUCE__?.backend.addEventListener(name, cb);
      return () => window.__JUCE__?.backend.removeEventListener(name, cb);
    }
  };

  const emitEvent = (name: string, data: any) => {
    if (backendAvailable.value) {
      window.__JUCE__?.backend.emitEvent(name, data);
    }
  };

  const getPresetList = async (): Promise<string[]> => {
  const result = await callNativeFunction('getPresetList');
  // result comes back as a juce::var array, coerce to string[]
  if (Array.isArray(result)) return result as string[];
  return [];
};

const savePreset = async (name: string): Promise<void> => {
  await callNativeFunction('savePreset', name);
};

const loadPreset = async (name: string): Promise<void> => {
  await callNativeFunction('loadPreset', name);
};

const deletePreset = async (name: string): Promise<void> => {
  await callNativeFunction('deletePreset', name);
};
  onMounted(() => initialize());

  return {
    isInitialized,
    backendAvailable,
    initData,
    getSlider,
    getToggle,
    callNativeFunction,
    onBackendEvent,
    emitEvent,

 getPresetList,
  savePreset,
  loadPreset,
  deletePreset
  };
}