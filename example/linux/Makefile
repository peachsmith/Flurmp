CC=gcc

# change the value of FLURMP_LIBS to match the directory
# where the Flurmp example dependencies are installed
FLURMP_LIBS=$(HOME)/Documents/flurmp_libs/output
SDL2_HOME=$(FLURMP_LIBS)/SDL2
SDL2_TTF_HOME=$(FLURMP_LIBS)/SDL2_ttf
FREETYPE_HOME=$(FLURMP_LIBS)/freetype

INC=-I$(SDL2_HOME)/include -I$(FREETYPE_HOME)/include -I../.. -I../include
LIB=-L$(SDL2_HOME)/lib -L$(SDL2_TTF_HOME)/lib -L$(FREETYPE_HOME)/lib
LNK=-lSDL2 -lSDL2_ttf -lfreetype -Wl,-rpath=$(SDL2_HOME)/lib -Wl,-rpath=$(SDL2_TTF_HOME)/lib -Wl,-rpath=$(FREETYPE_HOME)/lib

OBJ=obj
OBJECTS=$(OBJ)/main.o $(OBJ)/flurmp_impl.o $(OBJ)/input.o $(OBJ)/resource.o $(OBJ)/data_panel.o $(OBJ)/scene.o $(OBJ)/text.o $(OBJ)/console.o $(OBJ)/dialog.o $(OBJ)/player.o $(OBJ)/block_200_50.o $(OBJ)/sign.o $(OBJ)/menu.o $(OBJ)/pause_menu.o $(OBJ)/pause_submenu.o $(OBJ)/fish_submenu.o $(OBJ)/confirmation.o

all:
	$(CC) -c ../src/core/main.c           -o $(OBJ)/main.o          $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/core/flurmp_impl.c    -o $(OBJ)/flurmp_impl.o   $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/core/input.c          -o $(OBJ)/input.o         $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/core/resource.c       -o $(OBJ)/resource.o      $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/core/data_panel.c     -o $(OBJ)/data_panel.o    $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/core/scene.c          -o $(OBJ)/scene.o         $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/core/text.c           -o $(OBJ)/text.o          $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/console/console.c     -o $(OBJ)/console.o       $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/dialog/dialog.c       -o $(OBJ)/dialog.o        $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/entity/player.c       -o $(OBJ)/player.o        $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/entity/block_200_50.c -o $(OBJ)/block_200_50.o  $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/entity/sign.c         -o $(OBJ)/sign.o          $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/menu/menu.c           -o $(OBJ)/menu.o          $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/menu/pause_menu.c     -o $(OBJ)/pause_menu.o    $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/menu/pause_submenu.c  -o $(OBJ)/pause_submenu.o $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/menu/fish_submenu.c   -o $(OBJ)/fish_submenu.o  $(INC) $(LIB) $(LNK)
	$(CC) -c ../src/menu/confirmation.c   -o $(OBJ)/confirmation.o  $(INC) $(LIB) $(LNK)
	$(CC) $(OBJECTS) -o example $(INC) $(LIB) $(LNK)

clean:
	rm $(OBJ)/*.o

