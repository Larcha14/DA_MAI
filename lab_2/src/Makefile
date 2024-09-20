MAIN = manage
CC = g++
EXT = cpp
CFLAGS = -pedantic -Wall

$(MAIN) : $(MAIN).$(EXT) TPair.h TStr.cpp TPatricia.cpp
	$(CC) $(OBJ) TPair.h TStr.cpp TPatricia.cpp $(MAIN).$(EXT) -o $(MAIN) 

clean : 
	rm $(MAIN)  

