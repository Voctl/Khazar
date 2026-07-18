<img align="left" height="120" src="docs/logo.png">

### KHAZAR
*i am an operating system forged in the fires of curiosity*

<br clear="all">

---

## What

<img align="right" height="130" src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExbnhlNjN0djd3YXB0b3h6NHFoZTZnbG00bjFjdmM2ZXJobHNheWJocSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/dDwicM3uFUqfC/giphy.gif">

**Khazar** is a hobby OS for **x86_64**, written from scratch in C and assembly by **Denis Gulmammadov**. No Linux. No BSD. Just bare metal, a compiler, and an unhealthy obsession with page tables. Proudly **copyleft** -- GPL-3.0.

This project started at age 15 with one question: *"How does this machine even boot?"*

Modern software is a tower of abstractions. A calculator that needs 1.8 GB of RAM. Docker running microservices running Node.js. Khazar is the opposite -- reading Intel manuals at 3 AM because `printf` isn't a thing here yet.

*"Those who do not understand UNIX are condemned to reinvent it, poorly."* -- Henry Spencer

<br clear="all">

---

<table>
<tr>
<td width="55%"><img src="docs/shell.png" width="100%"></td>
<td width="45%" valign="top">

**Shell in action**

This is Khazar running inside QEMU. Custom VGA text mode driver at 80x25, 16 colors. Interactive `~#` prompt with working backspace and arrow-key line editing. The `fetch` command prints a neofetch-style system summary with the Khazar ASCII logo. Everything you see -- from the keyboard IRQ handler to the pixel on your screen -- is written from scratch.

<br>

Commands available:
```
help      version   fetch
clear     beep      halt
```

