# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(oeenclave STATIC
    ../common/asn1.c
    ../common/cert.c
    ../common/datetime.c
    ../common/quote.c
    ../common/report.c
    ../common/revocation.c
    ../common/sgxcertextensions.c
    ../common/tcbinfo.c
    asn1.c
    cert.c
    crl.c
    ec.c
    cmac.c
    key.c
    link.c
    random.c
    report.c
    revocationinfo.c
    rsa.c
    sha.c
    start.S)

if(USE_LIBSGX)
target_compile_definitions(oeenclave PUBLIC OE_USE_LIBSGX)
endif()    

target_link_libraries(oeenclave PUBLIC
    mbedcrypto
    oelibc)

set_property(TARGET oeenclave PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${OE_LIBDIR}/openenclave/enclave)

install(TARGETS oeenclave ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/enclave)
