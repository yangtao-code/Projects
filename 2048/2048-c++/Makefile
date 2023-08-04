#目标文件名
TARGET:= 2048.exe
#编译器
CC:= arm-linux-g++ -std=c++11

#### 	以下参数是项目的文件路径	####
#头文件路径
HEADPATH:= -I inc/ -I lib_inc/
#库路径
LIBPATH:= -L lib/
#源文件路径
SRCPATH:= src
#路径库名
LINKLIB:= -ljpeg -lpthread
########################################

######  以下参数是arm-linux开发板的公共文件	 ######

# CHEADPATH  arm-linux开发板项目的公共头文件路径
CHEADPATH:= -I ~/code/project/Common/inc \
-I ~/code/project/Common/lib_inc

# CSRCPATH arm-linux开发板项目的公共源文件路径
CSRCPATH:= ~/code/project/Common/src
# CLIBPATH arm-linux开发板项目的公共库文件路径
CLIBPATH:= -L ~/code/project/Common/lib

COBJS:=$(wildcard $(CSRCPATH)/*.o)
#################################################

CSRC:=$(wildcard $(SRCPATH)/*.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(CSRC))

$(TARGET):$(OBJS) $(COBJS)
	$(CC) $^ -o $@ $(LIBPATH) $(LINKLIB)

%.o:%.cpp
	$(CC) -c $< -o $@ $(HEADPATH) $(CHEADPATH)

.PHONY : clean

#清空中间文件和目标文件
clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET) 

cp:
	cp ../2048-c++/ /mnt/hgfs/share/study/project/2048/ -r