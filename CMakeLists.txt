cmake_minimum_required(VERSION 3.15...3.30)
# Specify the project name
project(Paralelismo_dotproduct)
# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Specify the source files
set(SOURCES
  classes/dotproduct.cpp
  main.cpp
)
# Specify the header files (optional, for better IDE integration)
set(HEADERS
  classes/dotproduct.h
)
set(EXECUTABLE_NAME "dp_par_sec")
add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})