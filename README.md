# Cpp Demo

## 第三方库集成

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

### 编译问题

1. warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失

   > 文件编码问题。将文件改为带签名的（BOM） `UTF-8` 编码格式即可。