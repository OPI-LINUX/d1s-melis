/* version.c */

/*
 * Copyright (c) 1997-2010, 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <version.h> /* generated by MAKE, at compile time */

#define KERNELVERSION SDK_GIT_VERSION
static char *kernel_version = KERNELVERSION;

/**
 *
 * @brief Return the kernel version of the present build
 *
 * The kernel version is a four-byte value, whose format is described in the
 * file "kernel_version.h".
 *
 * @return kernel version
 */
char *sys_kernel_version_get(void)
{
    return kernel_version;
}
