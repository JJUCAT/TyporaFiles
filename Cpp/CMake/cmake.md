#### 一般工程配置

工程目录

```sh
./tutorial.cpp
./CMakeLists.txt
./TutorialConfig.h
./build
```

CMakeLists.txt：

```cmake
#版本要求
cmake_minimum_required(VERSION 3.13)
#设置工程名称和版本号
project(Tutorial VERSION 1.0)
#指定 C++ 版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#配置头文件将版本号传递给源文件
configure_file(TutorialConfig.h.in TutorialConfig.h)
#编译可执行文件
add_executable(Tutorial tutorial.cpp)

#目录添加到路径列表中以搜索包含文件
target_include_directories(Tutorial PUBLIC
						   "${PROJECT_BINARY_DIR}"
						   )
```

TurtialConfig.h.in：

```cmake
#设置主版本和子版本
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

tutorial.cpp：

```cpp
std::out << "Version" << Tutorial_VERSION_MAJOR << "."
         << Tutorial_VERSION_MINOR << std::endl;
```



在  `./build`  中执行  `cmake ..` ，再执行 `cmake --build .` 编译工程

