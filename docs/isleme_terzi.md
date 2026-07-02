# 🌊 Khazar OS — İşləmə Tərzi və Sənədləşdirmə (Documentation)

Salam, qardaş! Bu sənəddə Khazar OS kernelinin sıfırdan necə yükləndiyi, yaddaşı necə idarə etdiyi və kəsilmələri (interrupts) necə idarə etdiyini detallı şəkildə izah edirik. Kodun daxilindəki şərhlər (comments) və yazılış tərzinə uyğun olaraq hər bir hissəni addım-addım təhlil edək.

---

## 📂 Layihənin Qısa Strukturu (Layout)

*   **`src/boot`** — Sistemin ilk ayaq basdığı yer. 32-bit Protected Mode-dan 64-bit Long Mode-a keçid və paging-in ilkin qurulması.
*   **`src/kernel`** — Kernelin əsas başlanğıc funksiyası (`kernel_main`) və panic mexanizmi.
*   **`src/cpu`** — Prosessor səviyyəsində tənzimləmələr: GDT, IDT və Interrupt Service Routines (ISRs).
*   **`src/drivers`** — VGA ekran driveri, klaviatura (keyboard) və PIT timer.
*   **`src/memory`** — Fiziki Yaddaş Meneceri (PMM).
*   **`src/include`** — Bütün başlıq (header) faylları.

---

## 🚀 1. Bootloader mərhələsi (`src/boot/loader.s`)

Sistem ilk işə düşəndə GRUB bootloader kernelimizi yükləyir. `loader.s` faylı kernelimizin giriş qapısıdır.

### 🔹 Multiboot Header
GRUB-un kerneli tanıması üçün faylın lap başında **Multiboot** standartına uyğun olaraq magic number, flags və checksum təyin edirik:
*   `MAGIC_NUMBER equ 0x1BADB002`
*   `FLAGS equ (1 << 1)` — Bizə memory map (mmap) məlumatı lazımdır deyə GRUB-dan bunu tələb edirik.

### 🔹 32-bit Protected Mode-dan 64-bit Long Mode-a Keçid
GRUB bizi 32-bitlik Protected Mode-da qarşılayır. Biz 64-bit rejiminə keçmək üçün bu addımları yerinə yetiririk:
1.  **Stakın (Stack) təyin olunması**: `esp` registrini müvəqqəti olaraq `kernel_stack`-in sonuna yönləndiririk.
2.  **Multiboot ünvanının qorunması**: `ebx` registrində gələn Multiboot məlumat strukturunun ünvanını `ebp`-yə köçürürük. Çünki növbəti addımda çağıracağımız `cpuid` əmri `ebx` registrini sıfırlayacaq (bu çox vacib məqamdır!).
3.  **CPUID və Long Mode yoxlanışı**: Prosessorun 64-bit dəstəklədiyindən əmin olmaq üçün `check_cpuid` və `check_long_mode` funksiyalarını çağırırıq. Dəstəklənmirsə, ekrana səhv kodu yazdırıb sistemi dayandırırıq (`hlt`).
4.  **İlkin Paging-in qurulması (`setup_page_tables`)**: 64-bit Long Mode paging olmadan işləyə bilməz. Biz ilkin olaraq 1 GiB yaddaşı **Identity Mapping** (fiziki ünvan = virtual ünvan) edirik:
    *   `p4_table` (PML4) -> `p3_table` (PDPT) -> `p2_table` (PD).
    *   `p2_table` üzərində 512 ədəd entry-ni döngü ilə doldururuq. Hər entry 2 MiB ölçüsündə böyük səhifəni (Huge Page) təmsil edir (`or eax, 0b10000011` -> present, writable, huge page flags).
5.  **Paging-in Aktivləşdirilməsi (`enable_paging`)**:
    *   P4 cədvəlinin ünvanını `cr3` registrinin daxilinə yazırıq.
    *   `cr4` registrində PAE (Physical Address Extension) bitini (bit 5) aktivləşdiririk.
    *   EFER (Extended Feature Enable Register) MSR (Model Specific Register) vasitəsilə Long Mode-u aktiv edirik (`or eax, 1 << 8`).
    *   Nəhayət, `cr0` registrində Paging bitini (bit 31) 1 edərək paging-i rəsmi olaraq qoşuruq.
6.  **64-bit Seqmentinə Keçid**: `gdt64` pointerini yükləyirik (`lgdt [gdt64.pointer]`) və uzaq tullanma (far jump) edərək 64-bitlik `long_mode_start` kod blokuna keçirik.
7.  **`kernel_main` çağırışı**: Bütün data seqmentlərini sıfırlayırıq, `ebp`-də qoruduğumuz Multiboot ünvanını `rdi` (64-bit-də funksiyaya ötürülən 1-ci arqument) registrinə köçürüb `kernel_main` funksiyasına keçid edirik.

---

## 🛠 2. GDT (Global Descriptor Table) (`src/cpu/gdt.c` & `gdtasm.s`)

64-bit-də seqmentasiya demək olar ki, istifadə olunmur, lakin imtiyaz səviyyələrini (Kernel vs User Mode) təyin etmək üçün yenə də GDT lazımdır.

