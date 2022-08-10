# Global Makefile for make-ing outside of src/

FLAGS = -ansi -std=c99 -Wall -g
TARFILES = README.md ./src ./doc ./bin COPYRIGHT.txt INSTALL.txt Makefile
GTKC = `pkg-config --cflags gtk+-2.0`
GTKL = `pkg-config --libs gtk+-2.0`
all: ./bin/Login ./bin/server

test: ./bin/Chess
	./bin/Chess

tar: ./bin/server ./bin/Login
	tar -zcvf Chat_Alpha.tar.gz $(TARFILES)


Login: ./bin/Login
	./bin/Login 

Chat: ./bin/Chat
	./bin/Chat

ChatWinTest: ./bin/ChatWinTest ./src/ChatWinTest.o
	./bin/ChatWinTest

TestChess: ./bin/TestChess
	./bin/TestChess

Main: ./bin/ChatAndChess

Server: ./bin/server

./bin/ChatAndChess: ./src/LoginWin.o ./src/Main.o ./src/ClientWin.o ./src/ChatWin.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Check.o ./src/Movement.o
	gcc ./src/LoginWin.o ./src/Main.o ./src/ClientWin.o ./src/ChatWin.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Check.o ./src/Movement.o -o ./bin/ChatAndChess $(FLAGS) $(GTKL)
	
./bin/Chat: ./src/ChatWin.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Main.o
	gcc ./src/ChatWin.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Main.o ./src/LoginWin.o ./src/ClientWin.o -o ./bin/Chat $(FLAGS) $(GTKL)

./bin/ChatWinTest: ./src/ChatWinTest.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Check.o
	gcc ./src/ChatWinTest.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Movement.o ./src/Check.o -o ./bin/ChatWinTest $(FLAGS) $(GTKL)

./src/Main.o: ./src/Main.c ./src/Main.h ./src/LoginWin.h ./src/ClientWin.h ./src/ChatWin.h
	gcc -c ./src/Main.c -o ./src/Main.o $(FLAGS) $(GTKC)

./src/ChatWin.o: ./src/ChatWin.c ./src/ChatWin.h ./src/Board.h ./src/Piece.h ./src/Square.h ./src/Main.h ./src/LoginWin.h ./src/ClientWin.h
	gcc -c ./src/ChatWin.c -o ./src/ChatWin.o $(FLAGS) $(GTKC)

./src/ChatWinTest.o: ./src/ChatWinTest.c ./src/ChatWinTest.h ./src/Board.h ./src/Piece.h ./src/Square.h ./src/Movement.h ./src/Check.h
	gcc -c ./src/ChatWinTest.c -o ./src/ChatWinTest.o $(FLAGS) $(GTKC)
	
./bin/ClientWin: ./src/ClientWin.o
	gcc ./src/ClientWin.o -o ./bin/ClientWin $(FLAGS) $(GTKL)

./src/ClientWin.o: ./src/ClientWin.c ./src/ClientWin.h
	gcc -c ./src/ClientWin.c -o ./src/ClientWin.o $(FLAGS) $(GTKC)

./src/LoginWin.o: ./src/LoginWin.c ./src/LoginWin.h
	gcc -c ./src/LoginWin.c -o ./src/LoginWin.o $(FLAGS) $(GTKC)

./bin/server: ./src/server.o
	gcc ./src/server.o -o ./bin/server $(FLAGS)

./src/server.o: ./src/server.c
	gcc -c ./src/server.c -o ./src/server.o $(FLAGS)

./bin/servertest: ./src/servertest.o
	gcc ./src/servertest.o -o ./bin/servertest $(FLAGS)

./src/servertest.o: ./src/servertest.c
	gcc -c ./src/servertest.c -o ./src/servertest.o $(FLAGS)

# Chess portion below
./bin/TestChess: ./src/TestChess.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Movement.o ./src/Check.o
	gcc  ./src/TestChess.o ./src/Movement.o ./src/Board.o ./src/Piece.o ./src/Square.o ./src/Check.o -o ./bin/TestChess $(FLAGS) $(GTKL)

./src/TestChess.o: ./src/TestChess.c ./src/Board.h ./src/Piece.h ./src/Square.h ./src/Movement.h
	gcc -c ./src/TestChess.c -o ./src/TestChess.o $(FLAGS) $(GTKC)

./bin/Chess: ./src/Chess.o ./src/Board.o ./src/Movement.o ./src/Square.o ./src/Piece.o ./src/Check.o ./src/Scoring.o ./src/AI_PosMoves.o ./src/Checkmate.o ./src/AI_MinMax.o
	gcc ./src/Chess.o ./src/Board.o ./src/Movement.o ./src/Square.o ./src/Piece.o ./src/Check.o ./src/Scoring.o ./src/AI_PosMoves.o ./src/Checkmate.o ./src/AI_MinMax.o -o ./bin/Chess $(FLAGS)	

./src/Chess.o: ./src/Chess.c ./src/Chess.h ./src/Board.h ./src/Piece.h ./src/Square.h ./src/Movement.h ./src/AI_MinMax.h ./src/Check.h
	gcc -c ./src/Chess.c -o ./src/Chess.o $(FLAGS)

./src/Board.o: ./src/Board.c ./src/Board.h ./src/Piece.h ./src/Square.h
	gcc -c ./src/Board.c -o ./src/Board.o $(FLAGS) 

./src/Movement.o: ./src/Movement.c ./src/Movement.h ./src/Square.h ./src/Board.h ./src/Check.h
	gcc -c ./src/Movement.c -o ./src/Movement.o $(FLAGS) $(GTKC)

./src/Piece.o: ./src/Piece.h ./src/Piece.c 
	gcc -c ./src/Piece.c -o ./src/Piece.o $(FLAGS)

./src/Square.o: ./src/Square.c ./src/Square.h ./src/Piece.h
	gcc -c ./src/Square.c -o ./src/Square.o $(FLAGS)

./src/Check.o: ./src/Check.c ./src/Check.h ./src/Piece.h ./src/Movement.h ./src/Board.h ./src/Square.h
	gcc -c ./src/Check.c -o ./src/Check.o $(FLAGS)

./src/Checkmate.o: ./src/Checkmate.c ./src/Checkmate.h ./src/Piece.h ./src/Board.h ./src/Square.h ./src/Movement.h ./src/Scoring.h ./src/Check.h
	gcc -c ./src/Checkmate.c -o ./src/Checkmate.o $(FLAGS)
		
./src/AI_PosMoves.o: ./src/AI_PosMoves.c ./src/AI_PosMoves.h ./src/Piece.h ./src/Movement.h ./src/Board.h ./src/Square.h ./src/Scoring.h
	gcc -c ./src/AI_PosMoves.c -o ./src/AI_PosMoves.o $(FLAGS)

./src/Scoring.o: ./src/Scoring.c ./src/Scoring.h ./src/Piece.h ./src/Movement.h ./src/Board.h ./src/Square.h
	gcc -c ./src/Scoring.c -o ./src/Scoring.o $(FLAGS)

./src/AI_MinMax.o: ./src/AI_MinMax.c ./src/AI_MinMax.h ./src/AI_PosMoves.h ./src/Piece.h ./src/Movement.h ./src/Board.h ./src/Square.h ./src/Check.h ./src/Checkmate.h
	gcc -c ./src/AI_MinMax.c -o ./src/AI_MinMax.o $(FLAGS)


# Remove the following files
clean: 
	rm -f ./src/*.o ./bin/Chess ./bin/Login ./bin/Chat ./bin/server ./bin/ChatAndChess