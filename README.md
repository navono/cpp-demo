# Cpp Demo

## 库

使用 `CMake` 生成动态库，不会自动生成 `.lib` 文件，需要为工程创建导出头文件，也就是在 `CMakeLists.txt` 中增加

> set(PROJECT_EXPORT_HPP ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/export.hpp)

然后在头文件中使用其生成的宏，例如：

> SHAREDLIB_EXPORT

这样，才会在 CMake 的 build 目录下为其生成 `.lib` 文件。具体参考 `sharedLib 工程`。

## 第三方库集成

### 使用 conan

在 Python 环境中
> pip install conan
>

生成默认编译器配置
> conan profile detect --force

配置生成在 `~/.conan2` 目录下，可根据实际情况生成不同的编译器选项。

生成 conan 包依赖文件
> touch conanfile.txt


编译
> conan install . --output-folder=build --build=missing --profile=debug
>
或者这样
> conan install . --output-folder=cmake-build-debug --build=missing -s build_type=Debug

在 `CLion` 中的 `CMake option` 中
> -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug
> -DCMAKE_TOOLCHAIN_FILE=cmake-build-debug/build/generators/conan_toolchain.cmake

### vcpkg

1. 下载源码安装
2. 设置系统环境变量 `VCPKG_ROOT`
3. 清单模式下，新增 `vcpkg.json` 文件
4. 在 `CMake` 编译选项中增加
   > -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
5. 在工程目录下运行
   > vcpkg install
6. 依赖库用法参考 `CMakeLists.txt`

### Boost

1. 下载源码
2. bootstrap.bat 进行编译前准备
3. 使用 b2 进行编译，也可指定要编译的库：--with filesystem regex

   > b2.exe --build-type=complete address-model=64 -j4 install --prefix=D:\sourcecode\cpp\boost-install
4. b2 tools/bcp 编译 bcp.exe， 可以进行指定模块提取，例如：
   > dist\bin\bcp filesystem regex E:\myboost

### CppCommon

#### 编译

需按照文档，下载 `gil` 后，再更新（管理员权限）
> gil update

### 目标文件拷贝

在 IDE 中进行编译，例如 `CLion`，架构选择 `amd64`，编译完成后，将 `cppcommon.lib` 拷贝到 `third_party/libs`
目录下，将 `include` 拷贝到 `include/CppCommon` 目录下。

### sqlite_orm

在本地编译 `sqlite3` 后，将 `lib` 与 `sqlite3.h` 分别放到 `third_party/libs/·` 与 `include/sqlite3` 目录下。
同时在 `third_party/CMakeLists.txt` 中指定变量。

### matplot

需预先在机器上安装 `gnuplot`。

### ZeroMQ

1. 下载源码，编译
2. 在 `CMakeLists.txt` 中，如果是使用静态库，增加 `ZMQ_STATIC` 宏， 增加静态库 `libzmq`，同时 `Windows` 平台还需要增加 `
   iphlpapi.lib`

### find_package 无法查找到库 XXX

1. `github` 上查找此库的源码，同时确认相关的版本
2. `git clone` 到本地后，使用 `CMake` 构建并安装

   > cmake -B_builds_debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="D:\\cmake_install"
   >
   > cmake --build _builds_debug --config Debug --target install

   注意上述的构建目录、构建类型、安装目录
3. 在目标工具中，例如 `CLion` 增加环境变量；也可在系统中增加系统环境变量

   > CMAKE_INCLUDE_PATH="D:\cmake_install\include"
   >
   > CMAKE_LIBRARY_PATH="D:\cmake_install\lib"
4. 有时二进制会依赖动态库，所以需要将 `D:\cmake_install\bin` 加入到系统环境变量中

其他 `CMake` 选项还有：

      CMAKE_SYSTEM_PREFIX_PATH

## 编译问题

1. warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失

   > 文件编码问题。将文件改为带签名的（BOM） `UTF-8` 编码格式即可。