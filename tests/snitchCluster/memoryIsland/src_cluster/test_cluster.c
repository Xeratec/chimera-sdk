// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <string.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "trampoline.h"

// Include Runtime Headers
#include "snrt.h"

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) void clusterInterruptHandler() {
    _SET_CLUSTER_BUSY();
    _SETUP_GP();

    asm volatile(
        "csrr t0, mhartid\n"
        "la t1, __base_clint\n"
        "slli t0, t0, 2\n"
        "add t1, t1, t0\n"
        "sw zero, 0(t1)\n"
        "ret"
        :
        :
        : "t0", "t1");
}

// Offloaded functions live in the memory island; the cluster fetches them over
// its wide port, which is routed either directly to the memory island (wide) or
// re-routed to the narrow AXI (bypass), depending on WIDE_MEM_CLUSTER_n_BYPASS.
// The functions are 32B-aligned so consecutive calls force fresh fetches, and
// each returns a distinct marker so the host can tell which path executed.

int32_t __attribute__((aligned(32))) testMemNarrow(void *args __attribute__((unused))) {
    return TESTNARROW;
}

int32_t __attribute__((aligned(32))) testMemWide(void *args __attribute__((unused))) {
    return TESTWIDE;
}
