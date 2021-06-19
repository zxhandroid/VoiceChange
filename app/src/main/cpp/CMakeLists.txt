# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# TODO 第一步 导入头文件
include_directories("inc")

# TODO 第二步 定义变量，批量导入所有的源文件
file(GLOB allCPP *.c *.h *.cpp)

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${allCPP})


#TODO 第三步，导入库文件
# CMAKE_SOURCE_DIR : 就是CMakeLists.txt 的路径
# CMAKE_ANDROID_ARCH_ABI: 当前cpu架构

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# TODO 第四步，链接具体的库到我们的总库 native-lib
target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        fmod
        fmodL
        )