<img align="left" height="120" src="docs/logo.png">

### KHAZAR
*a hobby OS for x86_64 I wrote from scratch ngl i was just bored*

<br clear="all">

---

## What is this

<img align="right" height="130" src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExbnhlNjN0djd3YXB0b3h6NHFoZTZnbG00bjFjdmM2ZXJobHNheWJocSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/dDwicM3uFUqfC/giphy.gif">

Khazar is an OS for **x86_64** Im writing from scratch in C and assembly. No Linux, no BSD, no "just fork something." Just GRUB, my bootloader, my kernel, and a lot of late nights reading Intel manuals.
Started when I was 15, because I asked myself *how the hell does this machine even boot?*
GPL-3.0, proudly. Fah closed source.

<br clear="all">

---

## Screenshot

<img src="docs/shell.png" width="100%">

Thats Khazar in QEMU. Custom VGA text mode driver, interactive `~#` prompt, working keyboard, the whole shi written by hand. Commands:

```
help      version   fetch   vga50
clear     beep      halt    vga20
```

---

## Features

- x86_64 Long Mode (GRUB/Multiboot), GDT, IDT, PIC, ISR/IRQ
- Bitmap PMM + kernel heap (boundary-tag allocator)
- VGA text mode, PS/2 keyboard, PIT timer, PC speaker
- Interactive shell with 8 commands

About 2,000 lines. Linux has 30 million. I understand all 2,000 of mine.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fc.tenor.com%2FFbgWUbqSQIIAAAAC%2Flucky-star-anime.gif&f=1&nofb=1&ipt=cf83c1c080563dfac7f91da9fd60ab6974bead2ced5e6ca984d844fef11e4a6b">

---

## Build

```
git clone https://github.com/anomalyco/khazar.git
cd khazar
make iso        # compile + link -> khazar.iso
make run        # build and run in QEMU
make clean      # nuke build artifacts
```

Needs: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

Missing any of those? `apt install` your friend. Or your enemy. Depends on the day fr.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FlUU2wbgHrioAAAAC%2Fkonata-luckystar.gif&f=1&nofb=1&ipt=6464414178cfb92e602deb23e989b44edd4d68d6af43dbb1243b19d1f1992bc6">

---

## Why the name

The Caspian Seas also called the **Khazar Sea**. It connects East and West, and this project connects OS theory, math, programming, and architecture into one big mess I get to untangle. Also it sounds cool. Thats 90% of it.

---

## Why I built this shi


No framework. No dependency tree. No `.env`, no CI. Just me, the CPU, and the truth.

If you wanna write an OS, do it. Its the most rewarding thing Ive made, and the most time Ive ever spent staring at a hex dump wondering where the shi went wrong.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FAq8PrtQFqrsAAAAd%2Fkonata-konata-happy.gif&f=1&nofb=1&ipt=96eff9a7b7ba08ae13d0fb11e2a8f77f06ab0e6e25b476cf6179e547784ce4d6">

---

## Copyleft, because RMS would kill me otherwise

<p align="center">
    <img height="110" src="docs/rms_speak.jpg">
</p>

Fork this and make it proprietary — **RMS will personally visit your house.** He compiled your address from source. Hell stand outside your window at night whispering *"free software free software free software"* until you relicense under GPL-3.0. That man is relentless ngl.

> *"Proprietary software is malware."* -- Richard M. Stallman, who is right

---

## Manifesto

Yes, I wrote an actual manifesto. Five articles. Stallman, page tables, the whole shi. Read it if you dare: **[THE KHAZAR MANIFESTO](MANIFESTO.md)**

---

## In Memoriam

<img align="left" height="140" src="https://media.tenor.com/ZOh7bYrKEnwAAAAM/terry-davis-terry-a-davis.gif">

**Terry A. Davis** (1969--2018) built **TempleOS** alone — complete OS, custom compiler, custom language, games, 124,000 lines of public domain code. He talked to God through a random number generator and was completely sincere about it.

Rest in peace, king. The world was too stupid to deserve you.

<br clear="all">

---

## License

[GPL-3.0](LICENSE) -- Copyright (C) 2025 **Denis Gulmammadov**

That "no warranty" bit's doing heavy lifting. This OS'll eat your hard drive if youre dumb enough to boot it on real hardware. I did. It did. Live and learn.
