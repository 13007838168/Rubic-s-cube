cmake_minimum_required(VERSION 3.23)
project(RubicsCube C)
set(CMAKE_C_STANDARD 17)

include_directories("D:/software/SDL2/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/include/SDL2"
        include)
link_directories("D:/software/SDL2/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/lib"
        lib/x64)
add_executable(test1 test1.c )
add_executable(test2 test2.c)
add_executable(test3 test3.c test3_main.c test3_head.h)
add_executable(test4 test4.c)
add_executable(test5 test5.c )
add_executable(test6 test6.c )
target_link_libraries(test1  PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        opengl32
        )
target_link_libraries(test2  PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        opengl32
        )
target_link_libraries(test3  PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        opengl32
        libopengl32.a libfreeglut.a glu32.dll
        freeglut opengl32
        )
target_link_libraries(test4  PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        opengl32
        )

target_link_libraries(test5 PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        opengl32
        libopengl32.a libfreeglut.a glu32.dll
        freeglut opengl32
        )
target_link_libraries(test6 PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        opengl32
        libopengl32.a libfreeglut.a glu32.dll
        freeglut opengl32
        )
