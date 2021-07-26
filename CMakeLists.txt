
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
set(name "SLFDriver")
project ( ${name} )
ADD_SUBDIRECTORY(ReAlign)

option(SK_ZEISS_CONTROLLER "Find Zeiss Controller Support" OFF)
option(SK_ANDOR_CAMERA_SUPPORT "Support Andor Camera" ON)
option(SK_IOI_CAMERA_SUPPORT "Support IOI Camera" OFF)
option(SK_PCO_CAMERA_SUPPORT "Support PCO Camera" OFF)

if ( WIN32 )
    ADD_SUBDIRECTORY(ScanConfigGenerator)
	IF(SK_ZEISS_CONTROLLER)
		ADD_SUBDIRECTORY(ZeissController)
	ENDIF()
    
    
    # cmake module
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    # c++ version
    set (CMAKE_CXX_STANDARD 11)
    set (WIN_LIB_PATH "E:/libs")
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    #OpenCV
    if ( WIN32 )
        set(OpenCV_DIR ${WIN_LIB_PATH}/OpenCV/x64/vc14/lib)
        set(OpenCV_CONFIG_PATH ${WIN_LIB_PATH}/OpenCV)
    endif()
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})

    find_package(CUDA)
    set(CUDA_NVCC_FLAGS
        ${CUDA_NVCC_FLAGS}
    #    -O3 
        -gencode=arch=compute_61,code=\"sm_61,compute_61\"
        )


    # opengl
    find_package(OpenGL REQUIRED)
    if ( WIN32 )
        set(GLFW_INCLUDE_PATH ${WIN_LIB_PATH}/OpenGL/include/GLFW)
        set(GLFW_LIBRARY ${WIN_LIB_PATH}/OpenGL/lib/glfw3.lib )
        #set(GLEW_INCLUDE_PATH ${WIN_LIB_PATH}/OpenGL/include)
        #set(GLEW_LIBRARY_DEBUG ${WIN_LIB_PATH}/OpenGL/lib/glew32d.lib)
        #set(GLEW_LIBRARY_RELEASE ${WIN_LIB_PATH}/OpenGL/lib/glew32.lib)
        set(GLM_INCLUDE_PATH ${WIN_LIB_PATH}/OpenGL/include)
    endif()
    include_directories(
    #${GLFW_INCLUDE_PATH}
    #${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH})

    #Eigen
    set(EIGEN_INCLUDE_DIR ${WIN_LIB_PATH}/Eigen/include/eigen3)
    include_directories(${EIGEN_INCLUDE_DIR})

    ##nanogui
    #SET(NANOGUI_DIR ${WIN_LIB_PATH}/nanogui)
    #include_directories(${NANOGUI_DIR}/include)
    #SET(NANOGUI_LIBRARY ${NANOGUI_DIR}/lib/nanogui.lib)

    #nanogui
    # nano-gui, to add buttons
    SET(NANOGUI_DIR ${WIN_LIB_PATH}/NanoGUI)
    set(NANOGUI_LIBRARY 
        debug ${NANOGUI_DIR}/lib/nanogui_d.lib
        optimized ${NANOGUI_DIR}/lib/nanogui.lib
        ${NANOGUI_DIR}/lib/glfw_objects.lib
    )
    include_directories(
        ${NANOGUI_DIR}/include
    )
    add_definitions("-DNANOGUI_GLAD")

    SET(CAMERA_FILES 
        ${PROJECT_SOURCE_DIR}/SLFGUI/SKCamera/SKCamera.h
        ${PROJECT_SOURCE_DIR}/SLFGUI/SKCamera/SKCamera.cpp
    )
    include_directories(${PROJECT_SOURCE_DIR}/SLFGUI/SKCamera)

    if(SK_ANDOR_CAMERA_SUPPORT)
        #Andor
        SET(ANDOR_SDK_INCLUDE_DIR "C:/Program Files/Andor SDK3")
        SET(CAMERA_SDK_LIBRARY 
            "C:/Program Files/Andor SDK3/atcorem.lib"
            "C:/Program Files/Andor SDK3/atutilitym.lib")
        include_directories(${ANDOR_SDK_INCLUDE_DIR})
        SET(CAMERA_FILES 
            ${CAMERA_FILES}
            ${PROJECT_SOURCE_DIR}/SLFGUI/SKCamera/AndorCamera.cpp
        )
    elseif(SK_IOI_CAMERA_SUPPORT)
		include_directories("C:/Program Files/KAYA Instruments/Vision Point/include")
		SET(CAMERA_SDK_LIBRARY 
			"C:/Program Files/KAYA Instruments/Vision Point/lib/KYFGLib.lib")
        SET(CAMERA_FILES 
            ${CAMERA_FILES}
            ${PROJECT_SOURCE_DIR}/SLFGUI/SKCamera/IOICamera.cpp
        )
        add_definitions(-D_CMAKE_IOI_CAMERA_)
    elseif(SK_PCO_CAMERA_SUPPORT)
		include_directories("C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/include")
		SET(CAMERA_SDK_LIBRARY 
            "C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/lib64/PCO_CDlg.lib"
            "C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/lib64/Pco_conv.lib"
            "C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/lib64/PCO_CryptDll.lib"
            "C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/lib64/pco_edge_gs_calc.lib"
            "C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/lib64/SC2_Cam.lib"
            "C:/Program Files (x86)/PCO Digital Camera Toolbox/pco.sdk/lib64/SC2_DLG.lib"
            )
        SET(CAMERA_FILES 
            ${CAMERA_FILES}
            ${PROJECT_SOURCE_DIR}/SLFGUI/SKCamera/PcoCamera.cpp
        )
        add_definitions(-D_CMAKE_PCO_CAMERA_)
    endif()

    source_group( "__CAMERA__" FILES
        ${CAMERA_FILES}
    )

    #Labview
    SET(NI_DIR ${PROJECT_SOURCE_DIR}/NI/builds/scanProj/NIConrtolVersion2)
    include_directories(${NI_DIR})
    SET(NI_LIBRARY ${NI_DIR}/NIControl.lib)
    SET(NI_FILES 
        ${NI_DIR}/NIControl.h
        ${PROJECT_SOURCE_DIR}/NI/NInterface.h
        ${PROJECT_SOURCE_DIR}/NI/NInterface.cpp)
    source_group( "__NI__" FILES
        ${NI_FILES}
    )
    INCLUDE_DIRECTORIES("C:/Program Files/National Instruments/LabVIEW 2019/cintools")

#    #ImageLosslessZipper
#    #ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/ImageLosslessZipper)
#    file (GLOB ilz_headers_h "${PROJECT_SOURCE_DIR}/ImageLosslessZipper/*.h")
#    file (GLOB ilz_headers_hpp "${PROJECT_SOURCE_DIR}/ImageLosslessZipper/*.hpp")
#    file (GLOB ilz_sources_cpp "${PROJECT_SOURCE_DIR}/ImageLosslessZipper/*.cpp")
#    file (GLOB ilz_sources_cu "${PROJECT_SOURCE_DIR}/ImageLosslessZipper/*.cu")
#    file (GLOB ilz_sources_c "${PROJECT_SOURCE_DIR}/ImageLosslessZipper/*.c")
#
#    SET (IMZ_FILES
#        ${ilz_headers_h}
#        ${ilz_headers_hpp}
#        ${ilz_sources_cpp}
#        ${ilz_sources_cu}
#        ${ilz_sources_c}
#    )
#    #MESSAGE(${IMZ_FILES})
#    list(REMOVE_ITEM IMZ_FILES "${PROJECT_SOURCE_DIR}/ImageLosslessZipper/main.cpp")
#    source_group( "__ZIPPER__" FILES
#        ${IMZ_FILES}
#    )

    #tiff
    SET(LIBTIFF_DIR ${WIN_LIB_PATH}/libtiff)
    set(LIBTIFF_LIBRARY 
        ${LIBTIFF_DIR}/lib/tiff.lib
    )
    include_directories(
        ${LIBTIFF_DIR}/include
    )
    SET(TIFF_FILES
        ${PROJECT_SOURCE_DIR}/TiffWriter/TiffWriter.cpp
		${PROJECT_SOURCE_DIR}/TiffWriter/AsyncImageWriter.cpp
        ${PROJECT_SOURCE_DIR}/TiffWriter/TiffWriter.h
		${PROJECT_SOURCE_DIR}/TiffWriter/AsyncImageWriter.h
        )
    include_directories(${PROJECT_SOURCE_DIR}/TiffWriter)
    source_group( "__TIFF__" FILES
        ${TIFF_FILES}
    )

    #SLFGUI Fils
    file (GLOB gui_headers_h "${PROJECT_SOURCE_DIR}/SLFGUI/*.h")
    file (GLOB gui_headers_hpp "${PROJECT_SOURCE_DIR}/SLFGUI/*.hpp")
    file (GLOB gui_sources_cpp "${PROJECT_SOURCE_DIR}/SLFGUI/*.cpp")
    file (GLOB gui_sources_cu "${PROJECT_SOURCE_DIR}/SLFGUI/*.cu")
    file (GLOB gui_sources_c "${PROJECT_SOURCE_DIR}/SLFGUI/*.c")

    set (GUI_FILES
    ${gui_headers_h}
    ${gui_headers_hpp}
    ${gui_sources_cpp}
    ${gui_sources_cu}
    ${gui_sources_c}
    )
    source_group( "__SLF_DRIVER__" FILES
        ${GUI_FILES}
    )
    INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/SLFGUI")

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
    ${IMZ_FILES}
    ${GUI_FILES}
    ${NI_FILES}
    ${TIFF_FILES}
	${CAMERA_FILES}
    )

    source_group( "__SKCOMMON__" FILES
        SKOpenGL_NANOGUI.hpp
        SKOpenGL_NANOGUI.cpp
        SKCommon.hpp
    )
    include_directories(
        ${PROJECT_SOURCE_DIR}
    )

    #Link&Exec
    cuda_add_executable(${name}
        ${MY_FILES}
        )
    target_link_libraries(${name}
        ${ILZ_LIBRARY}
        ${NI_LIBRARY}
        ${NANOGUI_LIBRARY}
        ${OpenCV_LIBS}
        ${OPENGL_LIBRARY}
        ${GLFW_LIBRARY}
        ${LIBTIFF_LIBRARY}
        cuda
        ${CUDA_LIBRARY} 	
        ${CUDA_npp_LIBRARY}
        ${CAMERA_SDK_LIBRARY}
        #debug  ${GLEW_LIBRARY_DEBUG}
        #optimized  ${GLEW_LIBRARY_RELEASE}
        )

    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /DELAYLOAD:NIControl.dll")
endif()

SET(SK_MAJOR_VERSION "1")
SET(SK_MINOR_VERSION "3")
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
