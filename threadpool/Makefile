TARGET:=ThreadPool
CC:=g++ -std=c++11
HEADPATH:= inc
LIBPATH:= -L lib/
SRCPATH:= src
LINKLIB:=

CSRC:=$(wildcard $(SRCPATH)/*.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(CSRC))

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(LIBPATH) $(LINKLIB) 

%.o:%.cpp
	$(CC) -c $< -o $@ -I$(HEADPATH)

.PHONY : clean

clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET) 


SRC:= /home/china/code/project/threadpool/threadpool-CPP
DESPATH:= /mnt/hgfs/share/study/project/threadpool/

cp:
	cp $(SRC) $(DESPATH) -r