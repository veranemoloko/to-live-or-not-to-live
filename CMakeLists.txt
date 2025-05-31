cmake_minimum_required(VERSION 3.10)  
project(GameOfLife)                   

set(CMAKE_C_STANDARD 11)            

add_executable(life main.c)          

if(UNIX)
    target_link_libraries(life m)    

if(WIN32)
    target_link_libraries(life winmm) 
endif()


file(COPY fields DESTINATION ${CMAKE_BINARY_DIR})

target_compile_options(life PRIVATE -O2)

endif()