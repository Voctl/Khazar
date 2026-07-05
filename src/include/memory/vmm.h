#pragma once

// the start of the high disk memory
#define HDDM_OFFSET 0xFFFF80000000
// converts a physical address to a virtual address
#define PHY_T_VRT(phys) ((void*)((uint64_t)(phys) + HDDM_OFFSET))
// converts a virtual address to a physical address
#define VRT_T_PHY(virt) ((uint64_t)(virt) - HDDM_OFFSET)
