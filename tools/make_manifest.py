#!/usr/bin/env python3
"""Build an Itajai Drive updater manifest from release files."""
import argparse, hashlib, pathlib
p=argparse.ArgumentParser()
p.add_argument('--version', required=True)
p.add_argument('--base-url', required=True)
p.add_argument('--notes', default='')
p.add_argument('files', nargs='+')
a=p.parse_args()
print('protocol=1')
print(f'version={a.version}')
print(f'base_url={a.base_url.rstrip("/")}/')
if a.notes: print(f'notes={a.notes}')
for name in a.files:
    path=pathlib.Path(name)
    data=path.read_bytes()
    print(f'file={path.name}|{len(data)}|{hashlib.sha256(data).hexdigest()}')
