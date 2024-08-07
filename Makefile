CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -pthread 
LIB = lib
STATS = stats
V1 = monitor/v1
V2 = monitor/v2
V3 = monitor/v3

OUT = main1 main2 main3 main3multi

all: $(OUT)

main3multi: main3multi.o statistics.o
	$(CC) -Istats -o $@ $^ $(LFLAGS)

main1: main1.o santa_v1.o cnt_condition_variable.o utilities.o statistics.o
	$(CC) -o $@ $^ $(LFLAGS)

main2: main2.o santa_v2.o cnt_condition_variable.o utilities.o statistics.o
	$(CC) -o $@ $^ $(LFLAGS)

main3: main3.o santa_v3.o cnt_condition_variable.o utilities.o statistics.o
	$(CC) -o $@ $^ $(LFLAGS)

main3multi.o: main3multi.cpp
	$(CC) -I$(STATS) $(CFLAGS) -c -o $@ $<

main1.o: main1.cpp
	$(CC) -I$(STATS) -I$(LIB) -I$(V1) $(CFLAGS) -c -o $@ $<

main2.o: main2.cpp
	$(CC) -I$(STATS) -I$(LIB) -I$(V2) $(CFLAGS) -c -o $@ $<

main3.o: main3.cpp
	$(CC) -I$(STATS) -I$(LIB) -I$(V3) $(CFLAGS) -c -o $@ $<

statistics.o: $(STATS)/statistics.cpp $(STATS)/statistics.h
	$(CC) -I$(STATS) $(CFLAGS) -c -o $@ $<

utilities.o: $(LIB)/utilities.cpp $(LIB)/utilities.h
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

cnt_condition_variable.o: $(LIB)/cnt_condition_variable.cpp $(LIB)/cnt_condition_variable.h
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

santa_v1.o: $(V1)/santa_v1.cpp $(V1)/santa_v1.h
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

santa_v2.o: $(V2)/santa_v2.cpp $(V2)/santa_v2.h
	$(CC) -I$(LIB) $(CFLAGS) -c -o $@ $<

santa_v3.o: $(V3)/santa_v3.cpp $(V3)/santa_v3.h
	$(CC) -I$(LIB) -I$(STATS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *~ $(OUT)

