CC:=arm-linux-g++ -std=c++11
HEADPATH:= -I inc/ -I lib_inc/ 
LIBPATH:= -L lib/
SRCPATH:= src
LINKLIB:= -ljpeg 

CSRC:=$(wildcard $(SRCPATH)/*.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(CSRC))


cc: $(OBJS)
%.o:%.cpp
	$(CC) -c $< -o $@ $(HEADPATH)


.PHONY : clean

clean:
	rm -rf $(OBJS)

cp:
	cp ../Common /mnt/hgfs/share/study/project/ -r
