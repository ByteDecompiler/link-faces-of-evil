# Link: The Faces of Evil

```diff
- WARNING! -

This repository is a work in progress. It builds a matching program, but the naming and
documentation are not complete, so the code may still change a lot at any time.
```

This is a **decompilation** of ***Link: The Faces of Evil*** ((Philips CD-i, 1993)). The purpose of the project is to recreate a source code base for the game's main program, `cdi_link`, using information found inside the game. **It is not producing a PC port.** The C code compiles back into the exact original program, using the same compiler the game was built with: Apple MPW C 3.0.

All 357 functions are decompiled to C ((96.5% of the code bytes)). The only parts still in assembly are the MPW runtime and C startup, a few drawing routines, and the two NVRAM name coders. Functions use the game's original names ((from its MacsBug symbols)), and globals, structs and library calls are named too.

It builds the following programs:

* `build/cdi_link.mod` ((English)) `sha1: 9ccc5294e2d8d530c37bd37d7d425dd8a5996bb0`
* `build/fr/cdi_link.mod` ((French)) `sha1: 35cf8961f8e5907316f5f9c4b6810c525a25cce8`
* `build/de/cdi_link.mod` ((German)) `sha1: 0b9fc1657a48a84cd3b2d2a2fe9dd92a659c9393`
* `build/nl/cdi_link.mod` ((Dutch)) `sha1: d7fa8524585615ee194be45b67b0707e778dbad2`

**Note: This repository does not include any of the game's code or data. A prior copy of the game is required to extract them.**

## Supported discs

* English: `Link - The Faces of Evil (Europe).bin` `sha1: 001dddf10baff8603bfd2d52cf39e82c699b402c`
* English: `Link - The Faces of Evil (USA).bin` `sha1: 6e280552344a25e4078a12e704e9b753495352d7`
* French: `Link - The Faces of Evil (France).bin` `sha1: 75bc4673081639cd21b8142ded4d96541d20a0ea`
* German: `Link - Die Fratzen des Boesen (Germany).bin` `sha1: 941ba0b9ef9421efbd4f87aaf2811b13598673be`
* Dutch: `Link - De Gezichten van het Kwaad (Netherlands).bin` `sha1: 5bc6a88561619fd7f5743696cc5a6d8c4af81313`

The Europe and USA discs have the same files so either one builds the English version. Any other English dump with the same files also works, including the promotional "not for resale" disc.

Everything under `disc/`, `extract/`, `asm/`, `bin/`, `build/` and `ghidra_project/` comes from your disc. Its ignored by git and should never be commited.

## Installation

### Windows

