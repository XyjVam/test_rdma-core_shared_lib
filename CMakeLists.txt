cmake_minimum_required(VERSION 3.17)
project(test_ibv_reg_mr LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(IBLIBS "ibverbs")

# Add shared library
add_library(test_ibv_reg_mr_shared SHARED ib_context.cpp
  )
add_executable(test_ibv_reg_mr test_ibv_reg_mr.cpp)
target_link_libraries(test_ibv_reg_mr
test_ibv_reg_mr_shared ${IBLIBS})

add_executable(test_ibv_reg_mr_static test_ibv_reg_mr.cpp ib_context.cpp)
target_link_libraries(test_ibv_reg_mr_static ${IBLIBS})
