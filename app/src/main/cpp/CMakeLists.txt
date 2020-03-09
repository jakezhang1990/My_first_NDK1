# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
#cmake最小版本
cmake_minimum_required(VERSION 3.4.1)

#1.配置头文件的查找路径
include_directories(inc) #相对路径，也就是头文件相对这个CMakeLists.txt文件的路径,
                        #在这里${CMAKE_SOURCE_DIR}/inc的意思等同于inc。
                        #CMAKE_SOURCE_DIR表示的路径是src/main/cpp这一个路径。

##b1 设置bzip2的头文件查找路径
include_directories(bzip2)

## b2设置库的查找路径，下面那个是通过预编译so库的形式，现在是bzip2文件夹下C源码的形式，这里指定这个文件夹下的源码路径即可
aux_source_directory(bzip2 bzip2_sources) #表示bzip2这个源码的文件夹路径设置为bzip2_sources这个变量，后面使用的时候直接${bzip2_sources}来使用。

#2.设置库的查找路径（还是相对路径，相对于CMakeLists.txt这个文件）。追加参数（库都放在了jniLibs目录中了）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")  #使用系统变量CMAKE_CXX_FLAGS,也就是设置c++的编译参数。
                     #设置CMAKE_CXX_FLAGS这个系统变量的值为后面双引号中的内容。$符号表示引用。
        #CMAKE_CXX_FLAGS是系统变量，里面原本可能有值，需要保留，现在做一个追加，把要设置的内容追加在原本内容的后面
        # 大写的L表示设置一个库的查找路径，L后面没有空格直接跟上相对路径。
        #so文件在相对于CMakeList文件的上一层的jniLibs目录下，这个目录下又是各个平台对应的so，比如x86、arm64等平台。所以路径是${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}
        #CMAKE_ANDROID_ARCH_ABI是系统变量，会自动根据当前设备选择对应的平台。

#3.链接目标库，通过地下的target_link_libraries命令去链接。链接的其实就是平台文件夹下的so文件，链接的时候去掉前面的lib这3个字母，比如链接libfmod.so和libfmodL.so。


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#添加库
add_library( # Sets the name of the library.
             native-lib #库名字 libnative-lib.so

             # Sets the library as a shared library.
             SHARED  #库的类型static

             # Provides a relative path to your source file(s).
             native-lib.cpp #库源码相对路径
        ${bzip_sources}  # b3 使用bzip2的源码路径指定的变量，把这个bzip2源码加入本地库native-lib 中
        #bspatch.c
            )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
#查找库
find_library( # Sets the name of the path variable.
              log-lib  #log库的变量名也就是别名

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log #库名
            )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
#设置目标库要链接的其他库
target_link_libraries( # Specifies the target library.
                       native-lib #目标库名字

                       # Links the target library to the log library
                       # included in the NDK.
                        fmod   #b4 要链接的库
                        fmodL
                       ${log-lib} )