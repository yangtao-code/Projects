TARGET:= ftp_server
CC:= gcc
HEADPATH:=-I inc/
LIBPATH:= -L lib/
SRCPATH:=src
LINKLIB:= -lpthread

CSRC:=$(wildcard $(SRCPATH)/*.c)
OBJS:=$(patsubst %.c,%.o,$(CSRC))

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(LIBPATH) $(LINKLIB) 

%.o:%.c
	$(CC) -c $< -o $@ $(HEADPATH)

.PHONY : clean

clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET)