# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_SYSTEM_NAME Generic)

set(LLVM_TAG llvm)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/clang)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/clang++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_DIR}/clang)

set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/${LLVM_TAG}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIR}/${LLVM_TAG}-objdump)
set(CMAKE_AR ${TOOLCHAIN_DIR}/${LLVM_TAG}-ar)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=riscv32-unknown-elf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=riscv32-unknown-elf")