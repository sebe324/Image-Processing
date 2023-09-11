# Image-Processing

Real time demonstration of various image processing algorithms

## App preview

![obraz](https://github.com/sebe324/Image-Processing/assets/58781463/6d805cab-a9f3-4a53-8d1e-e36d72e63bb8)

## Project status

This project is 99% finished. I don't plan to add more stuff to it.
## How to build

### CMake
- Clone the source code
- Download SFML and OpenCV
- In the CMakeLists.txt change the following lines:
- ```set(CMAKE_PREFIX_PATH "C:\\libraries\\SFML-2.6.0-VC")```
- ```set(CMAKE_PREFIX_PATH "C:\\libraries\\opencv\\build")```
- ```target_include_directories(ImageProcessing PRIVATE "C:\\libraries\\opencv\\build\\include")```
- ```target_include_directories(ImageProcessing PRIVATE "C:\\libraries\\SFML-2.6.0-VC\\include")```
- To your download locations
- Then in console in the project do: ```cmake .``` ```cmake --build .```
- You may also need to move sfml and opencv DLLs to the folder with the executable.
### IDE
- Clone the source code
- Add it to a project in your IDE.
- Add the OpenCV library.
-  [OpenCV download tutorial](https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html), [OpenCV VS configuration](https://docs.opencv.org/4.x/dd/d6e/tutorial_windows_visual_studio_opencv.html)
-  Add the SFML library
-  [SFML VS configuration](https://www.sfml-dev.org/tutorials/2.6/start-vc.php)

## Other

This [video](https://www.youtube.com/watch?v=mRM5Js3VLCk) helped me to understand a lot of concepts related to image processing.
