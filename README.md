adding pybind11 as submodel:
git submodule add -b stable ../../pybind/pybind11 extern/pybind11
git submodule add https://github.com/faalbers/FaaRay extern/FaaRay
git submodule update --init --recursive

To get and build it, Visual Studios Build Tools, cmake and git need to be installed in your system:
git clone https://github.com/faalbers/FaaRayPy
cd FaaRayPy
cmake -S . -B ./build
cmake -DCMAKE_BUILD_TYPE:STRING=Release -H. -B./build
cmake --build ./build --config Release --target install -j 10 --

