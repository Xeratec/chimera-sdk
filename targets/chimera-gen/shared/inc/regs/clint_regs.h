// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// Generated-backed shim: the CLINT register definitions come from the clint IP's
// reggen output (regtool --cdefines -> .generated/clint.h, `make rdl`), resolved
// via CHIMERA_RDL_GEN_DIR. That header is generated with the real core count
// (NrCores=46), so per-hart MSIP is a multireg: CLINT_MSIP_0, CLINT_MSIP_1, ...
// The SDK driver treats MSIP as a single base register + hartid stride, so we
// alias the legacy unindexed name to core 0. The MTIME / MTIMECMP macro names
// already match the generated header and are used directly from it.

#ifndef _CLINT_REGS_SHIM_H_
#define _CLINT_REGS_SHIM_H_

// Note: named clint_hw_regs.h (not clint.h) to avoid colliding with the SDK's
// own clint driver API header host/runtime/inc/clint.h.
#include "clint_hw_regs.h"

#define CLINT_MSIP_REG_OFFSET CLINT_MSIP_0_REG_OFFSET

#endif // _CLINT_REGS_SHIM_H_
