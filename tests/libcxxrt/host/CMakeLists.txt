# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_executable(libcxxrt_host host.cpp)

target_link_libraries(libcxxrt_host oehostapp)
