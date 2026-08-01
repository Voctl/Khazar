<p align="center">

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

<h1>KHAZAR</h1>
a hobby <b>x86_64</b> OS wrote from scratch in C + asm.<br>
no linux, no bsd, no forks. just grub, my kernel, and way too many late nights with intel manuals.<br>
started this when i was 15, genuinely just to see if i could. best decision of my life fr.

</p>

<div align="center">

<img src="docs/shell.png" height="140"> &nbsp; <img src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FlUU2wbgHrioAAAAC%2Fkonata-luckystar.gif&f=1&nofb=1" height="140">
</div>

custom vga text mode driver, ps/2 keyboard, pit timer, interactive `~#` prompt — all hand written, no copypasta ngl.

**features:** x86_64 long mode (grub/multiboot) · gdt/idt/pic/isr-irq · bitmap pmm + kernel heap · vga text mode · interactive shell

about 2,000 lines. linux has 30 million. i understand all 2,000 of mine. and thats the point, yo king.

<div align="center">
<img src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FAq8PrtQFqrsAAAAd%2Fkonata-konata-happy.gif&f=1&nofb=1" height="90">
</div>

## build

```bash
git clone https://github.com/Voctl/Khazar.git
cd khazar
make iso        # compile + link -> khazar.iso
make run        # build and run in QEMU
make clean      # nuke build artifacts
```

needs: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64` — missing any? `apt install` that shi fr.

## why its free

<img src="docs/rms_speak.jpg" width="90" align="left">

GPL-3.0, proudly. **fuck closed source** — proprietary software is malware, straight up. your EULA is toilet paper. if you cant read the code running on your machine, you dont own your machine. fork it, fix it, break it, make it yours. no blobs, no backdoors dressed as features. free software or perish fr.

<br clear="all">

[GPL-3.0](LICENSE) — copyright (C) 2025 **Denis Gulmammadov**

that "no warranty" bit is doing heavy lifting. this OS will eat your hard drive if you boot it on real hardware. i did. it did. live and learn.
