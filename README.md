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

a hobby **x86_64** OS i wrote from scratch in C + asm. no linux, no bsd, no forks. just grub, my kernel, and way too many late nights with intel manuals.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FlUU2wbgHrioAAAAC%2Fkonata-luckystar.gif&f=1&nofb=1&ipt=6464414178cfb92e602deb23e989b44edd4d68d6af43dbb1243b19d1f1992bc6">

<img align="right" height="120" src="docs/shell.png">
<br clear="all">

custom vga text mode driver, ps/2 keyboard, pit timer, interactive `~#` prompt — all hand written.

**features**: x86_64 long mode (grub/multiboot), gdt/idt/pic/isr-irq, bitmap pmm + kernel heap, vga text mode, interactive shell.

about 2,000 lines. linux has 30 million. i understand all 2,000 of mine.

<img align="right" height="70" src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fmedia1.tenor.com%2Fm%2FAq8PrtQFqrsAAAAd%2Fkonata-konata-happy.gif&f=1&nofb=1&ipt=96eff9a7b7ba08ae13d0fb11e2a8f77f06ab0e6e25b476cf6179e547784ce4d6">
<br clear="all">

---

## build

```
git clone https://github.com/Voctl/Khazar.git
cd khazar
make iso        # compile + link -> khazar.iso
make run        # build and run in QEMU
make clean      # nuke build artifacts
```

needs: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

---

## why its free

<img align="left" height="110" src="docs/rms_speak.jpg">

GPL-3.0, proudly. **fuck closed source** — proprietary software is malware, straight up. your EULA is toilet paper. if you cant read the code running on your machine, you dont own your machine. fork it, fix it, break it, make it yours. no blobs, no backdoors dressed as features. free software or perish fr.

<br clear="all">

---

[GPL-3.0](LICENSE) -- copyright (C) 2025 **Denis Gulmammadov**

that "no warranty" bit is doing heavy lifting. this OSll eat your hard drive if you boot it on real hardware. i did. it did. live and learn.
