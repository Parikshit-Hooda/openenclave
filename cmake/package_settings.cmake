# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.
#
# Set default paths
# TODO: See #757: Actually use GNUInstallDirs and don't hard-code our
# own paths.

# Set the default install prefix for Open Enclave. One may override this value
# with the cmake command. For example:
#
#     $ cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/myplace ..
#
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
    "/opt/openenclave" CACHE PATH "default install prefix" FORCE)
endif()

include(GNUInstallDirs)
set(OE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/output CACHE INTERNAL "Path to the intermittent collector tree")
set(OE_BINDIR ${OE_OUTPUT_DIR}/bin CACHE INTERNAL "Binary collector")
set(OE_DATADIR ${OE_OUTPUT_DIR}/share CACHE INTERNAL "Data collector root")
set(OE_DOCDIR ${OE_OUTPUT_DIR}/share/doc CACHE INTERNAL "Doc collector root")
set(OE_INCDIR ${OE_OUTPUT_DIR}/include CACHE INTERNAL "Include collector")
set(OE_LIBDIR ${OE_OUTPUT_DIR}/lib CACHE INTERNAL "Library collector")

# Generate and install CMake export file for consumers using Cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/openenclave-config.cmake.in
  ${CMAKE_BINARY_DIR}/cmake/openenclave-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/cmake)
write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/cmake/openenclave-config-version.cmake
  COMPATIBILITY SameMajorVersion)
install(
  FILES ${CMAKE_BINARY_DIR}/cmake/openenclave-config.cmake
  ${CMAKE_BINARY_DIR}/cmake/openenclave-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/cmake)
install(
  EXPORT openenclave-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/cmake
  FILE openenclave-targets.cmake)
install(
  FILES ${PROJECT_SOURCE_DIR}/cmake/sdk_cmake_targets_readme.md
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/cmake
  RENAME README.md)

# CPack package handling
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "open-enclave")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open Enclave SDK")
set(CPACK_PACKAGE_CONTACT "openenclave@microsoft.com")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION ${OE_VERSION})
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsgx-enclave-common (>=2.3.100.46354-1), libsgx-enclave-common-dev (>=2.3.100.0-1), libsgx-dcap-ql (>=1.0.100.46460-1.0), libsgx-dcap-ql-dev (>=1.0.100.46460-1.0), pkg-config")
include(CPack)

# Generate the openenclaverc script.
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/openenclaverc.in
    ${CMAKE_BINARY_DIR}/output/share/openenclaverc
    @ONLY)

# Install the openenclaverc script.
install(FILES
    ${CMAKE_BINARY_DIR}/output/share/openenclaverc
    DESTINATION
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}")
