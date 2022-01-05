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
