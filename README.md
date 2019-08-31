CMake-Demo
=====

[CMake 入门实战](http://hahack.com/codes/cmake) 的源代码。



# 单个源文件 (Demo1)
## 编写源码
我们的项目中只有一个源文件 `main.cc` ，该程序实现计算一个数的指数幂。

## 编写 CMakeLists.txt
与 `main.cc` 源文件同个目录下：

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)

    # 项目的名称是 Demo1 
    project (Demo1)

    # 将源文件main.cc编译成一个名称为 Demo 的可执行文件
    add_executable(Demo main.cc)


`CMakeLists.txt` 的语法比较简单，由命令、注释和空格组成.

> 1 . 命令不区分大小写。 命令由命令名称、小括号和参数组成，参数之间使用空格进行间隔。  
> 2 . 符号 `#` 后面的内容是注释。  


对于上面的 `CMakeLists.txt` 文件，依次有如下命令：
> 1 . `cmake_minimum_required`：指定运行此配置文件所需的 CMake 的最低版本；  
> 2 . `project`：参数值是 Demo1，该命令表示项目的名称是 Demo1 。  
> 3 . `add_executable`： 将名为 main.cc 的源文件编译成一个名称为 Demo 的可执行文件。  

## 编译项目
在当前目录执行 `cmake .` ，得到 Makefile 后再使用 make 命令编译得到 Demo1 可执行文件。


# 多个源文件
## 同一目录，多个源文件(Demo2)

目录结构：

    ./Demo2    
        |
        +--- main.cc     
        |
        +--- MathFunctions.cc   
        |
        +--- MathFunctions.h   

`CMakeLists.txt` 为：

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)
    # 项目信息
    project (Demo2)
    # 指定生成目标
    add_executable(Demo main.cc MathFunctions.cc)

唯一的改动只是在 `add_executable` 命令中增加了一个 `MathFunctions.cc` 源文件。这样写当然没什么问题，但是如果源文件很多，把所有源文件的名字都加进去将是一件烦人的工作。更省事的方法是使用 `aux_source_directory` 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下：

    aux_source_directory(<dir> <variable>)

因此，可以修改 `CMakeLists.txt` 如下：

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)
    # 项目信息
    project (Demo2)
    # 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
    aux_source_directory(. DIR_SRCS)
    # 指定生成目标
    add_executable(Demo ${DIR_SRCS})

这样，CMake 会将当前目录所有源文件的文件名赋值给变量 `DIR_SRCS `，再指示变量 `DIR_SRCS` 中的源文件需要编译成一个名称为 Demo 的可执行文件。

## 多个目录，多个源文件(Demo3)
目录结构：

    ./Demo3   
        |
        +--- main.cc   
        |
        +--- math/   
            |
            +--- MathFunctions.cc   
            |
            +--- MathFunctions.h   

现在将 MathFunctions.h 和 MathFunctions.cc 文件移动到 math 目录下。

对于这种情况，需要分别在项目根目录 Demo3 和 math 目录里各编写一个 CMakeLists.txt 文件。为了方便，我们可以**先将 math 目录里的文件编译成静态库**再由 main 函数调用。

根目录中的 CMakeLists.txt ：

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)
    # 项目信息
    project (Demo3)
    # 查找当前目录下的所有源文件,并将名称保存到 DIR_SRCS 变量
    aux_source_directory(. DIR_SRCS)
    # 添加 math 子目录
    add_subdirectory(math)
    # 指定生成目标 
    add_executable(Demo main.cc)
    # 添加链接库
    target_link_libraries(Demo MathFunctions)

使用命令 `add_subdirectory` 指明本项目包含一个子目录 math，这样 math 目录下的 CMakeLists.txt 文件和源代码也会被处理 。
使用命令 `target_link_libraries` 指明可执行文件 main 需要连接一个名为 MathFunctions 的链接库 。

子目录中的 CMakeLists.txt：

    # 查找当前目录下的所有源文件
    # 并将名称保存到 DIR_LIB_SRCS 变量
    aux_source_directory(. DIR_LIB_SRCS)
    # 生成链接库
    add_library (MathFunctions ${DIR_LIB_SRCS})

在该文件中使用命令 `add_library` **将 src 目录中的源文件编译为静态链接库。**




*********************************
