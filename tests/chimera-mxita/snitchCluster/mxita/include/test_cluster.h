// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_CLUSTER_INCLUDE_GUARD_
#define _TEST_CLUSTER_INCLUDE_GUARD_

#include <stdint.h>

void clusterInterruptHandler();

int32_t testReturn(void *args);

int32_t testOtherCluster(void *args);

#endif //_TEST_CLUSTER_INCLUDE_GUARD_
