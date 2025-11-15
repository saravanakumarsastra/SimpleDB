TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/*.c, obj/*.o, $(SRC))
CFLAGS = -I include

run: clean default
	./$(TARGET) -f myfile.db -n

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*.o
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $? -Iinclude

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude
