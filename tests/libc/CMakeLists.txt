# add a test-case for each file listed in tests.supported
include(${CMAKE_CURRENT_LIST_DIR}/get_testcase_name.cmake)

# read tests.supported, sanitize the c-file, and create the test-case
file(STRINGS "tests.supported" alltests)
foreach(testcase ${alltests})
    get_testcase_name(${testcase} name)
    add_test(tests/libc/${name} host/libchost ./enc/libctest-${name}.signed.so)
endforeach(testcase)

add_subdirectory(enc)
add_subdirectory(host)