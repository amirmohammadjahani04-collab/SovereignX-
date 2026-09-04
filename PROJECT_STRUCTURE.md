# SovereignX repository structure

The source tree is intentionally kept close to the original project layout so that
existing include paths and build assumptions are less likely to break.

## Removed from the source archive

Generated or backup artifacts were excluded:

- `kernel/main.o`
- `kernel/myos.kernel`
- `kernel/myos.arm64.backup`
- the malformed/duplicate-looking `kernel/drivers/keyboard.` file

Large release artifacts such as ISO/IMG files should be published through GitHub
Releases rather than committed to the source repository.

## Suggested future additions

- `LICENSE`
- `CONTRIBUTING.md`
- `docs/BUILDING.md`
- `docs/ARCHITECTURE.md`
- `CHANGELOG.md`
- a reproducible build script/Makefile
