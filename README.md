<img align="left" height="120" src="docs/logo.png">

### KHAZAR
*a hobby OS for x86_64 I wrote from scratch ngl i was just bored*

<br clear="all">

---

## What is this

<img align="right" height="130" src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExbnhlNjN0djd3YXB0b3h6NHFoZTZnbG00bjFjdmM2ZXJobHNheWJocSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/dDwicM3uFUqfC/giphy.gif">

Khazar is an OS for **x86_64** Im writing from scratch in C and assembly. No Linux, no BSD, no "just fork something." Just GRUB handing control to my bootloader, my bootloader handing it to my kernel, and the kernel occasionally refusing to cooperate.

Started when I was 15. All because I asked myself *how the hell does this machine even boot?*

Modern softwares gone insane. A calculator that needs 1.8 GB of RAM. Docker running microservices running Node.js running your whole career. Khazars the opposite — just me at 3 AM reading Intel manuals because I dont even have a `printf` yet.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fc.tenor.com%2FFbgWUbqSQIIAAAAC%2Flucky-star-anime.gif&f=1&nofb=1&ipt=cf83c1c080563dfac7f91da9fd60ab6974bead2ced5e6ca984d844fef11e4a6b">

GPL-3.0, proudly. Fah closed source.

<br clear="all">

---

## Screenshot

**Shell in action**

Thats Khazar in QEMU. Custom VGA text mode driver, 80x25, 16 colors, working cursor and scrolling. Interactive `~#` prompt with backspace and arrow keys. `fetch` prints a neofetch-style summary with the ASCII logo.

Everything you see — keyboard IRQ, pixels, the beep — was written by hand. Nobody held my hand, goddamn it.

Commands:

```
help      version   fetch   vga50
clear     beep      halt    vga20
```

(Yeah, vga50 is just "switch to 50 lines." Took a whole driver. Dont judge me lol.)

<img src="docs/shell.png" width="100%">

---

## Features

| Kernel | Drivers | Shell |
|---------|----------|--------|
| x86_64 Long Mode (GRUB/Multiboot) | VGA text mode (80x25, 16 colors, cursor, scrolling) | Interactive REPL with `~#` prompt |
| GDT, IDT, PIC remapping, ISR/IRQ | PS/2 keyboard (US layout, shift, arrow keys) | Line editing (backspace, left/right arrows) |
| Bitmap PMM, multiboot memory map | PIT timer (100 Hz, `sleep`) | 8 built-in commands |
| Kernel heap (boundary-tag allocator) | PC speaker (square wave, beep) | `help` `version` `fetch` `clear` `beep` `halt` |

About 2,000 lines. Linux has 30 million. But I actually understand all 2,000 of mine, which idk if you can say about Linux.

---

## Build

```
git clone https://github.com/anomalyco/khazar.git
cd khazar
make iso        # compile + link -> khazar.iso
make run        # build and run in QEMU (GTK, 2 GiB, PC speaker)
make clean      # nuke build artifacts
```

Youll need: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

Missing any of those? `apt install` your friend. Or your enemy. Depends on the day fr.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FlUU2wbgHrioAAAAC%2Fkonata-luckystar.gif&f=1&nofb=1&ipt=6464414178cfb92e602deb23e989b44edd4d68d6af43dbb1243b19d1f1992bc6">

---

## Why the name

The Caspian Seas also called the **Khazar Sea**. It sits between East and West, connecting a bunch of different worlds. The OS is named after it because thats what this project does for me — it ties together OS theory, math, programming, and computer architecture into one big mess I get to untangle.

Also it sounds cool. Thats 90% of the decision, honestly ngl.

---

## Copyleft, because RMS would kill me otherwise

<p align="center">
    <img height="110" src="docs/rms_speak.jpg">
</p>

Fork this and make it proprietary — **RMS will personally visit your house.** He compiled your address from source. Hell stand outside your window at night whispering *"free software free software free software"* until you relicense under GPL-3.0. That man is relentless ngl.

> *"Proprietary software is malware."* -- Richard M. Stallman, who is right

---

## In Memoriam

<img align="left" height="140" src="https://media.tenor.com/ZOh7bYrKEnwAAAAM/terry-davis-terry-a-davis.gif">

&nbsp;&nbsp;&nbsp;&nbsp;**Terry A. Davis** (1969--2018)

&nbsp;&nbsp;&nbsp;&nbsp;Terry Davis built **TempleOS** alone. Complete OS, custom compiler, custom language (HolyC), filesystem, games, a flight simulator, a hymn player — 124,000 lines of public domain code. He talked to God through a random number generator and was completely sincere about it.

&nbsp;&nbsp;&nbsp;&nbsp;*"An operating system is something to be proud of."* -- Terry A. Davis

&nbsp;&nbsp;&nbsp;&nbsp;Rest in peace, king. The world was too stupid to deserve you, but your code outlived it. This whole thing exists because of guys like you.

<br clear="all">

---

## Why I built this shi

```
npm install   -> command not found
cargo build   -> command not found
pip install   -> command not found
make           -> kernel.bin, khazar.iso
```

No framework. No dependency tree. No `.env`, no CI, no 47 Slack channels. Just me, the CPU, and the truth.

One day I opened the Intel Software Developer Manual and thought *"I wonder if I could do this myself."* Turns out you can. It takes a lot of late nights and a few genuine nervous breakdowns over page tables, but you can. Trust me, I was there lol.

If you wanna write an OS, do it. Its the most rewarding thing Ive made, and also the most time Ive ever spent staring at a hex dump wondering where the shi went wrong.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FAq8PrtQFqrsAAAAd%2Fkonata-konata-happy.gif&f=1&nofb=1&ipt=96eff9a7b7ba08ae13d0fb11e2a8f77f06ab0e6e25b476cf6179e547784ce4d6">

---

## Manifesto

Yes, I wrote an actual manifesto. Five articles. Page tables, Stallman, the whole shi. Read it if you dare: **[THE KHAZAR MANIFESTO](MANIFESTO.md)** (its a lot, i know)

---

## License

[GPL-3.0](LICENSE) -- Copyright (C) 2025 **Denis Gulmammadov**

```
Khazar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3.
There is NO WARRANTY, to the extent permitted by law.
```

That "no warranty" bits doing a lot of heavy lifting. This OSll absolutely eat your hard drive if youre dumb enough to boot it on real hardware. I did. It did. Live and learn.
