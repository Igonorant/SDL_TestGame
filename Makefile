OBJS = Main.cpp Game.cpp

OBJ_NAME = testGame

all : $(OBJS)
	g++ $(OBJS) -IC:\Users\Igor\Documents\Development\SDL2_64x\include -LC:\Users\Igor\Documents\Development\SDL2_64x\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME) 2> compiler.log
