<!-- SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Chimera-SDK Tests

Each test is a directory with a `CMakeLists.txt` and `src_host/` (host / Cheshire-CVA6 code),
optionally `src_cluster/` (Snitch device code) and `include/`. Tests are **generic**; which ones
build for a given target is set in [`CMakeLists.txt`](CMakeLists.txt) via `TEST_MAPPINGS`.
Pass/fail is the program's return code (`0` = pass). Build with `make chim-sdk` from the repo root.

## Host tests (`host/`)

| Test | Description |
|------|-------------|
| `returnZero` | Minimal smoke test — returns 0. |
| `printf` | `printf` / stdio over UART. |
| `picolibc` | picolibc (libc) functionality. |
| `alloc` | Host heap allocation. |
| `uartSimple` | Simple UART output. |
| `hyperbus` | Host-side HyperRAM config + read/write, sweeping the clk-delay lines. |
| `peripheralGating` | Cheshire peripheral clock-gating register: reset value + set/clear (ported from chimera-top). |

## Snitch-cluster tests (`snitchCluster/`)

| Test | Description |
|------|-------------|
| `simpleOffload` | Single-cluster offload round-trip (boot addr, CLINT MSIP, return reg). |
| `offloadAll` | Offload to **every** cluster in turn and check each return (ported from chimera-top). |
| `clusterMemory` | Host writes/reads each cluster's TCDM over the narrow AXI (ported). |
| `memoryIsland` | Cluster reaches the memory island via the wide path and the bypassed narrow path; checks `WIDE_MEM_CLUSTER_n_BYPASS` (ported). |
| `bootAddrConfig` | Configurable Snitch boot-address register: reset value + R/W (ported). |
| `clusterGating` | Cluster reset/clock-gate register readback + idle-after-settle busy check (ported, now self-checking). |
| `idma` | Cluster iDMA copy within TCDM, verified by all cores (new; iDMA coverage gap). |
| `hyperbus` | Device-side HyperRAM access from a cluster. |
| `snrt` | SNRT runtime bring-up; cluster output via host-forwarded syscalls. |
| `matmul` | Matrix-multiply kernel on the cluster, checked against golden output. |

## Test / target matrix

`Cv` = chimera-convolve · `Op` = chimera-open · `Ho` = chimera-host.

| Test | Cv | Op | Ho |
|------|:--:|:--:|:--:|
| host/returnZero | ✓ | ✓ | ✓ |
| host/printf | ✓ | ✓ | ✓ |
| host/picolibc | ✓ | ✓ | ✓ |
| host/alloc | ✓ | ✓ | ✓ |
| host/uartSimple | ✓ | ✓ | ✓ |
| host/hyperbus | ✓ | ✓ | ✓ |
| host/peripheralGating | ✓ | ✓ | ✓ |
| snitchCluster/simpleOffload | ✓ | ✓ | |
| snitchCluster/offloadAll | ✓ | ✓ | |
| snitchCluster/clusterMemory | ✓ | ✓ | |
| snitchCluster/memoryIsland | ✓ | ✓ | |
| snitchCluster/bootAddrConfig | ✓ | ✓ | |
| snitchCluster/clusterGating | ✓ | ✓ | |
| snitchCluster/idma | ✓ | ✓ | |
| snitchCluster/hyperbus | ✓ | ✓ | |
| snitchCluster/snrt | ✓ | ✓ | |
| snitchCluster/matmul | ✓ | ✓ | |
