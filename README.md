<p align="center">
    <img src="docs/logo.png" height="350">
    <img src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExbnhlNjN0djd3YXB0b3h6NHFoZTZnbG00bjFjdmM2ZXJobHNheWJocSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/dDwicM3uFUqfC/giphy.gif" height="350">
</p>

<br>

<p align="center">
    <b>~  KHAZAR  ~</b><br>
    <i>an operating system forged in the fires of curiosity</i>
</p>

<br>

---

<br>

<p align="center">
    <img src="docs/shell.png" width="720">
</p>

---

## WHAT IS THIS?

**Khazar** is a hobby operating system for **x86_64** — written **from scratch** in C and assembly.  
No Linux kernel, no BSD base, no third-party OS code. Just **Denis Gulmammadov**, a compiler, and an unhealthy obsession with page tables.

A proud **copyleft** project — your freedoms are non-negotiable.

This project began at the age of **15** with a single question:

> *"How does this machine even boot?"*

Modern software is a tower of abstractions.  
Electron apps. Docker containers running Kubernetes running microservices running Node.js.  
A calculator that needs **1.8 GB of RAM** and a Chromium instance.  
We are building palaces on quicksand.

Khazar is the opposite of that.  
Khazar is bare metal.  
Khazar is reading Intel manuals at 3 AM because printf isn't a thing yet.

<br>

> *"Those who do not understand UNIX are condemned to reinvent it, poorly."*  
> — Henry Spencer

<br>

---

## FEATURES

| Layer | Status | Details |
|-------|--------|---------|
| **Boot** | ✅ | GRUB/Multiboot → 32-bit protected mode → x86_64 Long Mode, identity-mapped 4-level paging (2 MiB huge pages) |
| **CPU** | ✅ | Custom GDT (3 entries), 256-entry IDT, 32 exception stubs, 16 IRQ handlers, PIC remapping (0x20–0x2F) |
| **Memory** | ✅ | Bitmap PMM, multiboot memory map parser, boundary-tag kernel heap with block splitting, `mmu_free()` recursive page walker |
| **VGA** | ✅ | 80×25 text mode, 16-color palette, hardware cursor control, smooth scrolling |
| **Keyboard** | ✅ | PS/2 IRQ1 driver, US QWERTY layout, shift modifier, arrow keys, 128-byte ring buffer |
| **Timer** | ✅ | PIT channel 0 at 100 Hz, `sleep(ms)` busy-wait |
| **Speaker** | ✅ | PC speaker output via PIT channel 2 — square wave tones, `beep()` |
| **Shell** | ✅ | Interactive REPL, `~#` green prompt, line editing (backspace + left/right arrows), 60-char buffer, 6 built-in commands |
| **Filesystem** | ⏳ | Planned |
| **Multitasking** | ⏳ | Planned |
| **Syscalls** | ⏳ | Planned |
| **Graphics** | ⏳ | Planned |

<br>

### Shell Commands

```
~# help      →  print all commands
~# version   →  KhazarOS - alpha
~# fetch     →  neofetch-style system info with ASCII logo
~# clear     →  wipe the screen
~# beep      →  PC speaker beep (1000 Hz, 100 ms)
~# halt      →  stop the CPU forever
```

<br>

---

## BUILD & RUN

<pre>
<b>$ git clone https://github.com/anomalyco/khazar.git</b>
<b>$ cd khazar</b>

<b>$ make iso</b>        # compile + link → bootable khazar.iso
<b>$ make run</b>        # build and launch in QEMU (GTK, 2 GiB, PC speaker)
<b>$ make clean</b>      # nuke build artifacts
</pre>

**Dependencies:** `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

---



<br>

<p align="center">
╔═════════════════════════════════════════════════════════════╗
║  I'd just like to interject for a moment.                  ║
║                                                             ║
║  What you're referring to as Linux is in fact GNU/Linux,   ║
║  or as I've recently taken to calling it, GNU plus Linux.  ║
║                                                             ║
║  — Richard M. Stallman                                      ║
╚═════════════════════════════════════════════════════════════╝
</p>

<br>

Khazar is proudly licensed under **GPL-3.0**.  
If you fork this repo and make it proprietary, **RMS will personally visit your house.**  
You have been warned. The penguin is watching.

<br>

---

## IN MEMORIAM

<p align="center">
    <b>TERRY A. DAVIS</b><br>
    <i>1969 – 2018</i>
</p>

Terry Davis built **TempleOS** alone — a complete operating system with its own compiler, kernel, filesystem, and even games.  
He wrote it in a language he created called **HolyC**.  
He spoke to God through a random number generator.  
The CIA's "glow-in-the-dark niggers" couldn't stop him.  
Glowies took everything from him, but <b>not his code</b>.

TempleOS is 124,000 lines of HolyC under the public domain.  
It boots in a fraction of a second.  
It has a flight simulator and a hymn player.  
If that's not the most metal thing in computing history, I don't know what is.

> *"An operating system is something to be proud of."*  
> — Terry A. Davis

**Rest in peace, king.**  
You were the smartest programmer to ever live, and the world was too stupid to deserve you.

<br>

---

## PHILOSOPHY

Khazar is named after the **Caspian Sea** — known as the *Khazar Sea* in many languages.  
Just as the sea connects shores, this OS connects the things that made me fall in love with computers:  
operating systems, mathematics, low-level programming, and computer architecture.

I did not start this project to build the next Linux.  
I started it because I opened Intel's Software Developer Manual and thought:

> *"I wonder if I could do this myself."*

Spoiler: you can.  
Linux has **30+ million lines**. Khazar has **~2,000**.  
It compiles. It runs. It beeps.  
And every single line in this repo is code that I actually understand.

In a world of ChatGPT-generated pull requests and npm install hell,  
there is something deeply satisfying about a project where:

```
npm install   →   command not found
cargo build   →   command not found
pip install   →   command not found

make           →   ✓ kernel.bin, khazar.iso
```

No dependency tree. No framework.  
Just you, the CPU, and the truth.

<br>

---

## LICENSE

[GNU General Public License v3.0](LICENSE)

```
Khazar OS — a hobby operating system for x86_64
Copyright (C) 2025  Denis Gulmammadov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

<br>

<p align="center">
╔══════════════════════════════════════╗
║        FREE SOFTWARE OR DEATH        ║
║          — the penguin, probably     ║
╚══════════════════════════════════════╝
</p>