*   `init_gdt` funksiyası ilə cəmi 5 descriptor təyin edirik:
    1.  `Null Segment` (Boş)
    2.  `Kernel Code Segment` (Access: `0x9A`, Granularity: `0xAF` — 64-bit kodu təyin edir)
    3.  `Kernel Data Segment` (Access: `0x92`, Granularity: `0xCF`)
    4.  `User Code Segment` (Access: `0xFA`, Granularity: `0xAF`)
    5.  `User Data Segment` (Access: `0xF2`, Granularity: `0xCF`)
*   Descriptor-lar qurulduqdan sonra assembly daxilindəki `gdt_flush` funksiyası çağırılır. Bu funksiya `lgdt` ilə yeni cədvəli yükləyir, bütün data seqmentlərini (`ds`, `es`, `fs`, `gs`, `ss`) `0x10` (Kernel Data offset) ilə yeniləyir.
*   Code segment-i (CS) yeniləmək üçün isə staka kernel code selectoru (`0x08`) və hədəf ünvanı itələyib `retfq` (Far Return) əmri ilə tullanma edirik.

---

## ⚡ 3. IDT və Kəsilmələrin İdarə Edilməsi (`src/cpu/idt.c`, `gdtasm.s` & `isr.c`)

Kəsilmələr (Interrupts) prosessorun kənar hadisələrə (məsələn, klaviaturaya basılması) və ya daxili səhvlərə (məsələn, sıfıra bölmə - Page Fault) cavab vermə mexanizmidir.

### 🔹 IDT Quruluşu
*   `idt` massivində cəmi 256 ədəd kəsilmə girişi (gate) var.
*   `idt_set_gate` funksiyası ilə verilən kəsilmə nömrəsinin ünvanını (low, mid, high offset olaraq parçalayaraq), selectoru (`0x08` - Kernel Code) və flag-ları (`0x8E` - Interrupt Gate, present, Ring 0) doldururuq.

### 🔹 PIC Remap (Programmable Interrupt Controller)
IBM PC arxitekturasında standart olaraq aparat kəsilmələri (IRQ) CPU Exception-ları ilə eyni nömrələrə düşür. Toqquşmanın qarşısını almaq üçün `pic_remap` funksiyası vasitəsilə 8259 PIC çiplərinə əmrlər göndəririk:
*   Master PIC-in başlanğıc interrupt nömrəsini `0x20` (32) edirik.
*   Slave PIC-in başlanğıc interrupt nömrəsini `0x28` (40) edirik.
*   Beləliklə, məsələn, Timer (IRQ0) `32`-ci, Klaviatura (IRQ1) isə `33`-cü interrupt kimi gəlir.

### 🔹 Assembly Stub-lar və Ortq Çaqırış
Hər kəsilmə baş verəndə prosessorun registrlərinin vəziyyətini qorumalıyıq. Bunun üçün `gdtasm.s` daxilində makrolar vasitəsilə hər interrupt üçün kiçik stub-lar yaradılır:
*   `ISR_NOERRCODE` və `ISR_ERRCODE`: Səhv kodu olan və olmayan CPU exception-ları üçün.
*   `IRQ`: Aparat kəsilmələri üçün.
Bu stub-lar registrləri staka yığır (`push rax`, `push rbx`...) və ortaq handling funksiyasını çağırır:
*   Exception-lar üçün `isr_handler` çağırılır.
*   Aparat kəsilmələri üçün `irq_handler` çağırılır.
*   İş bitdikdən sonra registrlər stakdan geri oxunur (`pop`) və `iretq` əmri ilə kəsilmədən əvvəlki koda qayıdılır.

Kəsilmələrin C tərəfində dinamik idarə olunması üçün `register_interrupt_handler` funksiyası istifadə olunur.

---

## 📺 4. VGA Text Mode Driver (`src/drivers/vga.c`)

Ekrana yazı yazmaq üçün birbaşa VGA yaddaş sahəsinə (`0xb8000`) müraciət edirik.

*   **Yaddaş Strukturu**: Hər bir simvol ekranda 2 bayt yer tutur. 1-ci bayt ASCII simvol özü, 2-ci bayt isə rəng kodudur (məsələn, `0x0f` — qara fonda ağ yazı).
*   **Kursrun İdarə Edilməsi**: Kursrun yerini dəyişmək və ya onun cari yerini öyrənmək üçün VGA-nın idarəetmə portlarından istifadə edirik:
    *   Control Port: `0x3d4`
    *   Data Port: `0x3d5`
    *   *Qeyd*: `cursor_get` funksiyasında hər simvol 2 bayt yer tutduğu üçün portdan gələn offseti 2-yə vurub qaytarırıq.
*   **Scrolling (Ekranın Sürüşdürülməsi - `scrolln`)**:
    *   Əgər yazdığımız mətn ekran ölçüsünü (25 sətir x 80 sütun) aşarsa, `scrolln` funksiyası işə düşür.
    *   `memorycpy` funksiyası ilə ekranın 2-ci sətirindən etibarən bütün məlumatları 1 sətir yuxarıya kopyalayırıq.
    *   Ən sonuncu sətiri isə boşluq simvolları ilə doldurub təmizləyirik.
