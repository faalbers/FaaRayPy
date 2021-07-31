# FaaRayPy

Python wrapper using
[FaaRay](https://github.com/faalbers/FaaRay)
ray trace rendering library.

### git clone:

```
git clone --recursive https://github.com/faalbers/FaaRayPy.git
```

### git clone in vscode:

```
ctrl+sht+p -> Git: Clone (Recursive)
```

### build

```
cd FaaRay
cmake -B build -D CMAKE_BUILD_TYPE:STRING=Release
cmake --build build --config Release --target install -j 10 --
```

### run python test to render and safe an image
```
python testfaaray
```
