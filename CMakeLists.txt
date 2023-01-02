cmake_minimum_required(VERSION 3.25.1)

project(pt)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-fPIC -O3 -fopenmp")

add_link_options(-pthread)

# find_package(OpenCV REQUIRED)

# set(OpenCV_DIR "/usr/include/opencv4")

aux_source_directory(./include INCLUDE_FILE)

aux_source_directory(./src SRC_FILE)

aux_source_directory(. INCLUDE_FILE)

add_executable(pt ${INCLUDE_FILE} ${SRC_FILE})

# target_link_libraries(pt ${OpenCV_LIBS})