*   **Ədədlərin Çap Olunması**:
    *   `puthex`: Verilən 64-bitlik ədədi 16-lıq say sistemində (məsələn, `0x00100000`) ekrana yazdırır.
    *   `putdec`: Ədədləri 10-luq say sistemində ekrana çıxarmağa kömək edir.

---

## 💾 5. Fiziki Yaddaş Meneceri (PMM - Physical Memory Manager) (`src/memory/pmm.c`)

Khazar OS-də fiziki yaddaşın hansı hissələrinin dolu, hansı hissələrinin boş olduğunu idarə etmək üçün **Bitmap** alqoritmindən istifadə olunur.

### 🔹 PMM Bitmap
*   `frame_bitmap` massivi hər bitin bir 4KB-lıq səhifəni (frame) təmsil etdiyi böyük bir xəritədir.
*   Massivin ölçüsü `1024 * 1024 / 8` baytdır. Bu da `256K` frame deməkdir (yəni cəmi 1 GB yaddaşı idarə edə bilirik).
*   `pmm_set_addr(addr)`: Ünvana uyğun gələn biti `1` edir (dolu kimi işarələyir).
*   `pmm_clear(addr)`: Ünvana uyğun gələn biti `0` edir (boşaldır).
*   `pmm_test_addr(addr)`: Səhifənin dolu (`1`) və ya boş (`0`) olmasını yoxlayır.

### 🔹 PMM-in Başlanğıc Tənzimlənməsi (`pmm_init`)
1.  İlk olaraq bütün bitmap-i `0xFF` (yəni bütün yaddaş doludur/əlçatmazdır) olaraq doldururuq.
2.  Daha sonra GRUB-dan gələn **Multiboot Memory Map** strukturunu tarayırıq (`mb->mmap_addr`).
3.  Memory map daxilində tipi `1` (USABLE - istifadəyə yararlı RAM) olan bütün yaddaş sahələrinin bitlərini `pmm_clear` vasitəsilə boş (0) olaraq işarələyirik. Beləliklə, həqiqi RAM sahələrini istifadəyə açırıq.
4.  **Low 1MB Qorunması**: Yaddaşın ilk 1 MB-lıq hissəsində BIOS məlumatları, VGA yaddaşı və kernelimizin özü yerləşdiyi üçün bu sahəni hər zaman dolu olaraq işarələyib qoruyuruq (`pmm_set_addr`).

### 🔹 Allocation və Free
*   `pmm_alloc()`: `pmm_first_free` ilə bitmap-i gəzib ilk tapdığı boş frame-in indeksini tapır. Həmin bit-i `1` edib (dolu kimi işarələyib) fiziki ünvanı qaytarır.
*   `lowest_available` dəyişəni vasitəsilə növbəti axtarış zamanı bitmap-in əvvəlindən başlamırıq, axırıncı dəfə harda qalmışdıqsa oradan davam edərək performansı artırırıq.
*   `pmm_free(addr)`: Ayrılmış fiziki ünvanı yenidən azad edir (bitini 0 edir).

### 🔹 Səhifə Cədvəllərinin Azad Edilməsi (`mmu_free`)
*   Verilmiş PML4 cədvəlindən aşağıya doğru bütün alt cədvəlləri (PDPT, PD, PT) və həmin cədvəllərin özlərinin yerləşdiyi fiziki frame-ləri azad etmək üçün rekursiv döngü ilə yaddaşı təmizləyir.

---

## 🎬 6. Kernel Giriş Nöqtəsi (`src/kernel/kernel.c`)

Hər şey hazır olduqdan sonra `kernel_main` funksiyasında işlərin gedişatı belə olur:

```c
void kernel_main(uint64_t multiboot_addr) {
  clear();              // Ekranı təmizləyirik
  init_gdt();           // GDT qurulur
  idt_init();           // IDT qurulur və PIC remap olunur
  init_keyboard();      // Klaviatura callback-i IRQ1-ə bağlanır
  init_timer(100);      // PIT timer 100 Hz tezliklə işə salınır

  asm volatile("sti");  // CPU interruptlarını rəsmi olaraq aktiv edirik (sti)

  // ... loglar yazdırılır ...

  multiboot_info_t *mb = (multiboot_info_t*)multiboot_addr;
  pmm_init(mb);         // Multiboot memory map ilə PMM aktiv olunur

  // ... Loqo çapı və ekran təmizlənməsi ...

  pmm_stats();          // Yaddaş statistikasını (Total, Used, Free MB) ekrana çıxarırıq

  while (1)             // Sonsuz döngü (Sistem açıq qalır)
    ;
}
```

---

## 🛠 Necə Build edək və Run edək?

Kernelimizi işə salmaq üçün terminalda aşağıdakı `make` əmrlərindən istifadə edirik:

1.  `make` — Kodları kompilyasiya edir və `khazar.iso` faylını yaradır.
2.  `make run` — ISO faylını yaradır və QEMU emulyatorunda işə salır.
3.  `make clean` — Bütün `.o` obyekt fayllarını və ISO-nu təmizləyir.