For Windows 10 or 11, install WSL and a distribution by following this [WSL Installation Guide](https://learn.microsoft.com/en-us/windows/wsl/install). We recommend using Ubuntu as the Linux distribution. Then follow the Linux instructions below.

### Linux ((Native or under WSL))

#### 1. Install build dependencies

The build process has the following requirements:

* Python 3
* A Python virtual environment with Capstone, for the disassembly tools:

```bash
python3 -m venv ~/foe_tools/venv
~/foe_tools/venv/bin/pip install capstone
```

The compiler runs in an emulated Mac, so you also need:

* Basilisk II, built from kanjitalk755/macemu with SDL2
* A 68040 Mac ROM that you own, at `emu/Quadra630.ROM` ((a Quadra 630 ROM was used))
* A System 7.5.3 disk image at `emu/System753.dsk`, with MPW 3.3 ((`tools/scripts/install_mpw.py`)) and MPW C 3.0 ((12/20/88, from the MPW 3.0/3.1 CD)) installed as the tool `C30`

Apple's software is not included in this repository. `emu/mpwrun.sh` runs a compile job in the emulator without any clicking. See [docs/emulator.md](docs/emulator.md) for more information.

#### 2. Clone the repository

Clone the repository where you wish to have the project, with the command:

```bash
git clone https://github.com/ByteDecompiler/link-faces-of-evil.git
```

This will copy the GitHub repository contents into a new folder in the current directory called `link-faces-of-evil`. Change into this directory before doing anything else:

```bash
cd link-faces-of-evil
```

#### 3. Prepare a base disc

Place a copy of the game disc ((`.bin` and `.cue`)) inside the `disc/EN/` folder, for example `disc/EN/Link - The Faces of Evil (Europe).bin`. Check its SHA-1 against the list above.

#### 4. Extract the program

Extract the program from the disc and split it into pieces with the following commands:

```bash
python3 tools/scripts/cdi_modules.py "disc/EN/Link - The Faces of Evil (Europe).bin" extract/modules
~/foe_tools/venv/bin/python -I tools/split.py
```

This extracts the program's code and data to `asm/` and `bin/`, and writes `config/functions.txt` and `config/jumptable.txt`.

#### 5. Build the program

Run the build script:

```bash
python3 tools/build.py
```

This compiles every function in `src/` in the emulator, links the program and checks its hash.

If all goes well, a new program should be built at `build/cdi_link.mod`, and the following text printed:

```
357 of 357 functions from C (96.5% of the code bytes)
build/cdi_link.mod: sha1 9ccc5294e2d8d530c37bd37d7d425dd8a5996bb0
OK: byte-identical to the original
```

If you instead see `NOT byte-identical`, the built program isn't the same as the original one so something went wrong or some part of the code doesn't match. Any function that doesn't match is listed as `DIFFERS`.

**Note:** `--no-compile` skips the compile and reuses the last objects ((handy if you only changed config files)). `--diff` shows each function that doesn't match next to the game's code.

## Changing build options

By default, the English version is built. To build another version, put its disc in `disc/FR/`, `disc/DE/` or `disc/NL/`, and run the same steps with `--version fr`, `de` or `nl`. For example, for German:

```bash
python3 tools/scripts/cdi_modules.py "disc/DE/Link - Die Fratzen des Boesen (Germany).bin" extract/DE/modules
~/foe_tools/venv/bin/python -I tools/split.py --version de
python3 tools/build.py --version de
```

All versions are built from the same C code. The differences are in `include/version.h`, selected at build time with `-d FRENCH`, `-d GERMAN` or `-d DUTCH`:

* Disc positions, buffer sizes, and the layout of text whose length depends on the language ((the inventory, the signs, the death screen)).
* The German and Dutch versions are later builds with one small change in the code: when saving the game, they check the free space before deleting the old save instead of after ((`NEW_SAVE_CHECK`, in `save_game_to_NVR`)).

The globals are at different offsets in every version because the texts have different lengths. These are listed in `config/<version>/symbols.txt`, made by `tools/port_symbols.py`.

`--mod` builds a modified program. See [docs/modding.md](docs/modding.md).

## Project Structure

```
link-faces-of-evil
├── asm: assembly pieces, extracted from your program (generated)
├── bin: module header and data, extracted from your program (generated)
├── build: output directory
├── config: what the build needs to know about the program
│   ├── functions.txt: every code piece in module order
│   ├── symbols.txt: globals and library routines, with their offsets
│   ├── jumptable.txt, segments.txt, relocs.txt: MPW jump table, code segments, references to fix in mods
│   └── fr, de, nl: files for the other versions
├── disc: your disc images (not included)
├── docs: documentation
├── emu: the emulated Mac that runs the compiler
├── include
│   ├── game.h: globals (with their offsets), structs, prototypes
│   └── version.h: values that differ between versions
├── src: C source code, one file per function
└── tools: build tools
    ├── build.py, split.py: the build and the splitter
    ├── port_symbols.py: finds the globals' offsets in another version
    ├── patch_disc.py: writes a built program into a copy of the disc image
    ├── cdi_fs.py, extract_*.py, a5init.py: disc reader and content extractors
    ├── ghidra_setup.ps1, ghidra_scripts: a Ghidra project with the program named and analysed
    └── scripts: comparison, disassembly and setup helpers
```

## Documentation

* [docs/data.md](docs/data.md): the files on the disc, the game's tables, the display, loading and saving.
* [docs/mechanics.md](docs/mechanics.md): the game's mechanics ((items, damage, the shop, rooms)), as written in the code.
* [docs/bugs.md](docs/bugs.md): bugs in the original code.
* [docs/modding.md](docs/modding.md): building mods, testing them in an emulator, extracting graphics and sound.
* [docs/emulator.md](docs/emulator.md): how the compiler was found, and the emulated Mac that runs it.

### How it works

`cdi_link.mod` is an OS-9 module. Its code was built on a Mac with MPW C 3.0 ((December 1988)), then converted to OS-9. Globals are reached through register A5, and every function has its name stored as a MacsBug symbol.

`tools/split.py` splits the module into 362 pieces of code: 344 named functions, 13 C functions without a name, 2 assembly routines called like C functions, and 3 stretches of assembly between functions.

`tools/build.py` compiles each function and puts it in its place in the module, using `config/symbols.txt` to find globals and routines by name. It then recomputes the module's CRC and checks its SHA-1.

To use Ghidra ((in PowerShell)), run `pwsh tools\ghidra_setup.ps1`, then open `ghidra_project\FacesOfEvil.gpr`.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

`main` has to build a matching program for every version. See [CONTRIBUTING.md](CONTRIBUTING.md) to get started.
