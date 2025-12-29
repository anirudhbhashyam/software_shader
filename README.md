# Software shader

Shader written by [XorDev](https://x.com/XorDev/status/1894123951401378051) implemented on the CPU using C++.

# Usage

```bash
clang++ -std=c++23 -O3 -o main main.cc
./main
ffmpeg -framerate 24 -i out_%02d.ppm shader.mp4
```

![output.gif](output.gif)