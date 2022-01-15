# OakD Library #


## Install DepthAI-Core C++ Library ##

```bash
git submodule init
git submodule update --recursive
mkdir Libraries/depthai-core/Build
cd Libraries/depthai-core/Build
cmake ..
make # optionally: make -j NUM_PROCESSORS_YOU_HAVE
sudo make install
```


## Compile The OakD Library ##

```bash
mkdir Build
cd Build
cmake ..
make # optionally: make -j NUM_PROCESSORS_YOU_HAVE
```


## Include The OakD Library In A CMake Project ##

```
cmake_minimum_required(VERSION 3.4)
# Add the OakD library as a CMake subdirectory.
add_subdirectory(PATH_TO/OakD)
project(PROJECT_NAME)

...

add_executable(PROJECT_EXE ...)
# Link OakD to your project.
target_link_libraries(PROJECT_EXE PUBLIC OakD)
```


## Use The OakD Library To Stream RGB Video And Depth Frames ##

```cpp
#include "OakD.hpp"

int main()
{
    OakD oakCam;
    oakCam.Connect();
    oakCam.StartStream();
}
```


## Other Functionality ##

```cpp
#include "OakD.hpp"

int main()
{
    OakD oakCam;
    bool connected = oakCam.Connect();
    if (connected)
    {
        cv::Mat rgbImage = oakCam.GetImage();
        cv::Mat depthImage = oakCam.GetStereoImage();
    }
    else
    {
        // Failed to connect.
    }
}
```
