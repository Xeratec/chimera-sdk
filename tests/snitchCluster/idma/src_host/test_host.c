// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Host side of the cluster iDMA test: offload the DMA copy to a cluster and
// check it reports success.

// Include Application Headers
#include "test_host.h"

#include "test_snitchCluster_idma_device_symbols.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"
#include "shared.h"

#define CLUSTER 0
#define STACK_ADDRESS (_chimera_clusterBase[CLUSTER] + 0x20000 - 1)

int main(void) {
    void *stack_cluster_ptr[NUM_CLUSTER_CORES];
    generate_snitchCluster_SPs_uniform(CLUSTER, (void *)STACK_ADDRESS, 0x2000, stack_cluster_ptr);

    setup_snitchCluster_interruptHandler(device_clusterInterruptHandler);

    set_snitchCluster_clockGating(CLUSTER, 0);
    set_snitchCluster_reset(CLUSTER, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(CLUSTER, 0);

    printf_log("Waiting for cluster DMA...\n");
    offload_snitchCluster(device_testDma, device_trampoline, NULL, stack_cluster_ptr, CLUSTER);
    uint32_t retVal = wait_snitchCluster_return(CLUSTER);

    set_snitchCluster_clockGating(CLUSTER, 1);

    uint32_t expected = (TESTVAL | 0x1);
    printf("DMA test returned 0x%08x, expected 0x%08x\n", retVal, expected);

    return (retVal != expected);
}
