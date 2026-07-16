// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Generated-backed shim: the Cheshire register-file layout comes from Cheshire's
// SystemRDL (peakrdl c-header -> .generated/cheshire.h, `make rdl`), resolved via
// CHIMERA_RDL_GEN_DIR. The flat CHESHIRE_*_REG_OFFSET names the SDK uses are
// derived here from the generated cheshire__regs_t struct via offsetof, so they
// track the RTL. (Only the offsets actually referenced by the SDK are provided;
// the modern cheshire.h no longer exposes clk_gate_en_peripherals.)

#ifndef _CHESHIRE_REG_DEFS_
#define _CHESHIRE_REG_DEFS_

#include <stddef.h>

#include "cheshire.h"

#define CHESHIRE_SCRATCH_0_REG_OFFSET offsetof(cheshire__regs_t, scratch[0])
#define CHESHIRE_RTC_FREQ_REG_OFFSET  offsetof(cheshire__regs_t, rtc_freq)

#endif // _CHESHIRE_REG_DEFS_
