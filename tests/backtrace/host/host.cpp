// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/host.h>
#include <openenclave/internal/elf.h>
#include <openenclave/internal/error.h>
#include <openenclave/internal/tests.h>
#include <openenclave/internal/tests.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../args.h"

const char* arg0;

int main(int argc, const char* argv[])
{
    arg0 = argv[0];
    oe_result_t r;
    oe_enclave_t* enclave = NULL;
    const oe_enclave_type_t type = OE_ENCLAVE_TYPE_SGX;
    const uint32_t flags = oe_get_create_flags();

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s ENCLAVE\n", argv[0]);
        exit(1);
    }

    r = oe_create_enclave(argv[1], type, flags, NULL, 0, NULL, 0, &enclave);
    OE_TEST(r == OE_OK);

    /* Test() */
    {
        static const char* syms[] = {
            "GetBacktrace",
            "Test",
            "_handle_call_enclave",
            "_handle_ecall",
            "__oe_handle_main",
            "oe_enter",
        };
        Args args;
        args.syms = syms;
        args.num_syms = OE_COUNTOF(syms);
        args.okay = false;
        r = oe_call_enclave(enclave, "Test", &args);
        OE_TEST(r == OE_OK);

        if (!args.okay)
        {
            fprintf(stderr, "%s: backtrace failed: Test()\n", argv[0]);
            exit(1);
        }
    }

    /* TestUnwind() */
    {
        static const char* syms[] = {
            "func4",
            "func3",
            "func2",
            "func1",
            "TestUnwind",
            "_handle_call_enclave",
            "_handle_ecall",
            "__oe_handle_main",
            "oe_enter",
        };
        Args args;
        args.syms = syms;
        args.num_syms = OE_COUNTOF(syms);
        args.okay = false;
        r = oe_call_enclave(enclave, "TestUnwind", &args);
        OE_TEST(r == OE_OK);

        if (!args.okay)
        {
            fprintf(stderr, "%s: backtrace failed: TestUnwind()\n", argv[0]);
            exit(1);
        }
    }

    r = oe_terminate_enclave(enclave);
    OE_TEST(r == OE_OK);

    printf("=== passed all tests (%s)\n", argv[0]);

    return 0;
}
