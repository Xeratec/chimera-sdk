// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Ported from chimera-top sw/tests/testCfgBootAddr.c (Lorenzo Leone).
// Verify the configurable Snitch boot-address register: reset value and R/W.

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"

#define TESTVAL 0x00E0D0C0
#define RSTVAL 0x30000000

int main(void) {
    volatile uint32_t *regPtr =
        (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CONFIGURABLE_BOOT_ADDR_REG_OFFSET);

    // Reset value must be the Snitch bootrom base.
    uint32_t rst = *regPtr;
    if (rst != RSTVAL) {
        printf("CONFIGURABLE_BOOT_ADDR reset value FAIL: got 0x%08x, expected 0x%08x\n", rst,
               RSTVAL);
        return 1;
    }

    // Writability check.
    *regPtr = TESTVAL;
    uint32_t rb = *regPtr;
    if (rb != TESTVAL) {
        printf("CONFIGURABLE_BOOT_ADDR write FAIL: got 0x%08x, expected 0x%08x\n", rb, TESTVAL);
        return 2;
    }

    // Restore the reset value so a subsequent cluster boot uses the bootrom.
    *regPtr = RSTVAL;

    printf("CONFIGURABLE_BOOT_ADDR OK (reset 0x%08x, R/W verified)\n", RSTVAL);
    return 0;
}
