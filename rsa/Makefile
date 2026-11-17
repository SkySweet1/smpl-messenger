CC = gcc
CFLAGS = -Wall -Wextra -O2 -Ibigint -Isrc
TARGET = rsa_test

SRCS = src/main.c src/rsa.c bigint/bigint.c bigint/bigint_math.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)