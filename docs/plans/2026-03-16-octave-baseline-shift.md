# Octave Baseline Shift Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Make `global_octave` default to `2`, expand both octave ranges to `-6..6`, and place `Global Oct` first in controls.

**Architecture:** Update DSP defaults/range clamps and backward-compatible preset loading defaults, then align UI edit ordering and module metadata ordering. Validate with failing-first tests for default value, range behavior, and UI ordering.

**Tech Stack:** C (midi_fx_api_v1), JavaScript UI, module.json metadata, shell/C tests.

---

### Task 1: Add failing tests for new default/range/ordering

**Files:**
- Modify: `tests/test_chordflow_octave_transpose.c`
- Modify: `tests/chordflow_ui_behavior_test.sh`

**Step 1: Write failing test**
- Assert fresh instance `global_octave == 2`.
- Assert octave params clamp to `-6..6`.
- Assert UI edit order starts with `global_octave`.

**Step 2: Run test to verify it fails**
Run: `./tests/test_chordflow_octave_transpose.sh`
Expected: FAIL before implementation.

### Task 2: Implement DSP default/range changes

**Files:**
- Modify: `src/dsp/chord_flow_plugin.c`

**Step 1: Minimal implementation**
- Change octave clamp ranges from `-3..3` to `-6..6` for global + pad.
- Change missing `global_octave` preset default to `2`.
- Ensure fallback default preset global octave is `2`.

**Step 2: Run test**
Run: `./tests/test_chordflow_octave_transpose.sh`
Expected: PASS.

### Task 3: Implement UI/module ordering and ranges

**Files:**
- Modify: `src/ui.js`
- Modify: `src/module.json`
- Modify: `README.md`
- Modify: `src/help.json`

**Step 1: Minimal implementation**
- Put `global_octave` first in edit rows.
- Update UI range handling for both octave params to `-6..6`.
- Set module default `global_octave` to `2` and metadata ranges to `-6..6`.

**Step 2: Run test**
Run: `./tests/chordflow_ui_behavior_test.sh`
Expected: PASS.

### Task 4: Full verification + deploy

**Files:**
- None

**Step 1: Run full suite**
Run: `./scripts/test.sh`
Expected: all pass.

**Step 2: Build + install**
Run: `./scripts/build.sh && ./scripts/install.sh`
Expected: success and deployed module.
