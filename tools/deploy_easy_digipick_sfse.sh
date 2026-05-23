#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
plugin_dll="$repo_root/Data/SFSE/Plugins/EasyDigipickSFSE.dll"
plugin_ini="$repo_root/sfse-digipick-plugin/config/EasyDigipickSFSE.ini"
live_plugins_dir="$HOME/.local/share/Steam/steamapps/common/Starfield/Data/SFSE/Plugins"

if [[ ! -f "$plugin_dll" ]]; then
  echo "Missing DLL: $plugin_dll"
  echo "Build first from sfse-digipick-plugin/."
  exit 1
fi

mkdir -p "$live_plugins_dir"

cp -f -- "$plugin_dll" "$live_plugins_dir/EasyDigipickSFSE.dll"
cp -f -- "$plugin_ini" "$live_plugins_dir/EasyDigipickSFSE.ini"

echo "Deployed: $live_plugins_dir/EasyDigipickSFSE.dll"
echo "Deployed: $live_plugins_dir/EasyDigipickSFSE.ini"
