import("stdfaust.lib");

// ==========================================
// 1. UI PARAMETERS
// ==========================================
// Drive: 1 to 100. Mapped exponentially to create massive gain.
drive_ctrl = hslider("[1] Drive", 50, 1, 100, 0.1);

// Tone: 0 to 100. Blends between dark mud and biting fizz.
tone_ctrl  = hslider("[2] Tone", 50, 0, 100, 0.1);

// Level: Output volume from -60dB to +12dB.
level_ctrl = hslider("[3] Level [unit:dB]", -12, -60, 12, 0.1) : ba.db2linear;

// NEW: Dry/Wet Mix (0 = totally clean, 100 = totally destroyed)
mix_ctrl   = hslider("[4] Mix (Dry/Wet)", 100, 0, 100, 0.1);


// ==========================================
// 2. DSP ARCHITECTURE
// ==========================================

// --- The Drive Stage ---
pregain = pow(10, drive_ctrl / 30.0);

// --- The "Nasty" Clipper ---
bias = 0.03;
clipper(x) = max(-0.9, min(0.6, (x + bias) * pregain));

// --- The Tone Stage ---
lp = fi.lowpass(1, 300);
hp = fi.highpass(1, 1500);
tone_mix = tone_ctrl / 100.0; // Renamed to avoid conflicts
tone_stage(x) = (x : lp) * (1 - tone_mix) + (x : hp) * tone_mix;

// --- The Fuzz Block ---
// We bundle the entire distortion process into one tidy function.
fuzz_circuit = clipper : fi.dcblocker : tone_stage;


// ==========================================
// 3. MAIN PROCESS
// ==========================================
// Convert the 0-100 slider into a 0.0 to 1.0 multiplier
wet_amount = mix_ctrl / 100.0;
dry_amount = 1.0 - wet_amount;

// We explicitly declare the incoming signal as 'x' so we can branch it.
// The left side of the plus handles the dry signal, the right handles the fuzz.
// After they are summed together, we apply the master level control.
process(x) = (x * dry_amount) + (fuzz_circuit(x) * wet_amount) : *(level_ctrl);