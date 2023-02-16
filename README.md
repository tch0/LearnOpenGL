# OpenGL学习

测试环境：
- Windows 10，MinGW 12.2.0
- Windows 10, MSVC 2022
- Ubuntu 20.04, gcc 12.1.0
- 原则上是跨平台的，不过其他平台并未测试。

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
