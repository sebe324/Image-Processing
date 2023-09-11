cmake_minimum_required(VERSION 3.10)

project(ImageProcessing)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "C:\\libraries\\SFML-2.6.0-VC")
find_package(SFML 2.6 COMPONENTS system graphics window REQUIRED)

set(CMAKE_PREFIX_PATH "C:\\libraries\\opencv\\build")
find_package(OpenCV REQUIRED)
add_executable(ImageProcessing ${SOURCES})

target_include_directories(ImageProcessing PRIVATE "C:\\libraries\\opencv\\build\\include")
target_include_directories(ImageProcessing PRIVATE "C:\\libraries\\SFML-2.6.0-VC\\include")

target_link_libraries( ImageProcessing PRIVATE
                    sfml-system
                    sfml-window
                    sfml-graphics
                    ${OpenCV_LIBS}
    
)
file(GLOB DESCRIPTIONS CONFIGURE_DEPENDS "*.txt")
file(COPY ${DESCRIPTIONS}
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)