<pre>
       ____
     /      \
    /   ____/
   /   /
  |   |
  |   |__________
  \              \
   \______________\
</pre>

# KHAZAR

A hobby **x86_64** OS written from scratch in C and assembly. No Linux, no BSD, no forks. Just GRUB, my kernel, and the Intel manuals.

## Features

- x86_64 Long Mode (GRUB/Multiboot), GDT, IDT, PIC, ISR/IRQ
- Bitmap PMM + kernel heap (boundary-tag allocator)
- VGA text mode, PS/2 keyboard, PIT timer, PC speaker
- Interactive `~#` shell

## Build

```
git clone https://github.com/Voctl/Khazar.git
cd khazar
make iso        # compile + link -> khazar.iso
make run        # build and run in QEMU
make clean      # nuke build artifacts
```

Needs: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

---

## Copyleft, because RMS would kill me otherwise

<p align="center">
    <img height="110" src="docs/rms_speak.jpg">
</p>

Fork this and make it proprietary — **RMS will personally visit your house.** He compiled your address from source. Hell stand outside your window at night whispering *"free software free software free software"* until you relicense under GPL-3.0.

> *"Proprietary software is malware."* -- Richard M. Stallman, who is right

---

## In Memoriam

<img align="left" height="140" src="https://media.tenor.com/ZOh7bYrKEnwAAAAM/terry-davis-terry-a-davis.gif">

**Terry A. Davis** (1969--2018) built **TempleOS** alone — complete OS, custom compiler, custom language, games, 124,000 lines of public domain code. He talked to God through a random number generator and was completely sincere about it.

Rest in peace, king. The world was too stupid to deserve you.

<br clear="all">

---

## License

[GPL-3.0](LICENSE) -- Copyright (C) 2025 **Denis Gulmammadov**
