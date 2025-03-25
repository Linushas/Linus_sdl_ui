

OBJ = main.o button.o checkbox.o ui_extra.o #ex på viler 
# Hämta OS-namnet via uname. 
# Om uname inte finns (t.ex. ren Windows utan MSYS), sätt OS till Windows_NT.
OS := $(shell uname -s 2>/dev/null)
ifeq ($(OS),)
  OS := Windows_NT
endif

ifeq ($(OS), Darwin)
    # --- macOS Settings ---
    # Variables for compiler and flags
    CC = clang
    CFLAGS = -fsanitize=address -fsanitize=undefined -g -c -Wall -Wextra \
             -I/opt/homebrew/include/SDL2 \
             -I/opt/homebrew/include/SDL2_image \
             -I/opt/homebrew/include/SDL2_ttf \
             -I/opt/homebrew/include/SDL2_mixer \
             -I/opt/homebrew/include/SDL2_net
    LDFLAGS = -fsanitize=address \
              -I/opt/homebrew/include/SDL2 \
              -L/opt/homebrew/lib \
              -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf \
              -lSDL2_mixer -lSDL2_net

    # File names
#Saman satta fil namnet
    EXEC = Isometric 
#sök väg för source filse från relevent phat
	SRCDIR = source
#remove comand
	REMOV = rm -f *.o 
# exequte operator
	PREFORM =MallocNanoZone=0 ./

else ifeq ($(OS), Windows_NT)
    # --- Windows (MinGW/MSYS) Settings ---
    # Adjust these paths for your environment:
    CC = gcc
    # If your SDL2 is in C:/SDL2, for example:
    ####################  LADE TILL: \SDL2      !!
    INCLUDE=C:\msys64\mingw64\include\SDL2
	CFLAGS=-g -c -I$(INCLUDE)
    #################################### # LADE TILL: -lSDL2_ttf -lSDL2_mixer -lSDL2_net        !!
	LDFLAGS= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net 
#-mwindows 
    EXEC = main.exe
	SRCDIR = ./source
	REMOV = del /f *.o
	PREFORM = ./

endif

 # Linking
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compile source code
main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o main.o

button.o: $(SRCDIR)/button.c
	$(CC) $(CFLAGS) $(SRCDIR)/button.c -o button.o

ui_extra.o: $(SRCDIR)/ui_extra.c
	$(CC) $(CFLAGS) $(SRCDIR)/ui_extra.c -o ui_extra.o

checkbox.o: $(SRCDIR)/checkbox.c
	$(CC) $(CFLAGS) $(SRCDIR)/checkbox.c -o checkbox.o

# Clean binaries
clean: 
	$(REMOV) $(EXEC)

# Run the program
run:  
	$(PREFORM)$(EXEC)
