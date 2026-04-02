CC = gcc
CFLAGS = -Wall -Wextra

mdren: main.c reader.c parser.c renderer.c
	$(CC) $(CFLAGS) main.c reader.c parser.c renderer.c -o mdren

clean:
	rm -f mdren
