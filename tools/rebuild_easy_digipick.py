#!/usr/bin/env python3
from __future__ import annotations

import re
import struct
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
PLUGIN_PATH = REPO_ROOT / "Data" / "Easy Digipick.esm"
STARFIELD_MASTER_PATH = Path.home() / ".local/share/Steam/steamapps/common/Starfield/Data/Starfield.esm"
BACKUP_PATH = PLUGIN_PATH.with_suffix(".esm.bak")

HEADER_LEN = 24
SUBREC_HEADER_LEN = 6


def read_u_security_ids_from_master(master_bytes: bytes) -> set[str]:
    pattern = re.compile(rb"uSecurity[A-Za-z0-9]+")
    return {m.group(0).decode("ascii", errors="ignore") for m in pattern.finditer(master_bytes)}


def parse_group(plugin_bytes: bytes) -> tuple[int, int]:
    group_offset = plugin_bytes.find(b"GRUP")
    if group_offset == -1:
        raise RuntimeError("GRUP section not found in plugin")

    group_size = struct.unpack_from("<I", plugin_bytes, group_offset + 4)[0]
    group_end = group_offset + group_size
    if group_end > len(plugin_bytes):
        raise RuntimeError("Invalid GRUP size in plugin")

    return group_offset, group_end


def get_edid_from_record_data(record_data: bytes) -> str | None:
    pos = 0
    while pos + SUBREC_HEADER_LEN <= len(record_data):
        sig = record_data[pos : pos + 4]
        size = struct.unpack_from("<H", record_data, pos + 4)[0]
        start = pos + SUBREC_HEADER_LEN
        end = start + size
        if end > len(record_data):
            return None

        if sig == b"EDID":
            raw = record_data[start:end]
            return raw.decode("ascii", errors="ignore").rstrip("\x00")

        pos = end

    return None


def rebuild_plugin(plugin_bytes: bytes, valid_ids: set[str]) -> tuple[bytes, list[str], list[str]]:
    group_offset, group_end = parse_group(plugin_bytes)

    before_group = plugin_bytes[:group_offset]
    group_header = bytearray(plugin_bytes[group_offset : group_offset + HEADER_LEN])
    group_payload = plugin_bytes[group_offset + HEADER_LEN : group_end]
    after_group = plugin_bytes[group_end:]

    kept: list[str] = []
    dropped: list[str] = []
    new_payload = bytearray()

    pos = 0
    while pos + HEADER_LEN <= len(group_payload):
        rec_sig = group_payload[pos : pos + 4]
        rec_size = struct.unpack_from("<I", group_payload, pos + 4)[0]
        rec_total = HEADER_LEN + rec_size
        rec_end = pos + rec_total
        if rec_end > len(group_payload):
            raise RuntimeError("Invalid GMST record size while rebuilding")

        rec_blob = group_payload[pos:rec_end]
        if rec_sig != b"GMST":
            new_payload.extend(rec_blob)
            pos = rec_end
            continue

        rec_data = rec_blob[HEADER_LEN:]
        edid = get_edid_from_record_data(rec_data)
        if not edid:
            dropped.append("<missing-edid>")
        elif edid in valid_ids:
            kept.append(edid)
            new_payload.extend(rec_blob)
        else:
            dropped.append(edid)

        pos = rec_end

    new_group_size = HEADER_LEN + len(new_payload)
    struct.pack_into("<I", group_header, 4, new_group_size)

    rebuilt = bytearray()
    rebuilt.extend(before_group)
    rebuilt.extend(group_header)
    rebuilt.extend(new_payload)
    rebuilt.extend(after_group)

    return bytes(rebuilt), kept, dropped


def main() -> None:
    if not PLUGIN_PATH.exists():
        raise FileNotFoundError(f"Plugin not found: {PLUGIN_PATH}")
    if not STARFIELD_MASTER_PATH.exists():
        raise FileNotFoundError(f"Starfield master not found: {STARFIELD_MASTER_PATH}")

    plugin_bytes = PLUGIN_PATH.read_bytes()
    master_bytes = STARFIELD_MASTER_PATH.read_bytes()

    valid_ids = read_u_security_ids_from_master(master_bytes)
    rebuilt_bytes, kept, dropped = rebuild_plugin(plugin_bytes, valid_ids)

    if rebuilt_bytes == plugin_bytes:
        print("No changes needed; plugin already matches current master IDs.")
        return

    if not BACKUP_PATH.exists():
        BACKUP_PATH.write_bytes(plugin_bytes)

    PLUGIN_PATH.write_bytes(rebuilt_bytes)

    print(f"Rebuilt plugin: {PLUGIN_PATH}")
    print(f"Backup written: {BACKUP_PATH}")
    print(f"Kept records: {len(kept)}")
    for item in sorted(kept):
        print(f"  KEEP  {item}")
    print(f"Dropped records: {len(dropped)}")
    for item in sorted(dropped):
        print(f"  DROP  {item}")


if __name__ == "__main__":
    main()
