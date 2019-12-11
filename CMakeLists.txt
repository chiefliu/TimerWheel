# 版本限定,指定cmake的版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#编译机器限定（按需）
SET(ENV{Platform} x64) 
# 项目名称,支持语言列表可忽略,默认支持所有语言,将PROJECT_NAME替换为工程名称 
PROJECT(TimerManager CXX) 
#设置C++编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0  -g")
# 定义头文件的路径变量
INCLUDE_DIRECTORIES(
#  ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_SOURCE_DIR}/include/Timer
   #${PROJECT_SOURCE_DIR}/include/boost

  /usr/local/include
  /usr/include
)
# 添加链接库查找目录
LINK_DIRECTORIES(


  /usr/local/lib64
  /usr/local/lib
  /usr/lib64
)
#LINK_LIBRARIES(protobuf protoc)
# 设置可执行文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置库文件目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 添加源文件目录
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(example)

