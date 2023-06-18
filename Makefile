CC_VERBOSE = $(CC)
CC_NO_VERBOSE = @echo "Building $@..."; $(CC)

ifeq ($(VERBOSE),YES)
  V_CC = $(CC_VERBOSE)
  AT :=
else
  V_CC = $(CC_NO_VERBOSE)
  AT := @
endif

SDL2_LIBS = -lSDL2 -lSDL2_ttf -lm
FLAGS = -Wall -Wextra -pedantic

C_GRAFO_FILES = structures/grafo.c
O_GRAFO_FILES = ofiles/grafo.o
O_SDL_FILES = ofiles/sdl_utils.o
C_SDL_FILES = graphics/sdl_utils.c

OBJ_DIR = ofiles

.PHONY: all clean
.DEFAULT: all

all: main

main: $(O_GRAFO_FILES) $(O_SDL_FILES)
	$(V_CC) -o $@ $^ main.c $(FLAGS) $(SDL2_LIBS)

build:
	$(AT)mkdir -p $(OBJ_DIR)

$(O_SDL_FILES): $(C_SDL_FILES) | build
	$(V_CC) -c $< -o $@

$(O_GRAFO_FILES): $(C_GRAFO_FILES) | build
	$(V_CC) -c $< -o $@

clean:
	@echo Removing object files
	$(AT)-rm -f $(OBJ_DIR)/*.o
	@echo Removing applications
	$(AT)-rm -f main
	@echo Removing ofiles directory
	$(AT)-rm -rf ofiles
