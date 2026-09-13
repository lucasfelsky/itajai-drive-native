#!/usr/bin/env python3
"""Validate the moving Renderer 5.0 beta channel release candidate."""
from __future__ import annotations
import re
from release_check import DIST, check_binary, check_pak, check_version, fail, sha256


def check_beta_manifest(version: str, managed):
    path = DIST / "manifest.txt"
    if not path.is_file():
        fail("beta manifest.txt missing")
    values = {}
    files = {}
    for raw in path.read_text(encoding="ascii").splitlines():
        line = raw.strip()
        if not line:
            continue
        if line.startswith("file="):
            parts = line[5:].split("|")
            if len(parts) != 3:
                fail(f"bad beta manifest file line: {line}")
            name, size, digest = parts
            if not size.isdigit() or not re.fullmatch(r"[0-9a-f]{64}", digest):
                fail(f"bad beta manifest metadata: {line}")
            files[name] = (int(size), digest)
        elif "=" in line:
            k, v = line.split("=", 1)
            values[k] = v
    if values.get("protocol") != "1":
        fail("beta manifest protocol is not 1")
    if values.get("version") != version:
        fail("beta manifest VERSION mismatch")
    if not values.get("base_url", "").endswith("/renderer-beta/"):
        fail("beta manifest base_url does not target renderer-beta")
    expected = {p.name for p in managed}
    if set(files) != expected:
        fail(f"beta manifest managed set mismatch: {sorted(files)} != {sorted(expected)}")
    for built in managed:
        size, digest = files[built.name]
        if size != built.stat().st_size or digest != sha256(built):
            fail(f"beta manifest metadata mismatch for {built.name}")

    cfg = (DIST / "update_config.ini").read_text(encoding="ascii")
    required = (
        "channel=beta",
        "installed_channel=beta",
        "stable_manifest_url=https://github.com/",
        "beta_manifest_url=https://github.com/",
        "releases/latest/download/manifest.txt",
        "releases/download/renderer-beta/manifest.txt",
        "auto_launch=1",
    )
    for token in required:
        if token not in cfg:
            fail(f"beta update_config.ini missing {token}")
    print("OK beta updater selected/installed channel config and moving manifest")


def main():
    version = check_version()
    exe = check_binary("ItajaiDriveNative.exe", 100000)
    updater = check_binary("ItajaiDriveUpdater.exe", 15000)
    updater_next = check_binary("ItajaiDriveUpdater.next.exe", 15000)
    if sha256(updater) != sha256(updater_next):
        fail("beta updater handoff binary differs")
    pak = check_pak()
    build = DIST / "BUILD.txt"
    if not build.is_file():
        fail("beta BUILD.txt missing")
    build_id = build.read_text(encoding="ascii").strip()
    if not re.fullmatch(r"[0-9a-f]{40}", build_id):
        fail("beta BUILD.txt must contain the gated commit SHA")
    check_beta_manifest(version, (exe, pak, updater_next, build))
    print(f"BETA RELEASE CHECK PASSED: Itajai Drive Renderer {version} build {build_id[:12]}")


if __name__ == "__main__":
    main()
