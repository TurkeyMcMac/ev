executable = ev
object-dir = .objects
source-files = $(wildcard src/*.h)
object-files = $(patsubst src/%.h, $(object-dir)/%.o, $(source-files))
main-object = $(object-dir)/main.o
opt-level = -O3

all: $(executable)

$(executable): $(object-dir) $(object-files) $(main-object)
	$(CC) -o $(executable) $(opt-level) -pthread $(object-files) $(main-object)

$(object-dir):
	mkdir $(object-dir)

$(object-dir)/%.o: src/%.c src/%.h
	$(CC) $(opt-level) -c $< -o $@

$(main-object):
	$(CC) $(opt-level) -c src/main.c -o $(main-object)

clean:
	rm -f $(executable)
	rm -rf $(object-dir)
