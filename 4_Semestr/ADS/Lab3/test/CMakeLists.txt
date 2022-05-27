if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

add_executable(t234 t234.cpp)
target_link_libraries(t234 ${CONAN_LIBS})

add_executable(randtree randtree.cpp)
target_link_libraries(randtree ${CONAN_LIBS})

