// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Ported from chimera-top sw/tests/testCluster.c
// (Moritz Scherer, Viviane Potocnik).
//
// Host-side check that every Snitch cluster's TCDM is reachable over the narrow
// AXI: write a marker to the base of each cluster's memory and read it back.

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"

#define TESTVAL 0x00E0D0C0

int main(void) {
    // Bring every cluster out of reset and un-gate its clock so its TCDM is
    // accessible from the host over the narrow interconnect.
    setAll_snitchCluster_reset(0);
    setAll_snitchCluster_clockGating(0);

    uint32_t fails = 0;

    // Write phase.
    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        *(volatile uint32_t *)_chimera_clusterBase[c] = TESTVAL;
    }

    // Read-back phase.
    for (uint32_t c = 0; c < _chimera_numClusters; c++) {
        uint32_t got = *(volatile uint32_t *)_chimera_clusterBase[c];
        if (got != TESTVAL) {
            printf("Cluster %u TCDM mismatch: got 0x%08x, expected 0x%08x\n", c, got, TESTVAL);
            fails |= (1u << c);
        } else {
            printf("Cluster %u TCDM OK (0x%08x)\n", c, got);
        }
    }

    return fails;
}
