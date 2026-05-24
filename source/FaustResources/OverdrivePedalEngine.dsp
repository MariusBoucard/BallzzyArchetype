import("stdfaust.lib");

// ==========================================
// 1. UI PARAMETERS
// ==========================================
// Drive: 1 to 100. Scales to a moderate gain boost.
drive_ctrl = hslider("[1] Drive", 50, 1, 100, 0.1);

// Tone: 0 to 100. Controls a lowpass filter to tame high-end fizz.
tone_ctrl  = hslider("[2] Tone", 50, 0, 100, 0.1);

// Level: Output volume from -60dB to +12dB.
level_ctrl = hslider("[3] Level [unit:dB]", -12, -60, 12, 0.1) : ba.db2linear;

// Mix: Dry/Wet balance (0 = clean, 100 = full overdrive).
mix_ctrl   = hslider("[4] Mix (Dry/Wet)", 100, 0, 100, 0.1);


// ==========================================
// 2. DSP ARCHITECTURE
// ==========================================

// --- The Drive Stage ---
// Overdrives need less extreme gain than fuzzes.
pregain = pow(10, drive_ctrl / 50.0);

// --- The Pre-EQ (The "Secret Sauce") ---
// Classic overdrives (like the Ibanez Tube Screamer) cut bass *before* // the clipping stage. This prevents the distortion from sounding "farty" or muddy,
// tightening up the low end and creating a signature mid-frequency hump.
pre_eq = fi.highpass(1, 720);

// --- The Soft Clipper ---
// Instead of hard-clipping with max/min, we use the hyperbolic tangent function (tanh).
// 'tanh' naturally and smoothly compresses the signal as it gets louder,
// generating warm, odd-order harmonics without the harsh, sputtering artifacts.
clipper(x) = ma.tanh(x * pregain);

// --- The Tone Stage ---
// A smooth 2nd-order lowpass filter. We map the 0-100 UI slider to a
// frequency range of roughly 400Hz (dark) to 6000Hz (bright).
tone_freq = 400 + (tone_ctrl / 100.0) * 5600;
tone_stage = fi.lowpass(2, tone_freq);

// --- The Overdrive Block ---
// Bundle the pre-EQ, clipper, and tone stage together.
od_circuit = pre_eq : clipper : tone_stage;


// ==========================================
// 3. MAIN PROCESS
// ==========================================
wet_amount = mix_ctrl / 100.0;
dry_amount = 1.0 - wet_amount;

// Split the incoming signal 'x', process the wet side, sum them, and apply level.
process(x) = (x * dry_amount) + (od_circuit(x) * wet_amount) : *(level_ctrl);