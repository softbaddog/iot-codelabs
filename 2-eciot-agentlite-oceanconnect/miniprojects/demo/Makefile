OBJS = demo.o
CC = arm-linux-gnueabi-gcc
TARGET = demo

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o demo $(OBJS) -L../lib -lcrypto -lssl -luspsdk
$(OBJS):%.o: %.c
	$(CC) -c $(CFLAGS) -I../include $< -o $@

.PHONY: clean
clean:
	-rm -v *.o demo

