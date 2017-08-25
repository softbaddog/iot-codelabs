
#make file v1.0

LIBS = -L../lib/ar502 -lcrypto -lssl -luspsdk -lpthread
INCLUDE = -I../include -I./
CC = arm-linux-gnueabi-gcc
#CC = gcc
CFLAGS += -Wno-deprecated -Wall -g

#define target file
BIN = ../bin/demo

#define compile variable
CUR_PATH = ./
OBJ_PATH = $(CUR_PATH)obj/

#获取到本目录下所有c文件
ALL_C = $(foreach dir,$(CUR_PATH),$(wildcard $(dir)*.c))
#构造预期的目标 .o 文件
ALL_OBJ = $(patsubst $(CUR_PATH)%.c,%.o,$(ALL_C))
#指定 .o 文件生成的目录
ALL_OBJ_POS = $(addprefix $(OBJ_PATH),$(ALL_OBJ))

#define target
.PHONY : all
all: $(ALL_OBJ) 
	$(CC) $(LIBS) $(CFLAGS) -o $(BIN) $(ALL_OBJ_POS)

$(ALL_OBJ): %.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $(OBJ_PATH)$@

.PHONY: clean
clean :
	-rm $(OBJ_PATH)*.o $(BIN)