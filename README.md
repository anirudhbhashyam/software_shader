# Software shader

Shader written by [XorDev](https://x.com/XorDev/status/1894123951401378051) implemented on the CPU using C++.
Frames are generated at compile time.

# Usage
Supports c++23 or c++26 (`>= clang 21.0.0 `)

```bash
clang++ -O3 -std=c++26 -fconstexpr-steps=2000000000 -o main main.cc
./main
ffmpeg -framerate 30 -i out_%02d.ppm -loop 0 plasma.gif
```

![plasma](plasma.gif)
