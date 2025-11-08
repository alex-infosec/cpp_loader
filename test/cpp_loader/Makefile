TARGET=cpp_loader
SRC=src/main.cpp

all: $(TARGET).js

$(TARGET).js: $(SRC) src/gator.h
	em++ $(SRC) -std=c++17 -o $(TARGET).js --bind -s FETCH

debug:
	em++ $(SRC) -std=c++17 --bind -sFETCH=1 -O0 -g -sASSERTIONS=1 -o $(TARGET).js

serve:
	python3 -m http.server

clean:
	rm -f $(TARGET).js $(TARGET).wasm
