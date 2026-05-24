import("stdfaust.lib");

// ==========================================
// 1. UI PARAMETERS
// ==========================================
// Room Size: Controls the feedback level (0 = tiny closet, 1 = massive hall)
size_ctrl  = hslider("[1] Room Size", 0.5, 0.0, 1.0, 0.01);

// Damping: Absorbs high frequencies over time (0 = bright/reflective, 1 = dark/carpeted)
damp_ctrl  = hslider("[2] Damping", 0.3, 0.0, 1.0, 0.01);

// Mix: Dry/Wet balance (0 = completely dry, 100 = completely wet)
mix_ctrl   = hslider("[3] Mix (Dry/Wet)", 30, 0, 100, 0.1);

// Level: Master output volume
level_ctrl = hslider("[4] Level [unit:dB]", 0, -24, 12, 0.1) : ba.db2linear;


// ==========================================
// 2. DSP ARCHITECTURE
// ==========================================
// re.stereo_freeverb expects exactly 4 parameters:
// stereo_freeverb(combfeedback, allpassfeedback, damping, spatialspread)
// It natively accepts 2 inputs and provides 2 outputs.
reverb_engine = re.stereo_freeverb(size_ctrl, 0.5, damp_ctrl, 23);


// ==========================================
// 3. MAIN PROCESS (Mono In -> Stereo Out)
// ==========================================
wet_amount = mix_ctrl / 100.0;
dry_amount = 1.0 - wet_amount;

// We split the mono input 'x' into a stereo pair (x, x), pass it through
// the reverb, and cleanly mix the dry and wet signals at the end.
process(x) = x, x : reverb_engine : mix_stereo
with {
    mix_stereo(wet_L, wet_R) =
        ((x * dry_amount) + (wet_L * wet_amount)) * level_ctrl,
        ((x * dry_amount) + (wet_R * wet_amount)) * level_ctrl;
};