cmake_minimum_required(VERSION 3.3)
project(libds)

# Enable CUnit testing
enable_testing()
find_library(LIB_CUNIT cunit)

# Find Doxygen library so we can make documentation
find_package(Doxygen)

# Build flags
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/")
set(CMAKE_C_FLAGS "-std=c99 -g -Wall -Werror")

# Build the library
include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_HEADER_FILES include/libds/buffer.h
                         include/libds/dict.h
                         include/libds/hash.h
                         include/libds/iter.h
                         include/libds/list.h)
set(LIBRARY_SOURCE_FILES src/buffer.c
                         src/dict.c
                         src/hash.c
                         src/iter.c
                         src/list.c)
add_library(libds ${LIBRARY_SOURCE_FILES})

# Build the Doxygen docs
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT "${PROJECT_SOURCE_DIR}/Doxyfile")
    set(DOXYGEN_OUTPUT "${PROJECT_SOURCE_DIR}/doc")

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

    add_custom_target(apidoc_forced
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif(DOXYGEN_FOUND)

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