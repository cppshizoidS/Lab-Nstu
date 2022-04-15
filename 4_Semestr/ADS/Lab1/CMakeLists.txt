cmake_minimum_required(VERSION 3.10)
project(test)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_EXEC test)
set(TEST_SRC test.cpp)
set(MENU_EXEC menu)
set(MENU_SRC menu.cpp)

add_executable(${TEST_EXEC} ${TEST_SRC})
target_link_libraries(${TEST_EXEC} ${CONAN_LIBS})

add_executable(${MENU_EXEC} ${MENU_SRC})
target_link_libraries(${MENU_EXEC} ${CONAN_LIBS})

