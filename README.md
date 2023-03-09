# OpenGL学习

测试环境：
- Windows 10，MinGW 12.2.0
- Windows 10, MSVC 2022
- Ubuntu 20.04, gcc 9.4.0
- 原则上是跨平台的，不过其他平台并未测试。

## 环境配置

依赖：
- C++编译器
- CMake

Linux中：
- [glfw编译指南](https://www.glfw.org/docs/latest/compile.html)
- 安装xorg：
```sh
sudo apt install xorg-dev
```

## 构建

首先构建第三方库，并将库文件和头文件安装到`3rdparty-install/`目录：
```sh
cd ./3rdparty
mkdir build
cd ./build
```
- 对于单配置环境（`MinGW UNIX`等）：
```sh
cmake .. -G "your generator" -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install .
```
- 对于多配置环境（比如Visual Studio）：
```sh
cmake .. -G "your generator"
cmake --build . --config Release
cmake --install .
```

然后构建项目，请保证和第三方库使用同一工具链：
```sh
cd project-root-dir
mkdir build
cd ./build
cmake .. -G "your generator"
cmake --build .
```

## 资料

- [Khronos OpenGL® Registry](https://registry.khronos.org/OpenGL/index_gl.php)
- [OpenGL 4.6 Core Profile Specification with changes marked](https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.withchanges.pdf)
- [OpenGL® 4.5 Reference Pages](https://registry.khronos.org/OpenGL-Refpages/gl4/) (not yet updated for 4.6)
- The OpenGL® Shading Language, Version 4.60.7: [HTML](https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.60.html), [PDF](https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.60.pdf)
- [OpenGL 4.6 Quick Reference Card](https://www.khronos.org/files/opengl46-quick-reference-card.pdf)

## 参考

- [计算机图形学编程（使用OpenGL和C++）](https://book.douban.com/subject/34987432/)
