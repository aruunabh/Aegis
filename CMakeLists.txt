cmake_minimum_required(VERSION 3.15)
project(Aegis Simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Homebrew Boost
set(BOOST_ROOT "/opt/homebrew")
find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(aegis_backend backend/main.cpp backend/SimulationEngine.cpp)

target_include_directories(aegis_backend PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(aegis_backend PRIVATE Boost::system Boost::thread pthread)