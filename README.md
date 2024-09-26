# Recheck
=======

Recheck is a small C/C++ program for checking if a set of files have been registered as dumps
on [redump.org](http://redump.org). Inspired by [Redump Verifier](https://github.com/normalgamer/RedumpVerifier),
it is a way for me to refresh C (using it as much as I can) and a tool for checking and renaming my game library!

## Usage
From the command line
```
recheck.exe "path/game1.iso" "game2.iso"...
```
The executable checks that a folder called 'dat' is present next to it, which holds the .dat files.
As of now, you need to download the DATs manually from redump ([here](http://redump.org/downloads/)).

## TO DO

- Automatically download and update the DATs from redump.
- Rename files with the names of the redump DATs.
- Implement recursive checks on folders.
- MAKE file.
- General cleanup.

## Dependencies

This program needs libcrypto from [openssl](https://github.com/openssl/openssl).

## Licenses

This program uses the source code from [pugixml](https://github.com/zeux/pugixml).
All the licenses are stored in the 'licenses' folder.
 