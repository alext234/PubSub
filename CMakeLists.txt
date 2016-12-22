cmake_minimum_required(VERSION 3.2)

project(PubSub)

if (NOT TARGET gtest)
    add_subdirectory(googletest)
endif() 


set(GOOGLETEST ${CMAKE_CURRENT_SOURCE_DIR}/googletest/)  
include_directories(${GOOGLETEST}/googlemock/include ${GOOGLETEST}/googletest/include)
link_directories(${GOOGLETEST}/build/googlemock ${GOOGLETEST}/build/googlemock/gtest)
if(NOT CMAKE_BUILD_TYPE)
    message( SEND_ERROR "please set CMAKE_BUILD_TYPE to Debug or Release" )
endif(NOT CMAKE_BUILD_TYPE)

# set build type: cmake -DCMAKE_BUILD_TYPE=Debug  ; to see all options of AddressSanitizer, set the environment variable ASAN_OPTIONS=help=1
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -std=c++1y -Wall -Wextra -Wno-unused-parameter -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -g -std=c++1y -Wall -Wextra -Wno-unused-parameter")


add_executable(runtests  tests.cpp)
target_link_libraries(runtests gmock)
target_link_libraries(runtests gtest)
target_link_libraries(runtests pthread)

