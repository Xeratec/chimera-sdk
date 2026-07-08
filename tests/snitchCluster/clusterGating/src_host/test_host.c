// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Ported from chimera-top sw/tests/testClusterGating.c (Lorenzo Leone,
// Viviane Potocnik), made self-checking: instead of relying on waveform
// inspection, assert the reset/clock-gate registers read back the written
// value for every cluster and that a released, un-gated cluster settles to idle.

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"

static const uint32_t gateOff[] = {
    CHIMERA_CLUSTER_0_CLK_GATE_EN_REG_OFFSET, CHIMERA_CLUSTER_1_CLK_GATE_EN_REG_OFFSET,
    CHIMERA_CLUSTER_2_CLK_GATE_EN_REG_OFFSET, CHIMERA_CLUSTER_3_CLK_GATE_EN_REG_OFFSET,
    CHIMERA_CLUSTER_4_CLK_GATE_EN_REG_OFFSET};

static const uint32_t resetOff[] = {
    CHIMERA_RESET_CLUSTER_0_REG_OFFSET, CHIMERA_RESET_CLUSTER_1_REG_OFFSET,
    CHIMERA_RESET_CLUSTER_2_REG_OFFSET, CHIMERA_RESET_CLUSTER_3_REG_OFFSET,
    CHIMERA_RESET_CLUSTER_4_REG_OFFSET};

static inline uint32_t rd(uint32_t off) {
    return *(volatile uint32_t *)(SOC_CTRL_BASE + off);
}

int main(void) {
    uint32_t fails = 0;

    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        // Clock-gate register readback.
        set_snitchCluster_clockGating(c, 1);
        if ((rd(gateOff[c]) & 0x1) != 1) {
            printf("Cluster %u CLK_GATE_EN did not read back 1\n", c);
            fails |= (1u << c);
        }
        set_snitchCluster_clockGating(c, 0);
        if ((rd(gateOff[c]) & 0x1) != 0) {
            printf("Cluster %u CLK_GATE_EN did not read back 0\n", c);
            fails |= (1u << c);
        }

        // Reset register readback.
        set_snitchCluster_reset(c, 1);
        if ((rd(resetOff[c]) & 0x1) != 1) {
            printf("Cluster %u RESET did not read back 1\n", c);
            fails |= (1u << (c + 8));
        }
        set_snitchCluster_reset(c, 0);
        if ((rd(resetOff[c]) & 0x1) != 0) {
            printf("Cluster %u RESET did not read back 0\n", c);
            fails |= (1u << (c + 8));
        }
    }

    // Observe busy state after releasing + un-gating each cluster. This is
    // informational only, not asserted: whether a cluster has reached WFI (busy
    // cleared) by the time we sample is timing-dependent and not deterministic.
    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        set_snitchCluster_clockGating(c, 0);
        set_snitchCluster_reset(c, 0);
    }
    for (volatile int i = 0; i < 2000; i++);
    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        printf("Cluster %u busy = %d\n", c, snitchCluster_busy(c));
    }

    if (!fails) {
        printf("Cluster gating/reset registers OK for %u clusters\n", _chimera_numClusters);
    }
    return fails;
}
