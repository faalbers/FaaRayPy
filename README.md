cloning FaaRayPy:
git clone --recursive -b stable https://github.com/faalbers/FaaRayPy.git
in vscode: ctrl+sht+p Git: Clone (Recursive)

adding pybind11 as submodel:
git submodule add -b stable ../../pybind/pybind11 extern/pybind11
git submodule update --init --recursive

To build I used Visual Studios Build Tools, MINGW 64 did not seem to work well
git clone --recursive -b stable https://github.com/faalbers/FaaRayPy.git
cd FaaRayPy
cmake -DCMAKE_BUILD_TYPE:STRING=Release -G "Visual Studio 16 2019" -H. -B./build
cmake --build ./build --config Release --target install -j 10 --
python testfaaray.py
