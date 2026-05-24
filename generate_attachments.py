#!/usr/bin/env python3
"""
Generates a WebSliderAttachments C++ class from a JUCE ParameterID header file.

Toggle parameters (ENABLED, BYPASS) -> WebToggleButtonRelay / WebToggleButtonParameterAttachment
All others                           -> WebSliderRelay       / WebSliderParameterAttachment

Usage:
    python generate_attachments.py <input_header.h> [output_file.h]
    (output defaults to WebSliderAttachments.h)
"""

import re
import sys
from pathlib import Path

# ── helpers ──────────────────────────────────────────────────────────────────

TOGGLE_SUFFIXES = ("_ENABLED", "BYPASS")   # any ID ending with one of these is a toggle

def is_toggle(param_id: str) -> bool:
    return any(param_id.endswith(s) for s in TOGGLE_SUFFIXES)

def relay_type(param_id: str) -> str:
    return "juce::WebToggleButtonRelay" if is_toggle(param_id) else "juce::WebSliderRelay"

def attachment_type(param_id: str) -> str:
    return (
        "juce::WebToggleButtonParameterAttachment"
        if is_toggle(param_id)
        else "juce::WebSliderParameterAttachment"
    )

def relay_var(param_id: str) -> str:
    """BYPASS -> webBypassRelay   |   INPUT_GAIN -> webInputGainRelay"""
    words = param_id.lower().split("_")
    camel = words[0] + "".join(w.capitalize() for w in words[1:])
    return f"web{camel[0].upper()}{camel[1:]}Relay"

def attachment_var(param_id: str) -> str:
    base = relay_var(param_id)          # e.g. webInputGainRelay
    base = base[:-5]                    # strip 'Relay'
    suffix = "ToggleAttachment" if is_toggle(param_id) else "SliderAttachment"
    return f"{base}{suffix}"

def parse_parameter_ids(header_text: str) -> list[str]:
    """Return list of param IDs in declaration order, skipping duplicates."""
    # Match:  const juce::ParameterID SOME_ID { "SOME_ID", 1 };
    # We just grab the first quoted string right after the opening brace.
    pattern = re.compile(
        r'const\s+juce::ParameterID\s+(\w+)\s*\{\s*"([^"]+)"',
    )
    seen = set()
    result = []
    for match in pattern.finditer(header_text):
        var_name, string_id = match.group(1), match.group(2)
        if string_id not in seen:
            seen.add(string_id)
            result.append(string_id)
        else:
            print(f"  [warn] duplicate string ID '{string_id}' (var {var_name}) – skipped")
    return result

# ── code generation ───────────────────────────────────────────────────────────

def section_comment(width=80):
    return "    " + "/" * (width - 4)

def group_label(param_id: str) -> str | None:
    """Return a group name when the ID signals a new section."""
    prefixes = [
        ("BYPASS",                       "Global Parameters"),
        ("INPUT_GAIN",                   "Global Parameters"),
        ("OUTPUT_GAIN",                  "Global Parameters"),
        ("GATE_",                        "Gate"),
        ("PEDAL_INPUT_EQ_",              "Input EQ Pedal"),
        ("PEDAL_INPUT_OVERDRIVE_",       "Overdrive Pedal"),
        ("PEDAL_INPUT_FUZZ_",            "Fuzz Pedal"),
        ("PEDAL_INPUT_COMPRESSOR_",      "Compressor Pedal"),
        ("AMP_POST_EQ_",                 "Amp Post EQ"),
        ("AMP_",                         "Amp"),
        ("PEDAL_OUTPUT_DELAY_",          "Output Delay Pedal"),
        ("PEDAL_OUTPUT_REVERB_",         "Output Reverb Pedal"),
    ]
    for prefix, label in prefixes:
        if param_id.startswith(prefix):
            return label
    return None

