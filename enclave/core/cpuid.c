// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "cpuid.h"
#include <openenclave/bits/safecrt.h>
#include <openenclave/enclave.h>
#include <openenclave/internal/calls.h>
#include <openenclave/internal/cpuid.h>
#include <openenclave/internal/enclavelibc.h>
#include <openenclave/internal/raise.h>

static uint32_t _oe_cpuid_table[OE_CPUID_LEAF_COUNT][OE_CPUID_REG_COUNT];

/*
**==============================================================================
**
** oe_initialize_cpuid()
**
**     Initialize the enclave view of CPUID information as provided by host
**     during _initialize_enclave call as part of oe_create_enclave.
**
**==============================================================================
*/
oe_result_t oe_initialize_cpuid(uint64_t arg_in)
{
    oe_result_t result = OE_UNEXPECTED;
    oe_init_enclave_args_t* args = (oe_init_enclave_args_t*)arg_in;
    if (args != NULL)
    {
        // Abort the enclave if AESNI support is not present in the cached
        // CPUID Feature information (cpuid leaf of 1)
        if (!(args->cpuid_table[1][OE_CPUID_RCX] & OE_CPUID_AESNI_FEATURE))
            oe_abort();

        OE_CHECK(
            oe_memcpy_s(
                _oe_cpuid_table,
                OE_CPUID_LEAF_COUNT * OE_CPUID_REG_COUNT *
                    sizeof(_oe_cpuid_table[0][0]),
                args->cpuid_table,
                OE_CPUID_LEAF_COUNT * OE_CPUID_REG_COUNT *
                    sizeof(args->cpuid_table[0][0])));

        result = OE_OK;
    }

done:
    return result;
}

/*
**==============================================================================
**
** oe_emulate_cpuid()
**
**     Emulate the result of a CPUID call from within the enclave by returning
**     results initialized by the host during enclave creation.
**
**     This approach allows enclave app code a consistent view of untrusted
**     CPUID results compared to a per callout approach, but limits the extent
**     of emulation to only the cached CPUID leaves, which currently only
**     includes up to structured extended information. Specifically for leaf 4,
**     only subleaf 0 i.e. topology of processor cores in a physical package
**     is cached. Deterministic Cache Parameters for all levels of the
**     processor cache are not supported.
**
**     This primarily allows checking of CPU feature bits important for compat
*      and crypto.
**
**     Returns 0 if referenced CPUID leaf (and subleaf) is available, -1
*      otherwise.
**     For CPUID leaf 4, subleaf of 0 is only available as noted above.
**==============================================================================
*/
int oe_emulate_cpuid(uint64_t* rax, uint64_t* rbx, uint64_t* rcx, uint64_t* rdx)
{
    // upper bits zeroed on 64-bit for CPUID
    uint32_t cpuid_leaf = (*rax) & 0xFFFFFFFF;
    uint32_t cpuid_sub_leaf = (*rcx) & 0xFFFFFFFF;

    if (cpuid_leaf < OE_CPUID_LEAF_COUNT)
    {
        // For leaf 4 of cpuid, only subleaf of 0 is emulated
        if ((cpuid_leaf == 4) && (cpuid_sub_leaf != 0))
            return -1;

        *rax = _oe_cpuid_table[cpuid_leaf][OE_CPUID_RAX];
        *rbx = _oe_cpuid_table[cpuid_leaf][OE_CPUID_RBX];
        *rcx = _oe_cpuid_table[cpuid_leaf][OE_CPUID_RCX];
        *rdx = _oe_cpuid_table[cpuid_leaf][OE_CPUID_RDX];
        return 0;
    }
    return -1;
}
