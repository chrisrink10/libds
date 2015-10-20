cmake_minimum_required(VERSION 3.3)
project(libds)

# Enable CUnit testing
enable_testing()
find_library(LIB_CUNIT cunit)

# Build flags
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/")
set(CMAKE_C_FLAGS "-std=c99 -g -Wall -Werror")

# Build the library
include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_HEADER_FILES include/libds/buffer.h
                         include/libds/dict.h
                         include/libds/iter.h
                         include/libds/list.h)
set(LIBRARY_SOURCE_FILES src/buffer.c
                         src/dict.c
                         src/iter.c
                         src/list.c)
add_library(libds ${LIBRARY_SOURCE_FILES})

# Build the test target
include_directories(/usr/local/opt/cunit/include)
set(TEST_SOURCE_FILES test/dslib_test.c
                      test/main_test.c)
add_executable(libds_test ${TEST_SOURCE_FILES})
target_link_libraries(libds_test libds)
target_link_libraries(libds_test ${LIB_CUNIT})

# Install the library header files
if (NOT DEBUG)
    install(FILES ${LIBRARY_HEADER_FILES}
            DESTINATION "/usr/include/libds")
endif(NOT DEBUG)