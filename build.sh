#!/bin/sh

set -x
    #set是把它下面的命令打印到屏幕
SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-../build}
BUILD_TYPE=${BUILD_TYPE:-debug}
INSTALL_DIR=${INSTALL_DIR:-../${BUILD_TYPE}-install}
BUILD_NO_EXAMPLES=${BUILD_NO_EXAMPLES:-0}
                            #选项介绍
                            #--graphviz,生成依赖的graphviz图,待学习
                            #-DCMAKE_BUILD_TYPE,建立debug/release两目录，分别在其中执行cmake -DCMAKE_BUILD_TYPE=Debug（或Release），需要编译不同版本时进入不同目录执行make即可
                            #-DCMAKE_INSTALL_PREFIX,可执行文件的安装路径
                            #-DCMAKE_BUILD_NO_EXAMPLES,未知
                            #SOURCE_DIR,指在当前文件夹下执行cmake,该文件夹下应该包含CMakeLists.txt文件。
                            #$*这个变量表示目标模式中“%”及其之前的部分
                            # && 保证迁移命令执行成功，再执行后续命令
                            #CMake主要是编写CMakeLists.txt文件，然后用cmake命令将CMakeLists.txt文件转化为make所需要的makefile文件，最后用make命令编译源码生成可执行程序或共享库
mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && cmake --graphviz=dep.dot \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           -DCMAKE_BUILD_NO_EXAMPLES=$BUILD_NO_EXAMPLES \
           $SOURCE_DIR \
  && make $*

cd $SOURCE_DIR #&& doxygen

