CXXFLAGS := -fsanitize=address -g -I/usr/lib/jvm/java-21-openjdk-amd64/include \
	    -DDBG

FS := net str u 0
OBJS := $(patsubst %,%.cpp.o,$(FS))
CPPS := $(patsubst %,%.cpp,$(FS))

all: 0.exe

$(OBJS): $(CPPS)
	for f in $^; do clang++ -c $(CXXFLAGS) -o $$f.o $$f; done


0.exe: $(OBJS)
	clang++ $(CXXFLAGS) -o $@ $^

run: all
	./0.exe

gdb: all
	gdb ./0.exe

clean:
	rm -f *.o *.exe