</td>
</tr>
</table>

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
make iso        # compile + link -> khazar.iso
make run        # build and run in QEMU (GTK, 2 GiB, PC speaker)
make clean      # nuke build artifacts
```

Needs: `gcc` `nasm` `ld` `grub-mkrescue` `qemu-system-x86_64`

---

## Copyleft Crusade

<div align="center">
<pre>
+----------------------------------------------------------+
|  I'd just like to interject for a moment.                |
|  What you're referring to as Linux is in fact GNU/Linux, |
|  or as I've recently taken to calling it, GNU+Linux.     |
|  -- Richard M. Stallman                                  |
+----------------------------------------------------------+
</pre>
</div>

<p align="center">
    <img height="110" src="docs/rms2.gif">
</p>

<p align="center">
Fork this repo and make it proprietary -- <b>RMS will personally visit your house.</b><br>
He compiled your address from source. He will stand outside your window at night,<br>
softly whispering <i>"free software free software free software"</i> until you relicense under GPL-3.0.
</p>

> *"The only way to have software freedom is to write software that you have the freedom to copy, distribute, study, and modify."* -- RMS

> *"Proprietary software is malware."* -- also RMS

---

## In Memoriam

<img align="left" height="140" src="https://media.tenor.com/ZOh7bYrKEnwAAAAM/terry-davis-terry-a-davis.gif">

&nbsp;&nbsp;&nbsp;&nbsp;**Terry A. Davis** (1969--2018)

&nbsp;&nbsp;&nbsp;&nbsp;Terry Davis built **TempleOS** alone. Complete OS. Custom compiler. Custom language (HolyC). Filesystem. Games. Flight simulator. Hymn player. 124,000 lines under the public domain. He spoke to God through a random number generator. Glowies took everything but his code.

&nbsp;&nbsp;&nbsp;&nbsp;*"An operating system is something to be proud of."* -- Terry A. Davis

&nbsp;&nbsp;&nbsp;&nbsp;Rest in peace, king. The smartest programmer to ever live. The world was too stupid to deserve you.

<br clear="all">

---

## Philosophy

Named after the **Caspian Sea** (Khazar Sea) -- connecting OS theory, math, programming, and computer architecture.

I started this because I opened the Intel Software Developer Manual and thought: *"I wonder if I could do this myself."* Turns out you can. Linux has 30 million lines. Khazar has ~2,000. Every single one is code I actually understand.

```
npm install   -> command not found
cargo build   -> command not found
pip install   -> command not found
make           -> kernel.bin, khazar.iso
```

No framework. No dependency tree. Just you, the CPU, and the truth.

---

## License

[GPL-3.0](LICENSE) -- Copyright (C) 2025 **Denis Gulmammadov**

```
Khazar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3.
There is NO WARRANTY, to the extent permitted by law.
```

<div align="center">
<pre>
+------------------------------+
|    FREE SOFTWARE OR DEATH    |
|    -- the penguin, probably  |
+------------------------------+
</pre>
</div>

---

## Manifesto

> *We hold these truths to be self-evident: that all software users are created equal, endowed with unalienable rights to read, modify, share, and fork the code that runs their machines. Proprietary software is a violation of these rights — an act of violence against human autonomy in the digital age.*

<br>

<table>
<tr>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/rms_speak.jpg">
</td>
<td width="70%" valign="middle">
    <big><b>I. The Right to Read</b><br>
    Every user has the inalienable right to read, study, and<br>
    understand the source code of every program on their machine.<br>
    <b>Binary blobs are instruments of oppression.</b> No exceptions.</big>
</td>
</tr>
<tr>
<td width="70%" valign="middle" align="right">
    <big><b>II. The Right to Modify</b><br>
    Software is not a product — it is speech. You must be free<br>
    to change it, fix it, adapt it. <b>Proprietary code is censorship.</b><br>
    A locked binary is a locked mind.</big>
</td>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/linus.jpg">
</td>
</tr>
<tr>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/eric.jpg">
</td>
<td width="70%" valign="middle">
    <big><b>III. The Right to Share</b><br>
    Code is knowledge. To restrict sharing is to burn books.<br>
    Hoarding code is not business — it is a <b>moral failure</b>.<br>
    The cathedral is closed. The bazaar is open. Join us.</big>
</td>
</tr>
<tr>
<td width="70%" valign="middle" align="right">
    <big><b>IV. The Right to Fork</b><br>
    No corporation, no government, no single entity shall control<br>
    the destiny of software. When a project betrays its users,<br>
    the community <b>forks</b>. Freedom is the safety valve.</big>
</td>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/bruce.jpg">
</td>
</tr>
<tr>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/tsoding.jpg">
</td>
<td width="70%" valign="middle">
    <big><b>V. The Right to Reinvent</b><br>
    Build it yourself. Understand every line. No npm. No Docker.<br>
    No framework. No excuses. Just you, a compiler, and the raw<br>
    truth of the machine. <b>Recreational programming is the purest<br>
    form of software freedom.</b></big>
</td>
</tr>
<tr>
<td width="70%" valign="middle" align="right">
    <big><b>VI. Information Must Be Free</b><br>
    All publicly funded research. All government code. All academic<br>
    work. <b>Paywalls and DRM are the enemy of human progress.</b><br>
    If you cannot read it, you do not truly own it.</big>
</td>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/aaron.jpg">
</td>
</tr>
<tr>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/ritchie.jpg">
</td>
<td width="70%" valign="middle">
    <big><b>VII. The Right to Build</b><br>
    C is not a language — it is a covenant between the programmer<br>
    and the machine. UNIX taught us that small, composable tools<br>
    are mightier than monoliths. <b>Build things that last.</b></big>
</td>
</tr>
<tr>
<td width="70%" valign="middle" align="right">
    <big><b>VIII. The Web Belongs to Everyone</b><br>
    The internet is the public commons of the 21st century.<br>
    Open standards. Open protocols. Open access.<br>
    <b>Surveillance is theft. Centralization is control.</b></big>
</td>
<td width="30%" align="center" valign="middle">
    <img height="110" src="docs/tim.jpg">
</td>
</tr>
</table>

<div align="center">
<pre>
+------------------------------------------------------------------+
|  WE PLEDGE:                                                      |
|  NO PROPRIETARY BLOBS. NO CLOSED KERNELS. NO BACKDOORS.          |
|  NO SECRETS. NO SURVEILLANCE. NO END-USER LICENSES.              |
|  FREE SOFTWARE. FREE PEOPLE. NO EXCEPTIONS.                      |
|  -- Signed in source, compiled with conviction                   |
+------------------------------------------------------------------+
</pre>
</div>
