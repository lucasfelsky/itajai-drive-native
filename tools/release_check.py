#!/usr/bin/env python3
"""Fail CI if an Itajai Drive release candidate is incomplete or inconsistent."""
from __future__ import annotations
import hashlib
import pathlib
import re
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
EXPECTED_CARS = {
    "car_fiat_uno_way_2014",
    "car_vw_gol_g6_2016",
    "car_hyundai_hb20_2024",
    "car_fiat_strada_2024",
    "car_toyota_corolla_2020",
    "car_jeep_renegade_2021",
    "car_chevrolet_onix_2024",
    "car_chevrolet_celta_2015",
}
REC_SIZE = 84


def fail(msg: str) -> None:
    print(f"RELEASE CHECK FAILED: {msg}", file=sys.stderr)
    raise SystemExit(1)


def sha256(path: pathlib.Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def check_version() -> str:
    version = (ROOT / "VERSION").read_text(encoding="utf-8").strip()
    if not re.fullmatch(r"\d+\.\d+\.\d+", version):
        fail(f"invalid VERSION: {version!r}")
    identity = (ROOT / "src" / "final" / "release_version.inc").read_text(encoding="utf-8")
    m = re.search(r'#define\s+ITAJAI_VERSION\s+"([^"]+)"', identity)
    if not m or m.group(1) != version:
        fail("release_version.inc does not match VERSION")
    stage = re.search(r'#define\s+ITAJAI_STAGE\s+"([^"]+)"', identity)
    if not stage or not stage.group(1).strip():
        fail("release stage is empty")
    print(f"OK release identity: {version} / {stage.group(1)}")
    return version


def check_binary(name: str, minimum: int) -> pathlib.Path:
    path = DIST / name
    if not path.is_file():
        fail(f"missing {name}")
    size = path.stat().st_size
    if size < minimum:
        fail(f"{name} unexpectedly small ({size} bytes)")
    print(f"OK {name}: {size} bytes sha256={sha256(path)}")
    return path


def check_pak() -> pathlib.Path:
    path = check_binary("itajai_assets_v08.pak", 40000)
    data = path.read_bytes()
    if data[:8] != b"IDAPAK08":
        fail("asset PAK magic mismatch")
    if len(data) < 16:
        fail("asset PAK header truncated")
    version, count = struct.unpack_from("<II", data, 8)
    if version != 1:
        fail(f"unsupported asset PAK version {version}")
    if count < 16 or count > 32:
        fail(f"unexpected asset count {count}")
    table_end = 16 + count * REC_SIZE
    if table_end > len(data):
        fail("asset PAK record table truncated")
    names = set()
    for i in range(count):
        off = 16 + i * REC_SIZE
        raw = data[off:off + 32].split(b"\0", 1)[0]
        try:
            name = raw.decode("utf-8")
        except UnicodeDecodeError:
            fail(f"asset #{i} has invalid UTF-8 name")
        names.add(name)
    missing = sorted(EXPECTED_CARS - names)
    if missing:
        fail("missing vehicle assets: " + ", ".join(missing))
    print(f"OK PAK: {count} assets, all 8 production vehicles present")
    return path


def check_manifest(version: str, managed: tuple[pathlib.Path, ...]) -> None:
    path = DIST / "manifest.txt"
    if not path.is_file():
        fail("manifest.txt missing")
    lines = [x.strip() for x in path.read_text(encoding="ascii").splitlines() if x.strip()]
    values = {}
    files = {}
    for line in lines:
        if line.startswith("file="):
            parts = line[5:].split("|")
            if len(parts) != 3:
                fail(f"bad manifest file line: {line}")
            name, size, digest = parts
            if not size.isdigit() or not re.fullmatch(r"[0-9a-f]{64}", digest):
                fail(f"bad manifest file metadata: {line}")
            if name in files:
                fail(f"duplicate manifest file: {name}")
            files[name] = (int(size), digest)
        elif "=" in line:
            k, v = line.split("=", 1)
            values[k] = v
    if values.get("protocol") != "1":
        fail("manifest protocol is not 1")
    if values.get("version") != version:
        fail("manifest VERSION mismatch")
    if not values.get("base_url", "").endswith(f"/v{version}/"):
        fail("manifest base_url does not target current tag")
    expected_names = {p.name for p in managed}
    if set(files) != expected_names:
        fail(f"manifest managed set mismatch: {sorted(files)} != {sorted(expected_names)}")
    for built in managed:
        meta = files.get(built.name)
        if not meta:
            fail(f"manifest missing {built.name}")
        if meta[0] != built.stat().st_size or meta[1] != sha256(built):
            fail(f"manifest metadata mismatch for {built.name}")
    cfg = (DIST / "update_config.ini").read_text(encoding="ascii")
    if "releases/latest/download/manifest.txt" not in cfg or "auto_launch=1" not in cfg:
        fail("update_config.ini is incomplete")
    print("OK updater channel: exact managed set, sizes, hashes and config validated")


def main() -> None:
    version = check_version()
    exe = check_binary("ItajaiDriveNative.exe", 100000)
    updater = check_binary("ItajaiDriveUpdater.exe", 15000)
    updater_next = check_binary("ItajaiDriveUpdater.next.exe", 15000)
    if sha256(updater) != sha256(updater_next):
        fail("updater handoff binary differs from release updater")
    pak = check_pak()
    check_manifest(version, (exe, pak, updater_next))
    print("OK updater handoff: .next binary is byte-identical to updater 1.1")
    print(f"RELEASE CHECK PASSED: Itajai Drive {version}")


if __name__ == "__main__":
    main()
