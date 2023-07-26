TARGET:=CoolAlbum
CC:=arm-linux-gcc

HEADPATH:=inc
SRCPATH:=src
LIBPATH+=-Llib
LINKLIB:=-lm -lpthread -ljpeg

CSRC:=$(wildcard $(SRCPATH)/*.c)
OBJS:=$(patsubst %.c,%.o,$(CSRC))


$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(LIBPATH) $(LINKLIB) 

%.o:%.c
	$(CC) -c $< -o $@ -I$(HEADPATH)

$(LIBNAME):$(LIBOBJS)
	ar -rc $@ $<

.PHONY : clean

clean:
	rm -rf $(OBJS) $(TARGET) $(LIBOBJS)