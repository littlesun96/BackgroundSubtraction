cmake_minimum_required (VERSION 2.8)

project (show_img)

FIND_PACKAGE(OpenCV 3.0 REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
#    list(APPEND CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4" CACHE STRING COMPILE_FLAGS FORCE)
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    )

# ----------------------------------------------------------------------
set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp BackgroundSubtractionAverage.cpp 
	BackgroundSubtractionBase.cpp BackgroundSubtractionSingleGauss.cpp
)

set(HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/BackgroundSubtractionAverage.h BackgroundSubtractionBase.h
	BackgroundSubtractionSingleGauss.h
)


set(LIBS  ${OpenCV_LIBS})

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${LIBS})
