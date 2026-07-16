// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Ported from chimera-top sw/tests/testPeripheralsGating.c (Lorenzo Leone,
// Viviane Potocnik), made self-checking. Verifies the Cheshire peripheral
// clock-gating register can be driven: reset value, set the low bits, clear
// them. Host-only (no clusters) so it runs on both chimera-open and chimera-gen.

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers
#include "soc.h"

// Include Runtime Headers
#include "log.h"

// Cheshire register block base (peripheral clock-gating lives here).
#define CHESHIRE_REGS_BASE 0x03000000
// UART|I2C|SPIH|SLINK|GPIO|VGA|USB gate-enable bits (7 LSBs).
#define GATE_MASK 0x0000007F

int main(void) {
    volatile uint32_t *regPtr =
        (volatile uint32_t *)(CHESHIRE_REGS_BASE + CHESHIRE_CLK_GATE_EN_PERIPHERALS_REG_OFFSET);

    // Reset value must be 0 (nothing gated).
    if (*regPtr != 0) {
        printf("CLK_GATE_EN_PERIPHERALS reset value FAIL: 0x%08x\n", *regPtr);
        return 1;
    }

    // Enable clock gating on all peripherals.
    *regPtr |= GATE_MASK;
    if ((*regPtr & GATE_MASK) != GATE_MASK) {
        printf("CLK_GATE_EN_PERIPHERALS set FAIL: 0x%08x\n", *regPtr);
        return 2;
    }

    // Disable it again.
    *regPtr &= ~GATE_MASK;
    if ((*regPtr & GATE_MASK) != 0) {
        printf("CLK_GATE_EN_PERIPHERALS clear FAIL: 0x%08x\n", *regPtr);
        return 3;
    }

    printf("CLK_GATE_EN_PERIPHERALS OK (reset/set/clear verified)\n");
    return 0;
}
