
CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++17
LFLAGS = -pthread

SRC = src

LIB = $(SRC)/lib
STATS = $(SRC)/stats
V1 = $(SRC)/monitor/v1
V2 = $(SRC)/monitor/v2
V3 = $(SRC)/monitor/v3

BUILD = build

OBJ = $(BUILD)/obj
BIN = $(BUILD)/bin

OUT = $(BIN)/main1 $(BIN)/main2 $(BIN)/main3

all: $(BUILD) $(BIN) $(OBJ) $(OUT)

$(BUILD) $(BIN) $(OBJ):
	mkdir -p $@

$(BIN)/main1: $(OBJ)/main1.o $(OBJ)/santa_v1.o $(OBJ)/cnt_condition_variable.o $(OBJ)/utilities.o $(OBJ)/statistics.o
	$(CC) -o $@ $^ $(LFLAGS)

$(BIN)/main2: $(OBJ)/main2.o $(OBJ)/santa_v2.o $(OBJ)/cnt_condition_variable.o $(OBJ)/utilities.o $(OBJ)/statistics.o
	$(CC) -o $@ $^ $(LFLAGS)

$(BIN)/main3: $(OBJ)/main3.o $(OBJ)/santa_v3.o $(OBJ)/cnt_condition_variable.o $(OBJ)/utilities.o $(OBJ)/statistics.o
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ)/main1.o: $(SRC)/main1.cpp
	$(CC) -I$(STATS) -I$(LIB) -I$(V1) $(CFLAGS) -c -o $@ $<

$(OBJ)/main2.o: $(SRC)/main2.cpp
	$(CC) -I$(STATS) -I$(LIB) -I$(V2) $(CFLAGS) -c -o $@ $<

$(OBJ)/main3.o: $(SRC)/main3.cpp
	$(CC) -I$(STATS) -I$(LIB) -I$(V3) $(CFLAGS) -c -o $@ $<

$(OBJ)/statistics.o: $(STATS)/statistics.cpp $(STATS)/statistics.hpp
	$(CC) -I$(STATS) $(CFLAGS) -c -o $@ $<

$(OBJ)/utilities.o: $(LIB)/utilities.cpp $(LIB)/utilities.hpp
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

$(OBJ)/cnt_condition_variable.o: $(LIB)/cnt_condition_variable.cpp $(LIB)/cnt_condition_variable.hpp
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

$(OBJ)/santa_v1.o: $(V1)/santa_v1.cpp $(V1)/santa_v1.hpp
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

$(OBJ)/santa_v2.o: $(V2)/santa_v2.cpp $(V2)/santa_v2.hpp
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

$(OBJ)/santa_v3.o: $(V3)/santa_v3.cpp $(V3)/santa_v3.hpp
	$(CC) -I$(LIB) -I$(STATS) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD)
