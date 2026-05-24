/**
 * JUCE Backend Type Definitions
 * Extends the global Window interface to include JUCE properties
 */

interface ListenerList {
  addListener(callback: (value: any) => void): number
  removeListener(id: number): void
}

interface SliderState {
  getNormalisedValue(): number
  setNormalisedValue(value: number): void
  properties: {
    numSteps: number
  }
  valueChangedEvent: ListenerList
}

interface ToggleState {
  getNormalisedValue(): number
  setNormalisedValue(value: number): void
  valueChangedEvent: ListenerList
}

interface JuceBackend {
  addEventListener(eventName: string, callback: (data: any) => void): void
  removeEventListener(eventName: string, callback: (data: any) => void): void
  emitEvent(eventName: string, data: any): void
}

interface JuceInitialisationData {
  vendor: string
  pluginName: string
  pluginVersion: string
  __juce__platform: string[]
  __juce__functions: string[]
  __juce__registeredGlobalEventIds: any[]
  __juce__sliders: string[]
  __juce__toggles: string[]
  __juce__comboBoxes: string[]
}

interface JuceObject {
  backend: JuceBackend
  initialisationData: JuceInitialisationData
  postMessage: (message: any) => void
  getSliderState(parameterId: string): SliderState
  getToggleState(parameterId: string): ToggleState
  getNativeFunction(name: string): (...args: any[]) => Promise<any>
}

declare global {
  interface Window {
    __JUCE__?: JuceObject
  }
}

export {}
