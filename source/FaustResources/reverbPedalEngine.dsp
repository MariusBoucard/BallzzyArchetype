import("stdfaust.lib");

// ==========================================
// 1. UI DEFINITIONS
// ==========================================
rev_group(x) = hgroup("Filtered Room Reverb", x);

rev_mix  = rev_group(vslider("1. Mix", 0.3, 0, 1, 0.01));
rev_dec  = rev_group(vslider("2. Decay [unit:s]", 1.5, 0.1, 10.0, 0.1));
hp_freq  = rev_group(vslider("3. High Pass [unit:Hz]", 200, 20, 2000, 1));
lp_freq  = rev_group(vslider("4. Low Pass [unit:Hz]", 5000, 1000, 20000, 1));


// ==========================================
// 2. REVERB TAIL FILTERING
// ==========================================
// This filter block applies a 2nd-order High-Pass and Low-Pass filter
// sequentially. We will feed the wet reverb signal into this.
wet_filter = fi.highpass(2, hp_freq) : fi.lowpass(2, lp_freq);


// ==========================================
// 3. REVERB ENGINE CONFIGURATION
// ==========================================
reverb_engine = re.zita_rev1_stereo(rdtime, f1, f2, t60dc, t60wm, fsmax)
with {
    rdtime = 0.03;       // Pre-delay in seconds (30ms for a room)
    f1     = 200;        // Crossover frequency between low and mid decay
    t60dc  = rev_dec;    // Low-frequency decay time (tied to UI)
    t60wm  = rev_dec;    // Mid-frequency decay time (tied to UI)
    f2     = 6000;       // High-frequency damping frequency
    fsmax  = 48000;      // Maximum internal sample rate
};


// ==========================================
// 4. MAIN PROCESS (Mono Input -> Stereo Output)
// ==========================================
// Mix helper function to blend dry and filtered wet signals
mix_channel(dry, wet) = (dry * (1.0 - rev_mix)) + (wet * rev_mix);

// 1. Split a mono input 'x' into 4 paths: dryL, dryR, and 2 paths for the reverb engine.
// 2. Pass the reverb paths through the engine.
// 3. Filter ONLY the wet channels.
// 4. Merge the dry channels and the filtered wet channels together.
process(x) = x, x, x, x
           : ( _, _, reverb_engine )
           : ( _, _, wet_filter, wet_filter )
           : mix_routing;

// Rearranges the 4 streams (dryL, dryR, wetL, wetR) into the final stereo pair
mix_routing(dL, dR, wL, wR) = mix_channel(dL, wL), mix_channel(dR, wR);