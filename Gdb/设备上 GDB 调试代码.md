1. 编译完成的可执行程序得支持添加调试信息：

   make 编译需要添加 -g ；

   Cmake 编译需要在 CMakeLists.txt 文件添加如下信息：

   ```c
   #Version 2.4 might work, but it won't find static libraries with odd names
   cmake_minimum_required(VERSION 2.6)
   
   #CMake used to complain without this
   #if(COMMAND cmake_policy)
   #cmake_policy(SET CMP0003 NEW)
   #endif(COMMAND cmake_policy)
   
   SET(CMAKE_BUILD_TYPE "Debug")
   SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
   SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
   ```

   

2. 源码添加到本地设备：

   将源码拷贝的设备中，例如把 src/ 放到设备中的根目录；

   先执行 ：

   ```c
   gdb testapp
   ```

   在 gdb 中执行：

   ```c
   (gdb) start
   ```

   此时会显示原执行程序的源码路径：

   ```c
   Thread 1 "pp" hit Temporary breakpoint 1, main (argc=1, argv=0x7ffffffb38)
       at /home/zylin/x912_work/src/pp/src/main.cpp:92
   ```

   在 gdb 中执行：

   ```c
   (gdb) set substitute-path /src /home/zylin/x912_work/src/pp/src
   ```

   重新映射源码路径

   

3. 调试程序：

