# Chord Flow Octave Transpose Design

## Scope
Add two octave controls to Chord Flow:
- `global_octave`: preset-level transpose applied to all pad output.
- `pad_octave`: per-pad transpose applied on top of global.

Both controls use integer octaves in range `-3..+3`, default `0`.

## Behavior
- Final MIDI note generation becomes:
  - `input_note + (global_octave * 12) + (pad_octave * 12) + root + chord_interval`
- `global_octave` is saved inside each preset and restored on preset load.
- `pad_octave` is saved per pad in each preset and restored on preset load.
- Older preset files remain compatible:
  - missing `global_octave` defaults to `0`
  - missing per-pad `octave` defaults to `0`

## DSP/Data Model Changes
- Add `int octave` to `pad_slot_t`.
- Add `int global_octave` to `preset_t` and active instance state.
- Extend JSON loader/saver:
  - read/write top-level preset field `global_octave`
  - read/write per-pad field `octave`
- Expose params:
  - `global_octave` (current active preset-level value)
  - `pad_octave` (current pad value)
- Include octave fields in `state` JSON output for observability.

## UI/Metadata Changes
- Add edit rows in `ui.js`:
  - `Global Oct`
  - `Pad Oct`
- Add params in `module.json` root level param list/knobs:
  - `global_octave`
  - `pad_octave`
- Keep save flow unchanged except now saved snapshots include octave state.

## Testing
- Add DSP regression test for transpose stacking and persistence:
  - verifies semitone shift from `global_octave + pad_octave`
  - verifies save/load roundtrip preserves `global_octave` and `pad_octave`
- Update existing tests only where needed.

## Non-Goals
- No microtonal/semitone transpose controls.
- No separate UI screen for octave; keep within existing edit list.
