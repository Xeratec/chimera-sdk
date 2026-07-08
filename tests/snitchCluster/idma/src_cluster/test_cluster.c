// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// New test (no chimera-top equivalent): exercise the cluster iDMA. The DMA core
// copies a marked buffer within the cluster TCDM; every core then verifies the
// copy so the return value is race-free (all cores return the same verdict).

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

#define DMA_N 256

// Shared (memory-island) pointers so all cluster cores see the same buffers.
static uint32_t *volatile g_src;
static uint32_t *volatile g_dst;

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

int32_t testDma(void *args __attribute__((unused))) {
    snrt_init();

    // Core 0 allocates the L1 (TCDM) buffers and fills the source.
    if (snrt_cluster_core_idx() == 0) {
        g_src = (uint32_t *)snrt_l1_alloc(DMA_N * sizeof(uint32_t));
        g_dst = (uint32_t *)snrt_l1_alloc(DMA_N * sizeof(uint32_t));
        for (uint32_t i = 0; i < DMA_N; i++) {
            g_src[i] = (uint32_t)(TESTVAL + i);
            g_dst[i] = 0;
        }
    }
    snrt_cluster_hw_barrier();

    // The DMA core copies src -> dst.
    if (snrt_is_dm_core()) {
        snrt_dma_start_1d(g_dst, g_src, DMA_N * sizeof(uint32_t));
        snrt_dma_wait_all();
    }
    snrt_cluster_hw_barrier();

    // Every core verifies (deterministic, identical verdict on all cores).
    int32_t ret = TESTVAL;
    for (uint32_t i = 0; i < DMA_N; i++) {
        if (g_dst[i] != g_src[i]) {
            ret = 0;
            break;
        }
    }
    snrt_cluster_hw_barrier();

    return ret;
}
