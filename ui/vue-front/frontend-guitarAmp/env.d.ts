/// <reference types="vite/client" />
/// <reference types="vite/client" />

interface Window {
  __JUCE__?: { // Notice the ? - this makes it safe for browser testing
    initialisationData: {
      vendor: string;
      pluginName: string;
      pluginVersion: string;
      [key: string]: any;
    };
    backend: {
      addEventListener: (name: string, callback: (data: any) => void) => void;
      removeEventListener: (name: string, callback: (data: any) => void) => void;
      emitEvent: (name: string, data: any) => void;
    };
  };
}

declare module "*/index.js" {
  export function getSliderState(paramId: string): any;
  export function getToggleState(paramId: string): any;
  export function getComboBoxState(paramId: string): any;
  export function getNativeFunction(name: string): any;
  export function getBackendResourceAddress(name: string): string;
}