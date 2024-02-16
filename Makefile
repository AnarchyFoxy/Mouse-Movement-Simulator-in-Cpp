# Nazwa pliku wykonywalnego
TARGET = MouseMovement

# Kompilator
CC = g++

# Flagi kompilatora
CFLAGS = -Wall

# Pliki źródłowe
SRCS = main.cpp

# Nazwa systemu operacyjnego
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	# Dodatkowe flagi dla macOS
	CFLAGS += -framework ApplicationServices -framework CoreGraphics
endif

# Plik wykonywalny
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Czyszczenie
clean:
	rm -f $(TARGET)
