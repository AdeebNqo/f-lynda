app_name = flynda
opt = -std=c++11 -L/usr/include/ -lboost_filesystem -lboost_system
com = g++
obj = inotify-cxx.o
$(app_name): main.cpp inotify-cxx.o
	$(com) -o $(app_name) $(obj) Cacher.o main.cpp $(opt)
$(obj): inotify-cxx.h Cacher.o
	$(com) -c inotify-cxx.cpp

Cacher.o : Cacher.cpp
	$(com) -c Cacher.cpp
clean:
	rm -f *.o $(app_name) test
test: Cacher.o inotify-cxx.o
	g++ tests/main.cpp Cacher.o inotify-cxx.o -o test $(opt) -I tests/gtest/include && ./test
