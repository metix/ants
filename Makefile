CC = gcc

CFLAGS += -lGL -lglut -lGLU
CFLAGS += -Wall -O7

TARGET = ants

SOURCES = main.c ants.c graphics.c control.c
OBJECTS = $(SOURCES:%.c=%.o)

all: target
	
target: $(TARGET)
	@echo " -- finished!"
	
$(TARGET) : $(OBJECTS)
	@echo " -- linking target ($(TARGET))"
	@$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS)
	
%.o: %.c
	@echo " -- compile file($<)"
	@$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -rf $(TARGET)
	rm -rf *.o
