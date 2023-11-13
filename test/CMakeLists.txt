cmake_minimum_required(VERSION 2.4.0)
project (phtest)

include_directories(../include)

file(GLOB SOURCES "*.c")
file(GLOB LIBRARIES "../*.dylib")
message("LIBRARIES = ${LIBRARIES}")

add_executable(phtest ${SOURCES})
target_link_libraries(phtest ${LIBRARIES})  