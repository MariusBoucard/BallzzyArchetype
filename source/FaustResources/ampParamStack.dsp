import("stdfaust.lib");

// ==========================================
// 1. UI DEFINITIONS (4 Parameters Only)
// ==========================================
// Grouping them together in a horizontal layout
eq_group(x) = hgroup("Simple Proportional-Q EQ", x);

ls_gain   = eq_group(vslider("1. Low [unit:dB]", 0, -15, 15, 0.1));
mid_gain  = eq_group(vslider("2. Mid [unit:dB]", 0, -15, 15, 0.1));
pres_gain = eq_group(vslider("3. Presence [unit:dB]", 0, -15, 15, 0.1));
hs_gain   = eq_group(vslider("4. High [unit:dB]", 0, -15, 15, 0.1));


// ==========================================
// 2. FIXED FREQUENCIES & PROPORTIONAL Q MATH
// ==========================================
// Hardcoded frequencies so they don't clutter the UI
ls_freq   = 150;
mid_freq  = 1000;
pres_freq = 3500;
hs_freq   = 8000;

// Proportional Q formula:
// Starts at a wide Q of 0.7. As the gain moves away from 0, the Q increases.
// At 15dB of boost/cut, the Q becomes 0.7 + (15 * 0.2) = 3.7 (a much thinner band).
calc_q(gain) = 0.7 + (abs(gain) * 0.2);

mid_q  = calc_q(mid_gain);
pres_q = calc_q(pres_gain);


// ==========================================
// 3. DSP PROCESSING
// ==========================================
// The audio pipeline with the fixed frequencies and dynamic Q values
eq_channel = fi.low_shelf(ls_gain, ls_freq)
           : fi.peak_eq_cq(mid_gain, mid_freq, mid_q)
           : fi.peak_eq_cq(pres_gain, pres_freq, pres_q)
           : fi.high_shelf(hs_gain, hs_freq);

// Process the channel in parallel (stereo)
process = par(i, 2, eq_channel);