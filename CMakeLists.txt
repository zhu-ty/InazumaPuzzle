
message(
  "
           
               ┌─┐       ┌─┐
            ┌──┘ ┴───────┘ ┴──┐
            │                 │
            │       ───       │
            │  ─┬┘       └┬─  │
            │                 │
            │       ─┴─       │
            │                 │
            └───┐         ┌───┘
                │         │
                │         │
                │         │
                │         └──────────────┐
                │                        │
                │                        ├─┐
                │                        ┌─┘    
                │                        │
                └─┐  ┐  ┌───────┬──┐  ┌──┘         
                  │ ─┤ ─┤       │ ─┤ ─┤         
                  └──┴──┘       └──┴──┘ 
                With the bless of the legend,
                there shall not be any bug.
      
  ")
cmake_minimum_required (VERSION 3.10)
set(name "InazumaPuzzle")
project ( ${name} )

# cmake module
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# c++ version
set (CMAKE_CXX_STANDARD 11)
#set (WIN_LIB_PATH "E:/libs")
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#MainFolder
file (GLOB this_headers_h "${PROJECT_SOURCE_DIR}/*.h")
file (GLOB this_headers_hpp "${PROJECT_SOURCE_DIR}/*.hpp")
file (GLOB this_sources_cpp "${PROJECT_SOURCE_DIR}/*.cpp")
file (GLOB this_sources_cu "${PROJECT_SOURCE_DIR}/*.cu")
file (GLOB this_sources_c "${PROJECT_SOURCE_DIR}/*.c")

set (MY_FILES
${this_headers_h}
${this_headers_hpp}
${this_sources_cpp}
${this_sources_cu}
${this_sources_c}
)

source_group( "__SKCOMMON__" FILES
    SKCommon.hpp
)
include_directories(
    ${PROJECT_SOURCE_DIR}
)

#Link&Exec
add_executable(${name}
    ${MY_FILES}
    )
# target_link_libraries(${name}
#     )

SET(SK_MAJOR_VERSION "1")
SET(SK_MINOR_VERSION "1")
add_definitions( -D__SK_MAJOR_VERSION__=${SK_MAJOR_VERSION})
add_definitions( -D__SK_MINOR_VERSION__=${SK_MINOR_VERSION})
SET(SK_VERSION_NAME __skversion__)

FILE(WRITE ${CMAKE_BINARY_DIR}/skversion.h.in
"\#define __GIT_VERSION__ \"@VERSION@\"\n"
)
IF(EXISTS ${PROJECT_SOURCE_DIR}/.git)
FILE(WRITE ${CMAKE_BINARY_DIR}/skversion.cmake
"EXECUTE_PROCESS(
     COMMAND git log -1 --format=%ad --date=format:%y%m%d.%H%M%S
	 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
     OUTPUT_VARIABLE VERSION
     OUTPUT_STRIP_TRAILING_WHITESPACE
 )
 CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
")
ELSE()
FILE(WRITE ${CMAKE_BINARY_DIR}/skversion.cmake
"EXECUTE_PROCESS(
     COMMAND echo unknown
     OUTPUT_VARIABLE VERSION
     OUTPUT_STRIP_TRAILING_WHITESPACE
 )
 CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
")
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
ADD_CUSTOM_TARGET(
    ${SK_VERSION_NAME}
    ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/skversion.h.in
                     -D DST=${CMAKE_BINARY_DIR}/skversion.h
                     -P ${CMAKE_BINARY_DIR}/skversion.cmake
)

if(WIN32)
    ADD_DEPENDENCIES(${name} ${SK_VERSION_NAME})
endif()
