CSTD 		= -std=c++11
OPT_FLAG	= -O2 -g
LDFLAGS     = -lquantstorm -lcppkafka

all: producers consumers

producer-thread.o : producer-thread.cc
	$(CXX) $(CSTD) $(OPT_FLAG) -Wall -c $< -o $@

producers-main.o : producers-main.cc
	$(CXX) $(CSTD) $(OPT_FLAG) -Wall -c $< -o $@

producers: producers-main.o producer-thread.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	

consumer-thread.o : consumer-thread.cc
	$(CXX) $(CSTD) $(OPT_FLAG) -Wall -c $< -o $@

consumers-main.o : consumers-main.cc
	$(CXX) $(CSTD) $(OPT_FLAG) -Wall -c $< -o $@

consumers: consumers-main.o consumer-thread.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	

clean : 
	rm -f producers consumers *.o