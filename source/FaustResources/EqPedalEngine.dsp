import("stdfaust.lib");

// ==========================================
// 1. UI PARAMETERS (Graphic EQ Layout)
// ==========================================
// We use 'vslider' (vertical sliders) inside an 'hgroup' (horizontal group)
// so the UI actually looks like a physical graphic EQ pedal.
eq_group(x) = hgroup("[1] 10-Band Proportional Q EQ", x);

// Band Gain Sliders (-15dB to +15dB)
g31  = eq_group(vslider("[01] 31 Hz [unit:dB]", 0, -15, 15, 0.1));
g62  = eq_group(vslider("[02] 62 Hz [unit:dB]", 0, -15, 15, 0.1));
g125 = eq_group(vslider("[03] 125 Hz [unit:dB]", 0, -15, 15, 0.1));
g250 = eq_group(vslider("[04] 250 Hz [unit:dB]", 0, -15, 15, 0.1));
g500 = eq_group(vslider("[05] 500 Hz [unit:dB]", 0, -15, 15, 0.1));
g1k  = eq_group(vslider("[06] 1 kHz [unit:dB]", 0, -15, 15, 0.1));
g2k  = eq_group(vslider("[07] 2 kHz [unit:dB]", 0, -15, 15, 0.1));
g4k  = eq_group(vslider("[08] 4 kHz [unit:dB]", 0, -15, 15, 0.1));
g8k  = eq_group(vslider("[09] 8 kHz [unit:dB]", 0, -15, 15, 0.1));
g16k = eq_group(vslider("[10] 16 kHz [unit:dB]", 0, -15, 15, 0.1));

// Master Level to compensate for heavy boosting/cutting
level_ctrl = hslider("[2] Master Level [unit:dB]", 0, -24, 12, 0.1) : ba.db2linear;


// ==========================================
// 2. DSP ARCHITECTURE
// ==========================================

// --- Proportional Q Math ---
// Here is the magic. We take the absolute value of the gain (so -10dB and +10dB
// are treated the same).
// If gain is 0dB, Q is 1.0 (very wide, gentle curve).
// As gain approaches +/- 15dB, Q scales up to 4.0 (very thin, sharp curve).
calc_q(gain) = 1.0 + (abs(gain) / 15.0) * 3.0;

// --- Single Band Generator ---
// We use the standard library's Constant-Q Peak EQ function.
// It requires (Gain in dB, Frequency, Q). We dynamically calculate the Q
// based on whatever the current gain slider is set to.
eq_band(freq, gain) = fi.peak_eq_cq(gain, freq, calc_q(gain));

// --- The EQ Circuit ---
// We chain all 10 bands together in series.
eq_circuit =
    eq_band(31.25, g31) :
    eq_band(62.5, g62) :
    eq_band(125, g125) :
    eq_band(250, g250) :
    eq_band(500, g500) :
    eq_band(1000, g1k) :
    eq_band(2000, g2k) :
    eq_band(4000, g4k) :
    eq_band(8000, g8k) :
    eq_band(16000, g16k);


// ==========================================
// 3. MAIN PROCESS
// ==========================================
process = eq_circuit : *(level_ctrl);