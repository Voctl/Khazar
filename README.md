# Khazar

<p align="center"> <img src="docs/logo.png" width="128" alt="Khazar logo"> </p>

Khazar is a small operating system for x86_64 computers.

The project exists for learning, experimentation and curiosity. It is developed with simplicity in mind and follows the idea that software should be understandable by the people who use and modify it.

Khazar is not trying to be everything. There are already enough operating systems attempting to solve every problem ever invented. Khazar focuses on doing a few things well and keeping the codebase approachable.

The system is written from scratch and serves as a playground for operating systems, computer architecture and low-level programming.

## Building

Requirements:

* nasm
* gcc
* ld
* grub-mkrescue
* qemu-system-x86_64

Build:

```
make
```

Run:

```
make run
```

Clean:

```
make clean
```

## Philosophy

* Simplicity over complexity.
* Readable code over clever code.
* Understanding over abstraction.
* Small systems over large frameworks.
* Freedom to modify and experiment.

## License

GPL-3.0