def generate(param_ids: list[str], class_name: str = "WebSliderAttachments",
             processor_type: str = "SkeletonAudioProcessor") -> str:

    # ── constructor initialiser list ─────────────────────────────────────────
    init_items = [f"processorRef(p)"]
    for pid in param_ids:
        init_items.append(f'{relay_var(pid)}{{id::{pid}.getParamID()}}')

    first   = init_items[0]
    rest    = init_items[1:]
    indent  = " " * 56          # align with first relay
    init_block = f"        {first}"
    for item in rest:
        init_block += f",\n{indent}{item}"

    # ── createAttachments body ────────────────────────────────────────────────
    attach_lines = []
    for pid in param_ids:
        at = attachment_type(pid)
        av = attachment_var(pid)
        rv = relay_var(pid)
        attach_lines.append(
            f'        if (auto *param = state.getParameter(id::{pid}.getParamID()))\n'
            f'            {av} = std::make_unique<{at}>(\n'
            f'                *param, {rv}, nullptr);\n'
        )

    # ── addSlidersOptions body ────────────────────────────────────────────────
    opts_lines = []
    prev_group = None
    for pid in param_ids:
        grp = group_label(pid)
        if grp and grp != prev_group:
            opts_lines.append(f'\n                // {grp}')
            prev_group = grp
        opts_lines.append(f'                .withOptionsFrom({relay_var(pid)})')

    # ── private member declarations ───────────────────────────────────────────
    relay_decls      = []
    attachment_decls = []
    prev_group = None
    for pid in param_ids:
        grp = group_label(pid)
        if grp and grp != prev_group:
            sep = f'\n    // {"=" * 74}\n    // {grp}\n    // {"=" * 74}\n'
            relay_decls.append(sep)
            attachment_decls.append(sep)
            prev_group = grp
        relay_decls.append(
            f'    {relay_type(pid)} {relay_var(pid)};'
        )
        attachment_decls.append(
            f'    std::unique_ptr<{attachment_type(pid)}> {attachment_var(pid)};'
        )

    # ── assemble ──────────────────────────────────────────────────────────────
    lines = []
    lines.append("#pragma once\n")
    lines.append('#include <juce_gui_extra/juce_gui_extra.h>')
    lines.append('#include "ParameterIDs.h"\n')

    lines.append(f"class {class_name} {{")
    lines.append("public:")

    # constructor
    lines.append(f"    {class_name}({processor_type} &p)")
    lines.append(f"        : {init_block}")
    lines.append("    {}")

    lines.append("")

    # createAttachments
    lines.append("    void createAttachments() {")
    lines.append("        auto &state = processorRef.getState();\n")
    lines.append("\n".join(attach_lines))
    lines.append("    }\n")

    # addSlidersOptions
    lines.append("    juce::WebBrowserComponent::Options addSlidersOptions(juce::WebBrowserComponent::Options options) {")
    lines.append("        return options")
    lines.append("\n".join(opts_lines))
    lines.append("               ;")
    lines.append("    }\n")

    # private section
    lines.append("private:")
    lines.append(f"    {processor_type} &processorRef;\n")
    lines.append("\n".join(relay_decls))
    lines.append("")
    lines.append("\n".join(attachment_decls))
    lines.append("};")

    return "\n".join(lines)

# ── entry point ───────────────────────────────────────────────────────────────

def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_attachments.py <input.h> [output.h]")
        sys.exit(1)

    input_path  = Path(sys.argv[1])
    output_path = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("WebSliderAttachments.h")

    header_text = input_path.read_text(encoding="utf-8")
    param_ids   = parse_parameter_ids(header_text)

    print(f"Parsed {len(param_ids)} unique parameter IDs:")
    for pid in param_ids:
        kind = "toggle" if is_toggle(pid) else "slider"
        print(f"  [{kind:6}] {pid}")

    code = generate(param_ids)
    output_path.write_text(code, encoding="utf-8")
    print(f"\nGenerated → {output_path}")

if __name__ == "__main__":
    main()
