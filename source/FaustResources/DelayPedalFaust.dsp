import("stdfaust.lib");

// Sans piongpong !
// ==========================================
// Constants
// ==========================================
MAXDELAYSAMPLES = int(ma.SR * 4);

// ==========================================
// Parameters
// ==========================================
g(x)  = hgroup("Stereo Delay", x);
dTime = g(hslider("Time [unit:s]", 0.3, 0.001, 4, 0.001));
fb    = g(hslider("Feedback",      0.3, 0,     0.95, 0.01));
mix   = g(hslider("Mix",           0.5, 0,     1,    0.01)) : si.smoo;

dSamples = int(dTime * ma.SR);

// ==========================================
// Delay Core
// ==========================================
// de.sdelay: smoothly crossfades between delay taps on time change — no clicks.
// ~ *(fb): feeds the delayed output back into the input summing node.
fbDelay = (+ : de.sdelay(MAXDELAYSAMPLES, 1024, dSamples)) ~ *(fb);

// One channel: split input into dry and wet paths, sum them
channel = _ <: *(1 - mix), (fbDelay : *(mix)) :> _;

// ==========================================
// Stereo: two independent delay instances
// ==========================================
process = channel, channel;