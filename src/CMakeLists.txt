PROJECT(${PROJECT_NAME}) 

#设置程序源代码
#file(GLOB APP_SRCS "srv/*.cpp")

file(GLOB APP_SRCS "EnterPoint/*.cpp")
file(GLOB TIMER_SRCS "Timer/*.cpp")


	
#生成动态库
#ADD_LIBRARY(hello STATIC ${APP_SRCS})
#生成静态库
#ADD_LIBRARY(MSG STATIC ${MSGMANAGER_SRCS})
#生成可执行程序
ADD_EXECUTABLE(TimerExe ${APP_SRCS} ${TIMER_SRCS})

#设置所依赖的外部链接库
TARGET_LINK_LIBRARIES(TimerExe pthread )
	

