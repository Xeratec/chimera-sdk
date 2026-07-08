// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Ported from chimera-top sw/tests/testClusterOffload.c.
// Offload the same function to every Snitch cluster in turn and verify each
// returns the expected marker, exercising every cluster's offload plumbing
// (boot address, CLINT MSIP, return register, busy handshake).

// Include Application Headers
#include "test_host.h"

#include "test_snitchCluster_offloadAll_device_symbols.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"
#include "shared.h"

static offloadArgs_t offloadArgs = {.value = 0xdeadbeef};

int main(void) {
    setup_snitchCluster_interruptHandler(device_clusterInterruptHandler);

    uint32_t fails = 0;

    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        void *stack_cluster_ptr[NUM_CLUSTER_CORES];
        void *stack_top = (void *)(_chimera_clusterBase[c] + 0x20000 - 1);
        generate_snitchCluster_SPs_uniform(c, stack_top, 0x2000, stack_cluster_ptr);

        set_snitchCluster_clockGating(c, 0);
        set_snitchCluster_reset(c, 1);
        for (volatile int i = 0; i < 10; i++);
        set_snitchCluster_reset(c, 0);

        offload_snitchCluster(device_testReturn, device_trampoline, &offloadArgs, stack_cluster_ptr,
                              c);
        uint32_t retVal = wait_snitchCluster_return(c);

        set_snitchCluster_clockGating(c, 1);

        uint32_t expected = (TESTVAL | 0x1);
        if (retVal != expected) {
            printf("Cluster %u offload FAIL: got 0x%08x, expected 0x%08x\n", c, retVal, expected);
            fails |= (1u << c);
        } else {
            printf("Cluster %u offload OK (0x%08x)\n", c, retVal);
        }
    }

    return fails;
}
