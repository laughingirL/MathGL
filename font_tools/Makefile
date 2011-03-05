OBJ  = make_font.o
BIN  = make_font
#FLAGS = -O2 -DSMALL_SET -Wall -I/usr/include/freetype2
FLAGS = -O2 -Wall -I/usr/include/freetype2

all: $(BIN)

clean:
	rm -f $(OBJ) $(BIN) *~

# cpp: $(OBJ)
# 	g++ -DSMALL_SET -o make_font $(OBJ) -lfreetype -lftgl -lGL

$(BIN): $(OBJ)
	g++ -o $(BIN) $(OBJ) -L/opt/local/lib -lfreetype -lftgl

make_font.o: make_font.cpp 
	g++ $(FLAGS) -c -o make_font.o make_font.cpp
