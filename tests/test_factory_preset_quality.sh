#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

ROOT_DIR="$ROOT_DIR" ruby <<'RUBY'
require 'json'

root = ENV.fetch('ROOT_DIR')
preset_path = File.join(root, 'src/presets/default.json')
dsp_path = File.join(root, 'src/dsp/chord_flow_plugin.c')

presets = JSON.parse(File.read(preset_path))
dsp = File.read(dsp_path)

type_block = dsp[/static const char \*TYPE_NAMES\[\] = \{(.*?)\};/m, 1]
abort('FAIL: could not parse TYPE_NAMES from chord_flow_plugin.c') unless type_block
allowed_types = type_block.scan(/"([^"]+)"/).flatten.to_h { |t| [t, true] }

errors = []

presets.each do |preset|
  name = preset['name'] || '(unnamed)'
  pads = preset['pads'] || []

  if pads.length != 16
    errors << "#{name}: expected 16 pads, got #{pads.length}"
  end

  seen = {}
  pads.each_with_index do |pad, i|
    type = pad['chord_type'].to_s
    unless allowed_types[type]
      errors << "#{name} pad #{i + 1}: unsupported chord_type '#{type}'"
    end

    sig = [pad['root'], type, pad['inversion'], pad['bass'], pad['octave']].join('|')
    if seen.key?(sig)
      errors << "#{name}: duplicate pad recipe at #{seen[sig]} and #{i + 1} (#{sig})"
    else
      seen[sig] = i + 1
    end
  end
end

if errors.any?
  puts "FAIL: factory preset quality checks failed"
  errors.each { |e| puts " - #{e}" }
  exit 1
end

puts "PASS: factory preset quality"
RUBY
