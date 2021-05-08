OBJS = Main.cpp Engine\TextureManager.cpp Engine\Components.cpp Game\Game.cpp Game\Initialize.cpp

OBJ_NAME = testGame

all : $(OBJS)
	g++ -g $(OBJS) -IC:\Users\Igor\Documents\Development\SDL2_64x\include -LC:\Users\Igor\Documents\Development\SDL2_64x\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o $(OBJ_NAME) 2> compiler.log
