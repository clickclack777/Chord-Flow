# Octave Transpose Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add preset-level and per-pad octave transpose controls (`global_octave`, `pad_octave`) with save/load support.

**Architecture:** Extend DSP data structures and preset serialization for octave fields, expose params via API, and wire UI/module metadata to edit/display those params. Validate via failing-first DSP regression tests and full build/test verification.

**Tech Stack:** C (midi_fx_api_v1), JavaScript UI, shell test scripts, module.json metadata.

---

### Task 1: Add failing DSP regression tests for octave behavior

**Files:**
- Modify: `tests/test_chordflow_save_behavior.c`
- Create: `tests/test_chordflow_octave_transpose.c`
- Create: `tests/test_chordflow_octave_transpose.sh`
- Modify: `scripts/test.sh`

**Step 1: Write failing test**
- Add checks for:
  - `global_octave` transposes output by octaves.
  - `pad_octave` stacks with `global_octave`.
  - save/load preserves `global_octave` and `pad_octave`.

**Step 2: Run test to verify it fails**
Run: `./tests/test_chordflow_octave_transpose.sh`
Expected: FAIL because params/serialization are not implemented yet.

### Task 2: Implement DSP octave params and persistence

**Files:**
- Modify: `src/dsp/chord_flow_plugin.c`

**Step 1: Minimal implementation**
- Add octave fields to structs.
- Add param handling/getters for `global_octave` and `pad_octave`.
- Apply octave offsets in chord building.
- Read/write octave fields in preset JSON parser and saver.
- Add backward-compatible defaults (`0`) for missing fields.

**Step 2: Run tests to verify pass**
Run: `./tests/test_chordflow_octave_transpose.sh`
Expected: PASS.

### Task 3: Wire UI + module metadata

**Files:**
- Modify: `src/ui.js`
- Modify: `src/module.json`
- Modify: `tests/chordflow_ui_behavior_test.sh`

**Step 1: Write/adjust failing UI metadata checks**
- Add checks for `global_octave` + `pad_octave` rows/params.

**Step 2: Implement minimal UI/metadata changes**
- Add edit rows and numeric handling for octave params.
- Add module params and knob entries.

**Step 3: Run checks**
Run: `./tests/chordflow_ui_behavior_test.sh`
Expected: PASS.

### Task 4: Full verification

**Files:**
- Modify: `README.md` (if needed for docs sync)

**Step 1: Run full test suite**
Run: `./scripts/test.sh`
Expected: All tests pass.

**Step 2: Run module build**
Run: `./scripts/build.sh`
Expected: Build succeeds and `dist/chord-flow/` artifacts are produced.

**Step 3: Install to Move**
Run: `./scripts/install.sh`
Expected: Module copies to `/data/UserData/move-anything/modules/midi_fx/chord-flow/`.
