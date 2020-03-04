CFLAGS = -g -Wall -IEngine
LIBFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image
CC = gcc

GAMES_SRC = $(wildcard *.c)
GAMES_OBJ = $(patsubst %.c, target/%.o, $(GAMES_SRC))
GAMES_EXE = $(patsubst %.c, %.out, $(GAMES_SRC))

ENGINE_SRC = $(wildcard Engine/*.c)
ENGINE_OBJ = $(patsubst Engine/%.c, target/Engine/%.o, $(ENGINE_SRC))

CONNECT4_SRC = $(wildcard Connect4/*.c)
CONNECT4_OBJ = $(patsubst Connect4/%.c, target/Connect4/%.o, $(CONNECT4_SRC))

SINVADERS_SRC = $(wildcard SpaceInvaders/*.c)
SINVADERS_OBJ = $(patsubst SpaceInvaders/%.c, target/SpaceInvaders/%.o, $(SINVADERS_SRC))

TETRIS_SRC = $(wildcard Tetris/*.c)
TETRIS_OBJ = $(patsubst Tetris/%.c, target/Tetris/%.o, $(TETRIS_SRC))


buildall: connect4 space_invaders tetris

cleanup:
	rm $(CONNECT4_OBJ) $(SINVADERS_OBJ) $(ENGINE_OBJ) $(GAMES_OBJ) $(TETRIS_OBJ) $(GAMES_EXE)

connect4: target/connect4.o $(ENGINE_OBJ) $(CONNECT4_OBJ)
	$(CC) $(CFLAGS) $(ENGINE_OBJ) $(CONNECT4_OBJ) target/connect4.o -o connect4.out $(LIBFLAGS)

space_invaders: target/space_invaders.o $(ENGINE_OBJ) $(SINVADERS_OBJ)
	$(CC) $(CFLAGS) $(ENGINE_OBJ) $(SINVADERS_OBJ) target/space_invaders.o -o space_invaders.out $(LIBFLAGS)

tetris: target/tetris.o $(ENGINE_OBJ) $(TETRIS_OBJ)
	$(CC) $(CFLAGS) $(ENGINE_OBJ) $(TETRIS_OBJ) $< -o tetris.out $(LIBFLAGS)

target/Tetris/%.o: Tetris/%.c
	$(CC) $(CFLAGS) -c $< -o $@

target/Connect4/%.o: Connect4/%.c
	$(CC) $(CFLAGS) -c $< -o $@

target/SpaceInvaders/%.o: SpaceInvaders/%.c
	$(CC) $(CFLAGS) -c $< -o $@

target/Engine/%.o: Engine/%.c
	$(CC) $(CFLAGS) -c $< -o $@

target/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@