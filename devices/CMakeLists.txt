# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Device Libraries
################################################################################
set(DEVICE_MAPPINGS
    chimera-convolve:snitch_cluster
    chimera-open:snitch_cluster
    chimera-gen:snitch_cluster
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Device" "${DEVICE_MAPPINGS}")


