import("stdfaust.lib");

// ==========================================
// 1. UI DEFINITIONS
// ==========================================
delay_group(x) = hgroup("Stereo Delay Effect", x);

del_time  = delay_group(vslider("Delay Time [unit:ms]", 300, 0, 2000, 1));
del_fb    = delay_group(vslider("Feedback", 0.4, 0, 0.99, 0.01));
del_mix   = delay_group(vslider("Mix", 0.5, 0, 1, 0.01));
pingpong  = delay_group(checkbox("Ping Pong"));

// ==========================================
// 2. DELAY UTILITIES
// ==========================================
delay_samples = del_time * 0.001 * ma.SR;
max_samples   = 2 * ma.SR;

// ==========================================
// 3. CORE DELAY LOOP (with Ping-Pong Routing)
// ==========================================
// Receives 4 inputs: (xL, xR, fL, fR)
// Outputs 2 channels: (wetL, wetR)
delay_step(xL, xR, fL, fR) = wetL, wetR
with {
    // Standard 'with' local definitions work perfectly for assignments
    next_fL = select2(pingpong, fL, fR) * del_fb;
    next_fR = select2(pingpong, fR, fL) * del_fb;

    wetL = (xL + next_fL) : de.delay(max_samples, delay_samples);
    wetR = (xR + next_fR) : de.delay(max_samples, delay_samples);
};

// ==========================================
// 4. MAIN PROCESSING BLOCK (Dry/Wet Mix)
// ==========================================
// Mixes an original signal with its wet delay signal
mix_channel(dry, wet) = (dry * (1.0 - del_mix)) + (wet * del_mix);

// 1. Split inputs into 4 paths: (Left, Right, Left, Right)
// 2. Keep the first pair dry, feed the second pair into the delay loop
// 3. Re-route the 4 resulting streams into the stereo mixer
process = _, _
        : (split_dry_wet)
        : ( _, _, (delay_step ~ (_, _)) )
        : mix_routing;

// Signal routing helpers
split_dry_wet(x, y) = x, y, x, y;
mix_routing(dL, dR, wL, wR) = mix_channel(dL, wL), mix_channel(dR, wR);