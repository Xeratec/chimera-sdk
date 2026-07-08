// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Ported from chimera-top sw/tests/testMemBypass.c (Lorenzo Leone,
// Viviane Potocnik). Verifies clusters can reach the memory island over both
// the wide interconnect and the (bypassed) narrow AXI:
//   1. WIDE_MEM_CLUSTER_n_BYPASS reset value + writability, all clusters.
//   2. Offload through the narrow path (bypass=1) and the wide path (bypass=0),
//      each returning a distinct marker so the executed path is identifiable.
//
// Note: the original set the bypass register of a different cluster than the one
// it offloaded to; this port keeps the bypass and offload on the *same* cluster,
// which is what actually exercises that cluster's fetch path.

// Include Application Headers
#include "test_host.h"

#include "test_snitchCluster_memoryIsland_device_symbols.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"
#include "shared.h"

static offloadArgs_t offloadArgs = {.value = 0xdeadbeef};

static const uint32_t bypassOff[] = {
    CHIMERA_WIDE_MEM_CLUSTER_0_BYPASS_REG_OFFSET, CHIMERA_WIDE_MEM_CLUSTER_1_BYPASS_REG_OFFSET,
    CHIMERA_WIDE_MEM_CLUSTER_2_BYPASS_REG_OFFSET, CHIMERA_WIDE_MEM_CLUSTER_3_BYPASS_REG_OFFSET,
    CHIMERA_WIDE_MEM_CLUSTER_4_BYPASS_REG_OFFSET};

static inline volatile uint32_t *bypassReg(uint32_t c) {
    return (volatile uint32_t *)(SOC_CTRL_BASE + bypassOff[c]);
}

// Offload `fn` to cluster `c` with bypass configured, return the cluster's value.
static uint32_t offloadWithBypass(void *fn, uint32_t c, uint32_t bypass) {
    *bypassReg(c) = bypass;

    void *stack_cluster_ptr[NUM_CLUSTER_CORES];
    void *stack_top = (void *)(_chimera_clusterBase[c] + 0x20000 - 1);
    generate_snitchCluster_SPs_uniform(c, stack_top, 0x2000, stack_cluster_ptr);

    set_snitchCluster_clockGating(c, 0);
    set_snitchCluster_reset(c, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(c, 0);

    offload_snitchCluster(fn, device_trampoline, &offloadArgs, stack_cluster_ptr, c);
    uint32_t ret = wait_snitchCluster_return(c);

    set_snitchCluster_clockGating(c, 1);
    return ret;
}

int main(void) {
    setAll_snitchCluster_reset(0);
    setAll_snitchCluster_clockGating(0);

    // 1. Bypass register reset value + writability for every cluster.
    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        if ((*bypassReg(c) & 0x1) != 0) {
            printf("Cluster %u BYPASS reset value != 0\n", c);
            return 1;
        }
        *bypassReg(c) = 1;
        if ((*bypassReg(c) & 0x1) != 1) {
            printf("Cluster %u BYPASS not writable\n", c);
            return 2;
        }
    }

    setup_snitchCluster_interruptHandler(device_clusterInterruptHandler);

    // The return register carries the cluster value OR'd with the done-flag bit
    // 0, so compare against MARKER | 1 (both markers already have bit 0 set).

    // 2a. Narrow path (bypass = 1) on cluster 0.
    uint32_t retNarrow = offloadWithBypass(device_testMemNarrow, 0, 1);
    if (retNarrow != (TESTNARROW | 0x1)) {
        printf("Narrow path FAIL: got 0x%08x, expected 0x%08x\n", retNarrow, (TESTNARROW | 0x1));
        return 3;
    }

    // 2b. Wide path (bypass = 0) on cluster 1.
    uint32_t retWide = offloadWithBypass(device_testMemWide, 1, 0);
    if (retWide != (TESTWIDE | 0x1)) {
        printf("Wide path FAIL: got 0x%08x, expected 0x%08x\n", retWide, (TESTWIDE | 0x1));
        return 4;
    }

    printf("Memory island narrow + wide paths OK\n");
    return 0;
}
