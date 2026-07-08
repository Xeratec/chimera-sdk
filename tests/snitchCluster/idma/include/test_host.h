// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_HOST_INCLUDE_GUARD_
#define _TEST_HOST_INCLUDE_GUARD_

// Success marker returned by the cluster (bit 0 = 0; the offload path OR's in
// the done-flag bit, so the host compares against TESTVAL | 1).
#define TESTVAL 0x0D3AC0DE

#endif //_TEST_HOST_INCLUDE_GUARD_
