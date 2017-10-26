name = ev
optimization-level = 3

all:
	$(CC) -pthread -O$(optimization-level) -o $(name) src/*.c

clean:
	$(RM) $(name)
