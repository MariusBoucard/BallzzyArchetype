declare name "Standard Library Compressor";
declare version "2.0";

import("stdfaust.lib");

// --- User Interface Controls ---
thresh = hslider("Threshold [unit:dB]", -20, -60, 0, 0.1);
ratio  = hslider("Ratio", 4, 1, 20, 0.1);
att    = hslider("Attack [unit:ms]", 10, 0.1, 100, 0.1) : *(0.001); // The library expects seconds
rel    = hslider("Release [unit:ms]", 100, 1, 1000, 1) : *(0.001); // The library expects seconds
makeup = hslider("Makeup Gain [unit:dB]", 0, 0, 24, 0.1) : ba.db2linear;

// --- Stereo Process ---
// 1. Pass the parameters into the standard stereo compressor.
// 2. Multiply both the Left and Right output channels by the linear makeup gain.
process = co.compressor_stereo(ratio, thresh, att, rel) : *(makeup), *(makeup);