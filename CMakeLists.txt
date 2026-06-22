cmake_minimum_required(VERSION 3.15)
project(Netflix_Inventory_API VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find necessary packages
find_package(Threads REQUIRED)

# We will assume crow and pqxx are installed in the system (e.g. via apt in docker)
find_path(CROW_INCLUDE_DIR NAMES crow.h)
if(NOT CROW_INCLUDE_DIR)
    message(FATAL_ERROR "Crow header not found")
endif()

find_library(PQXX_LIBRARY pqxx)
find_library(PQ_LIBRARY pq)
if(NOT PQXX_LIBRARY OR NOT PQ_LIBRARY)
    message(FATAL_ERROR "libpqxx or libpq not found")
endif()

# Source files
file(GLOB_RECURSE SRC_FILES 
    "src/*.cpp" 
    "repositories/*.cpp" 
    "services/*.cpp" 
    "controllers/*.cpp"
)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/models 
    ${CMAKE_SOURCE_DIR}/repositories 
    ${CMAKE_SOURCE_DIR}/services 
    ${CMAKE_SOURCE_DIR}/controllers
    ${CROW_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Threads::Threads
    ${PQXX_LIBRARY}
    ${PQ_LIBRARY}
)
