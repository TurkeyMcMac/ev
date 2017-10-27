# The name of the executable
executable = ev
# The name of the directory to store object files
object-dir = .objects
# The source files not including main.c (that's why they're selected by header)
source-files = $(wildcard src/*.h)
# The object files other than main.o
object-files = $(patsubst src/%.h, $(object-dir)/%.o, $(source-files))
# main.o
main-object = $(object-dir)/main.o
# The level of compiler optimization to use
opt-level = -O3

all: $(executable)

$(executable): $(object-dir) $(object-files) $(main-object)
	$(CC) -o $(executable) $(opt-level) -pthread $(object-files) $(main-object)

$(object-dir):
	mkdir $(object-dir)

$(object-dir)/%.o: src/%.c src/%.h
	$(CC) $(opt-level) -c $< -o $@

$(main-object): src/main.c
	$(CC) $(opt-level) -c src/main.c -o $(main-object)

clean:
	rm -f $(executable)
	rm -rf $(object-dir)
