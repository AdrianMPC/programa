cmake_minimum_required(VERSION 3.15...3.30)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  DP
  VERSION 1.0
  LANGUAGES CXX)

  # Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# You should usually split this into folders, but this is a simple example


# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(DP main.cpp)