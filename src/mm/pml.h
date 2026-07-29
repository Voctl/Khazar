#pragma once

#include "types.h"

union PML { // PML schedula (512 entries)
  struct {
    U64 present : 1;
    U64 writable : 1;
    U64 user : 1;
    U64 writethrough : 1;
    U64 nocache : 1;
    U64 accessed : 1;
    U64 _available1 : 1;
    U64 size : 1;
    U64 global : 1;
    U64 cow_pending : 1;
    U64 _available2 : 2;
    U64 page : 40;
    U64 reserved : 11;
    U64 nx : 1;
  } bits;
  U64 raw;
};

// Ring 0-only: no user/kernel page distinction needed
