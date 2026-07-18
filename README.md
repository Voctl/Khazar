<p align="center">
    <img src="docs/logo.png" height="350">
    <img src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExbnhlNjN0djd3YXB0b3h6NHFoZTZnbG00bjFjdmM2ZXJobHNheWJocSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/dDwicM3uFUqfC/giphy.gif" height="350">
</p>

<p align="center">
    <b>KHAZAR</b><br>
    <i>an operating system forged in the fires of curiosity</i>
</p>

---

<p align="center">
    <img src="docs/shell.png" width="720">
</p>

---

## What

**Khazar** is a hobby OS for **x86_64**, written from scratch in C and assembly by **Denis Gulmammadov**. No Linux. No BSD. Just bare metal, a compiler, and an unhealthy obsession with page tables. Proudly **copyleft** — GPL-3.0.

This project started at age 15 with one question: *"How does this machine even boot?"*

Modern software is a tower of abstractions. Electron apps, Docker running Kubernetes running microservices running Node.js. A calculator that needs 1.8 GB of RAM. We are building palaces on quicksand. Khazar is the opposite — reading Intel manuals at 3 AM because `printf` isn't a thing here yet.

> *"Those who do not understand UNIX are condemned to reinvent it, poorly."* — Henry Spencer

---

## Features

| Kernel | Drivers | Shell |
|---------|----------|--------|
| x86_64 Long Mode (GRUB/Multiboot) | VGA text mode (80x25, 16 colors, cursor, scrolling) | Interactive REPL with `~#` prompt |
| GDT, IDT, PIC remapping, ISR/IRQ | PS/2 keyboard (US layout, shift, arrow keys) | Line editing (backspace, left/right arrows) |
| Bitmap PMM, multiboot memory map | PIT timer (100 Hz, `sleep`) | 6 built-in commands |
| Kernel heap (boundary-tag allocator) | PC speaker (square wave, beep) | `help` `version` `fetch` `clear` `beep` `halt` |

---

## Build

```
git clone https://github.com/anomalyco/khazar.git
cd khazar
make iso        # compile + link - khazar.iso
make run        # build and run in QEMU (GTK, 2 GiB, PC speaker)
make clean      # nuke build artifacts
```

Needs: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

---

## Project structure

```
src/
 arch/x86_64/    cpu (GDT, IDT, ISR, port I/O)
 boot/            multiboot loader, linker script, GRUB config
 drivers/         VGA, keyboard, PIT, PC speaker
 fs/              filesystem (planned)
 include/         kernel + multiboot headers
 kernel/          kernel main, panic handler
 libk/            string, types, memcpy, memset, strcmp
 mm/              physical memory manager, kernel heap
 proc/            multitasking (planned)
 shell/           interactive REPL, 6 commands
 sys/             system calls (planned)
```

---

<p align="center">
<pre>
+--------------------------------------------------------+
|  I'd just like to interject for a moment.               |
|  What you're referring to as Linux is in fact GNU/Linux,|
|  or as I've recently taken to calling it, GNU+Linux.    |
|  — Richard M. Stallman                                  |
+--------------------------------------------------------+
</pre>
</p>

Fork this repo and make it proprietary — **RMS will personally visit your house.** The penguin is watching.

---

## In Memoriam — Terry A. Davis (1969–2018)

Terry Davis built **TempleOS** alone. Complete OS. Custom compiler. Custom language (HolyC). Filesystem. Games. Flight simulator. Hymn player. 124,000 lines under the public domain. He spoke to God through a random number generator. Glowies took everything but his code.

> *"An operating system is something to be proud of."* — Terry A. Davis

Rest in peace, king. The smartest programmer to ever live. The world was too stupid to deserve you.

---

## Philosophy

Named after the **Caspian Sea** (Khazar Sea) — connecting OS theory, math, programming, and computer architecture.

I started this because I opened the Intel Software Developer Manual and thought: *"I wonder if I could do this myself."* Turns out you can. Linux has 30 million lines. Khazar has ~2,000. Every single one is code I actually understand.

```
npm install   - command not found
cargo build   - command not found
pip install   - command not found
make           - kernel.bin, khazar.iso
```

No framework. No dependency tree. Just you, the CPU, and the truth.

---

## License

[GPL-3.0](LICENSE) — Copyleft ( ɔ ) 2025 **Denis Gulmammadov**

```
Khazar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3.
There is NO WARRANTY, to the extent permitted by law.
```

<p align="center">
<pre>
+-----------------------------+
|    FREE SOFTWARE OR DEATH    |
|    — the penguin, probably   |
+-----------------------------+
</pre>
</p>
