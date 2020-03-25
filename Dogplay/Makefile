CXX = g++
CXX_STANDARD = c++14
CXXFLAGS = -Wall -Wextra -I include -std=$(CXX_STANDARD)
LIBS = -L lib -l sfml-graphics -l sfml-window -l sfml-system
EXEC = Dogplay

all : bin/$(EXEC)

bin/$(EXEC) : bin/prog
	echo "#!/bin/bash\n export LD_LIBRARY_PATH=lib; ./$<;" > $@
	chmod +x $@
	
bin/prog : bin/obj/main.o
	export LD_LIBRARY_PATH=lib
	$(CXX) $< -o $@ $(LIBS)
	
bin/obj/main.o : src/main.cxx
	$(CXX) $< -c -o $@ $(CXXFLAGS)

clean :
	rm -f bin/obj/*.o

mrproper : clean
	rm -f bin/$(EXEC) bin/prog

