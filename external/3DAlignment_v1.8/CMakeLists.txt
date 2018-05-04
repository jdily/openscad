# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.



cmake_minimum_required (VERSION 2.8.11)
project (LFD)


FIND_PACKAGE(OpenGL REQUIRED) 
MESSAGE("${OPENGL_INCLUDE_DIR}") 
FIND_PACKAGE(GLUT REQUIRED)
MESSAGE("${OPENGL_LIBRARIES}") 

MESSAGE("${GLUT_LIBRARIES}")

set(LFD_OUTPUT_LIB_DIR "./libs")

include_directories("./")
include_directories("./fftw/")
include_directories(${OPENGL_INCLUDE_DIR})

file(GLOB SOURCES "./*.c")
file(GLOB HEAD "./*.h") 
foreach(file_path ${HEAD})
    message(${file_path})
endforeach()

#set(lib_incs )
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

#link_directories(/usr/lib/x86_64-linux-gnu)
link_directories(${OPENGL_LIBRARIES})
link_directories(/usr/lib)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLU -lglut -lm") 
add_executable(${PROJECT_NAME} Main.c ${SOURCES})

target_link_libraries(LFD ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} "-lm -lrfftw -lfftw")


