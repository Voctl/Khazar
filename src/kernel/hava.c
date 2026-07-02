#include "../include/sound.h"
#include "../include/pit/pit.h"

// Megalovania üçün dəqiq not tezlikləri
#define NOTE_D5  587
#define NOTE_D6  1175
#define NOTE_A5  880
#define NOTE_G_SHARP5 831
#define NOTE_G5  784
#define NOTE_F5  698
#define NOTE_C5  523
#define NOTE_B4  494
#define NOTE_A_SHARP4 466

// Megalovania-nın sürətli ritm intervalları
#define T_SHORT 110  // Sürətli notlar üçün (Səkkizlik)
#define T_MID   220  // Normal notlar üçün (Dördlük)
#define T_LONG  440  // Uzun notlar üçün

void play_hava_nagila(void)
{
    // --- 1. RUND (D5 notu ilə başlayan əsas mövzu) ---
    plays(NOTE_D5); sleep(T_SHORT);
    plays(NOTE_D5); sleep(T_SHORT);
    plays(NOTE_D6); sleep(T_MID);
    plays(NOTE_A5); sleep(T_MID + T_SHORT);
    
    plays(NOTE_G_SHARP5); sleep(T_MID);
    plays(NOTE_G5);       sleep(T_MID);
    plays(NOTE_F5);       sleep(T_MID);
    plays(NOTE_D5);       sleep(T_SHORT);
    plays(NOTE_F5);       sleep(T_SHORT);
    plays(NOTE_G5);       sleep(T_SHORT);

    // --- 2. RUND (C5 notu ilə başlayan keçid) ---
    plays(NOTE_C5); sleep(T_SHORT);
    plays(NOTE_C5); sleep(T_SHORT);
    plays(NOTE_D6); sleep(T_MID);
    plays(NOTE_A5); sleep(T_MID + T_SHORT);
    
    plays(NOTE_G_SHARP5); sleep(T_MID);
    plays(NOTE_G5);       sleep(T_MID);
    plays(NOTE_F5);       sleep(T_MID);
    plays(NOTE_D5);       sleep(T_SHORT);
    plays(NOTE_F5);       sleep(T_SHORT);
    plays(NOTE_G5);       sleep(T_SHORT);

    // --- 3. RUND (B4 notu ilə başlayan keçid) ---
    plays(NOTE_B4); sleep(T_SHORT);
    plays(NOTE_B4); sleep(T_SHORT);
    plays(NOTE_D6); sleep(T_MID);
    plays(NOTE_A5); sleep(T_MID + T_SHORT);
    
    plays(NOTE_G_SHARP5); sleep(T_MID);
    plays(NOTE_G5);       sleep(T_MID);
    plays(NOTE_F5);       sleep(T_MID);
    plays(NOTE_D5);       sleep(T_SHORT);
    plays(NOTE_F5);       sleep(T_SHORT);
    plays(NOTE_G5);       sleep(T_SHORT);

    // --- 4. RUND (A_SHARP4 ilə bitiş) ---
    plays(NOTE_A_SHARP4); sleep(T_SHORT);
    plays(NOTE_A_SHARP4); sleep(T_SHORT);
    plays(NOTE_D6);       sleep(T_MID);
    plays(NOTE_A5);       sleep(T_MID + T_SHORT);
    
    plays(NOTE_G_SHARP5); sleep(T_MID);
    plays(NOTE_G5);       sleep(T_MID);
    plays(NOTE_F5);       sleep(T_MID);
    plays(NOTE_D5);       sleep(T_SHORT);
    plays(NOTE_F5);       sleep(T_SHORT);
    plays(NOTE_G5);       sleep(T_SHORT);
}
