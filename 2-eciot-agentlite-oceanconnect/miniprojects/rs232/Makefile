OBJS = 1.o
CC = arm-linux-gnueabi-gcc
TARGET = rs232_test

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o rs232_test $(OBJS) -L../lib -lcrypto -lssl -luspsdk
$(OBJS):%.o: %.c
	$(CC) -c $(CFLAGS) -I../include $< -o $@

.PHONY: clean
clean:
	-rm -v *.o rs232_test

