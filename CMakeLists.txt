project ( Goosball C CXX )

set ( SOURCE_FILES
    main.cpp
    src/glad.c
    init.c
    )

include_directories ( ${CMAKE_SOURCE_DIR}/include/ )

add_definitions ( -DUNICODE -D_WINNT=0x0600 )
add_executable ( Goosball WIN32 "${SOURCE_FILES}" )

target_link_libraries ( Goosball Opengl32 )
target_compile_features ( Goosball PRIVATE cxx_range_for )
