// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _SOC_ADDR_MAP_INCLUDE_GUARD_
#define _SOC_ADDR_MAP_INCLUDE_GUARD_

#include <stdint.h>

// Peripheral / cluster base addresses are derived from the SystemRDL-generated
// address map (peakrdl raw-header -> chimera-top/.generated/chimera_addrmap_raw.h),
// resolved via CHIMERA_RDL_GEN_DIR (see this target's CMakeLists.txt). This is
// the single difference from chimera-open's hand-maintained soc_addr_map.h.
#include "chimera_addrmap_raw.h"

#define CLINT_CTRL_BASE    CHIMERA_ADDRMAP_CHESHIRE_CLINT_BASE_ADDR
#define SOC_CTRL_BASE      CHIMERA_ADDRMAP_SOC_CTRL_BASE_ADDR
#define HYPERBUS_CTRL_BASE CHIMERA_ADDRMAP_HYPERBUS_CFG_BASE_ADDR

#define CLUSTER_0_BASE CHIMERA_ADDRMAP_CLUSTER_BASE_ADDR(0)
#define CLUSTER_1_BASE CHIMERA_ADDRMAP_CLUSTER_BASE_ADDR(1)
#define CLUSTER_2_BASE CHIMERA_ADDRMAP_CLUSTER_BASE_ADDR(2)
#define CLUSTER_3_BASE CHIMERA_ADDRMAP_CLUSTER_BASE_ADDR(3)
#define CLUSTER_4_BASE CHIMERA_ADDRMAP_CLUSTER_BASE_ADDR(4)

// --- SoC topology (cores/harts per cluster) — not part of the address map ----
#define HOST_NUMCORES 1
#define CLUSTER_0_NUMCORES 9
#define CLUSTER_1_NUMCORES 9
#define CLUSTER_2_NUMCORES 9
#define CLUSTER_3_NUMCORES 9
#define CLUSTER_4_NUMCORES 9

#define NUM_CLUSTER_CORES \
    (CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES + \
     CLUSTER_4_NUMCORES)

#define NUM_CORES (HOST_NUMCORES + NUM_CLUSTER_CORES)
#define NUM_CLUSTERS 5

#define HOST_HART_BASE 0
#define CLUSTER_0_HART_BASE (HOST_HART_BASE + HOST_NUMCORES)
#define CLUSTER_1_HART_BASE (CLUSTER_0_HART_BASE + CLUSTER_0_NUMCORES)
#define CLUSTER_2_HART_BASE (CLUSTER_1_HART_BASE + CLUSTER_1_NUMCORES)
#define CLUSTER_3_HART_BASE (CLUSTER_2_HART_BASE + CLUSTER_2_NUMCORES)
#define CLUSTER_4_HART_BASE (CLUSTER_3_HART_BASE + CLUSTER_3_NUMCORES)

#define CLUSTER_HART_BASE CLUSTER_0_HART_BASE

#define _chimera_numClusters 5

static const uint8_t _chimera_numCores[] = {CLUSTER_0_NUMCORES, CLUSTER_1_NUMCORES,
                                            CLUSTER_2_NUMCORES, CLUSTER_3_NUMCORES,
                                            CLUSTER_4_NUMCORES};
static const uint8_t _chimera_hartBase[] = {CLUSTER_0_HART_BASE, CLUSTER_1_HART_BASE,
                                            CLUSTER_2_HART_BASE, CLUSTER_3_HART_BASE,
                                            CLUSTER_4_HART_BASE};

extern const uintptr_t _chimera_clusterBase[_chimera_numClusters];

// The padframe and FLL live inside the external-config register window
// (ext_cfg_regs), which the RDL models as a single opaque region.
// TODO(docs/memory-map.md step 4): model padframe/FLL discretely in cfg/rdl.
#ifdef CHIMERA_PADFRAME_BASE_ADDRESS
#undef CHIMERA_PADFRAME_BASE_ADDRESS
#endif
#define CHIMERA_PADFRAME_BASE_ADDRESS CHIMERA_ADDRMAP_EXT_CFG_REGS_BASE_ADDR

#define FLL_BASE_ADDR (CHIMERA_ADDRMAP_EXT_CFG_REGS_BASE_ADDR + 0x1000)

#define HYPERRAM_BASE_ADDR CHIMERA_ADDRMAP_HYPERRAM_BASE_ADDR

#endif
