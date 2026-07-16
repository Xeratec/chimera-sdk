# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(AVAILABLE_TARGETS
  "chimera-gen"
  "chimera-open"
  "chimera-convolve"
  CACHE STRING "Available Targets"
)

get_property(OPT_STRINGS CACHE OPT PROPERTY STRINGS)

if(NOT TARGET_PLATFORM IN_LIST AVAILABLE_TARGETS)
  message(FATAL_ERROR "[CHIMERA-SDK] Wrong value for TARGET_PLATFORM: Got ${TARGET_PLATFORM}. Available targets are: ${AVAILABLE_TARGETS}")
endif()

message(STATUS "[CHIMERA-SDK] TARGET_PLATFORM       : ${TARGET_PLATFORM}")

if(TARGET_PLATFORM STREQUAL "chimera-gen")
  add_compile_definitions(TARGET_PLATFORM_CHIMERA_GEN)
  add_subdirectory(chimera-gen)
endif()

if(TARGET_PLATFORM STREQUAL "chimera-open")
  add_compile_definitions(TARGET_PLATFORM_CHIMERA_OPEN)
  add_subdirectory(chimera-open)
endif()

if(TARGET_PLATFORM STREQUAL "chimera-convolve")
  add_compile_definitions(TARGET_PLATFORM_CHIMERA_CONVOLVE)
  add_subdirectory(chimera-convolve)
endif()
