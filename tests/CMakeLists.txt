# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# Define mappings for tests
set(TEST_MAPPINGS
[[
chimera-convolve:
  chimera-convolve,
  host/alloc,
  host/picolibc,
  host/printf,
  host/returnZero,
  host/uartSimple,
  host/hyperbus,
  host/peripheralGating
  snitchCluster/matmul,
  snitchCluster/snrt,
  snitchCluster/simpleOffload,
  snitchCluster/hyperbus,
  snitchCluster/clusterMemory,
  snitchCluster/offloadAll,
  snitchCluster/bootAddrConfig,
  snitchCluster/clusterGating,
  snitchCluster/memoryIsland,
  snitchCluster/idma
]]
[[
chimera-open:
  chimera-open,
  host/alloc,
  host/picolibc,
  host/printf,
  host/returnZero,
  host/uartSimple,
  host/hyperbus,
  host/peripheralGating,
  snitchCluster/matmul,
  snitchCluster/snrt,
  snitchCluster/simpleOffload,
  snitchCluster/hyperbus,
  snitchCluster/clusterMemory,
  snitchCluster/offloadAll,
  snitchCluster/bootAddrConfig,
  snitchCluster/clusterGating,
  snitchCluster/memoryIsland,
  snitchCluster/idma
]]
[[
chimera-host:
  chimera-host,
  host/alloc,
  host/picolibc,
  host/printf,
  host/returnZero,
  host/uartSimple,
  host/hyperbus,
  host/peripheralGating
]]
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Test" "${TEST_MAPPINGS}")

