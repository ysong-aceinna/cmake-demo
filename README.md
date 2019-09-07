CMake-Demo
=====

[CMake 入门实战](http://hahack.com/codes/cmake) 的源代码。

**********************************************************

# 1. 单个源文件 (Demo1)
## 1.1 编写源码
我们的项目中只有一个源文件 `main.cc` ，该程序实现计算一个数的指数幂。

## 1.2 编写 CMakeLists.txt
与 `main.cc` 源文件同个目录下:

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)

    # 项目的名称是 Demo1 
    project (Demo1)

    # 将源文件main.cc编译成一个名称为 Demo 的可执行文件
    add_executable(Demo main.cc)


`CMakeLists.txt` 的语法比较简单，由命令、注释和空格组成.

> 1 . 命令不区分大小写。 命令由命令名称、小括号和参数组成，参数之间使用空格进行间隔。  
> 2 . 符号 `#` 后面的内容是注释。  


对于上面的 `CMakeLists.txt` 文件，依次有如下命令:
> 1 . `cmake_minimum_required`:指定运行此配置文件所需的 CMake 的最低版本；  
> 2 . `project`:参数值是 Demo1，该命令表示项目的名称是 Demo1 。  
> 3 . `add_executable`: 将名为 main.cc 的源文件编译成一个名称为 Demo 的可执行文件。  

## 1.3 编译项目
在当前目录执行 `cmake .` ，得到 Makefile 后再使用 make 命令编译得到 Demo1 可执行文件。


# 2. 多个源文件
## 2.1 同一目录，多个源文件(Demo2)

目录结构:

    ./Demo2    
        |
        +--- main.cc     
        |
        +--- MathFunctions.cc   
        |
        +--- MathFunctions.h   

`CMakeLists.txt` 为:

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)
    # 项目信息
    project (Demo2)
    # 指定生成目标
    add_executable(Demo main.cc MathFunctions.cc)

唯一的改动只是在 `add_executable` 命令中增加了一个 `MathFunctions.cc` 源文件。这样写当然没什么问题，但是如果源文件很多，把所有源文件的名字都加进去将是一件烦人的工作。更省事的方法是使用 `aux_source_directory` 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下:

    aux_source_directory(<dir> <variable>)

因此，可以修改 `CMakeLists.txt` 如下:

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)
    # 项目信息
    project (Demo2)
    # 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
    aux_source_directory(. DIR_SRCS)
    # 指定生成目标
    add_executable(Demo ${DIR_SRCS})

这样，CMake 会将当前目录所有源文件的文件名赋值给变量 `DIR_SRCS `，再指示变量 `DIR_SRCS` 中的源文件需要编译成一个名称为 Demo 的可执行文件。

## 2.2 多个目录，多个源文件(Demo3)
目录结构:

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

根目录中的 CMakeLists.txt :

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

子目录中的 CMakeLists.txt:

    # 查找当前目录下的所有源文件
    # 并将名称保存到 DIR_LIB_SRCS 变量
    aux_source_directory(. DIR_LIB_SRCS)
    # 生成链接库
    add_library (MathFunctions ${DIR_LIB_SRCS})

在该文件中使用命令 `add_library` **将 src 目录中的源文件编译为静态链接库。**


# 3. 自定义编译选项 (Demo4)
CMake 允许为项目增加编译选项，从而可以根据用户的环境和需求选择最合适的编译方案。

例如，可以将 MathFunctions 库设为一个可选的库，如果该选项为 `ON` ，就使用该库定义的数学函数来进行运算。否则就调用标准库中的数学函数库。

## 3.1 修改 CMakeLists 文件
在顶层的 CMakeLists.txt 文件中添加该选项:

    # CMake 最低版本号要求
    cmake_minimum_required (VERSION 2.8)

    # 项目信息
    project (Demo4)

    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    # 加入一个配置头文件，用于处理 CMake 对源码的设置
    configure_file (
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
        )

    # 是否使用自己的 MathFunctions 库
    option (USE_MYMATH
        "Use provided math implementation" ON)

    # 是否加入 MathFunctions 库
    if (USE_MYMATH)
        include_directories ("${PROJECT_SOURCE_DIR}/math")
        add_subdirectory (math)
        set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions) #EXTRA_LIBS后边追加MathFunctions
    endif (USE_MYMATH)

    # 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
    aux_source_directory(. DIR_SRCS)

    # 指定生成目标
    add_executable (Demo ${DIR_SRCS})
    target_link_libraries (Demo  ${EXTRA_LIBS})

解释:
> 1 . `configure_file` 命令用于加入一个配置头文件 config.h ，这个文件由 CMake 从 `config.h.in` 生成，通过这样的机制，将可以通过预定义一些参数和变量来控制代码的生成。    
> 2 . `option` 命令添加了一个 `USE_MYMATH` 选项，并且默认值为 `ON `。    
> 3 . 根据`USE_MYMATH`变量的值来决定是否使用我们自己编写的 MathFunctions 库。   

## 3.2 修改 main.cc 文件
修改 main.cc 文件，让其根据 `USE_MYMATH` 的预定义值来决定是否调用标准库还是 MathFunctions 库，具体参考main.cc源码。

## 3.3 编写 config.h.in 文件
main.cc第2行引用了一个 config.h 文件，这个文件预定义了 `USE_MYMATH` 的值。但我们并不直接编写这个文件，为了方便从 CMakeLists.txt 中导入配置，我们编写一个 config.h.in 文件，内容如下，这样 CMake 会自动根据 CMakeLists 配置文件中的设置自动生成 config.h 文件。

    #cmakedefine USE_MYMATH

## 3.4 编译项目
注意:这里和`https://www.hahack.com/codes/cmake/`教程中写的不一致，注意以下几点:
> 1 . 教程中的`ccmake`命令在MAC上运行时没有`g`选项，不能保存配置。   
> 2 . 通过在 `CMakeLists.txt` 文件中打印message的方式进行调试发现，`USE_MYMATH`的值已经为`ON`, 但是main.cc 中是 `undefine USE_MYMATH`。

基于以上2点，正确的编译方式是在camke时将`USE_MYMATH`作为参数传入，如下:

    mkdir build
    cd build
    cmake -D USE_MYMATH=ON ..
    make


# 4. cmake的常用变量:

CMAKE_BINARY_DIR,PROJECT_BINARY_DIR,_BINARY_DIR:
这三个变量内容一致，如果是内部编译，就指的是工程的顶级目录，如果是外部编译，指的就是工程编译发生的目录。

CMAKE_SOURCE_DIR,PROJECT_SOURCE_DIR,_SOURCE_DIR:
这三个变量内容一致，都指的是工程的顶级目录。

CMAKE_CURRENT_BINARY_DIR:外部编译时，指的是target目录，内部编译时，指的是顶级目录
CMAKE_CURRENT_SOURCE_DIR:CMakeList.txt所在的目录
CMAKE_CURRENT_LIST_DIR:CMakeList.txt的完整路径
CMAKE_CURRENT_LIST_LINE:当前所在的行
CMAKE_MODULE_PATH:如果工程复杂，可能需要编写一些cmake模块，这里通过SET指定这个变量
LIBRARY_OUTPUT_DIR,BINARY_OUTPUT_DIR:库和可执行的最终存放目录
PROJECT_NAME:PROJECT NAME

其他的内置变量
1.BUILD_SHARED_LIBS:set the default value when using ADD_LIBRARY()
2.CMAKE_C_FLAGS: set compiler for c language
2.CMAKE_CXX_FLAGS: set compiler for c++ language

*********************************
