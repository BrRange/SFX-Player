COMP = g++ -std=c++20 # Compiler
MODE = -m64 -mwindows # Compile mode
SRCE = -o main src/*.cpp # Source files
INCF = -Iincludes/Include # Inclusion folder
LIBF = -Lincludes/lib # Library folder
LIBS = -lSDL2main -lSDL2 -lSDL2_mixer -lmingw32 # Libraries
debug:
	$(COMP) $(MODE) -g $(INCF) $(LIBF) $(SRCE) $(LIBS)
WAPI = -lsetupapi -limm32 -lole32 -loleaut32 -lwinmm -lversion # Static inclusion of win32
release:
	$(COMP) -static $(MODE) $(INCF) $(LIBF) -O2 $(SRCE) $(LIBS) $(WAPI)