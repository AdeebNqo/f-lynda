app_name = flynda
opt = -std=c++11 -L/usr/include/ -lboost_filesystem -lboost_system
com = g++
obj = inotify-cxx.o
$(app_name): main.cpp inotify-cxx.o
	$(com) -o $(app_name) $(obj) main.cpp $(opt)
$(obj): inotify-cxx.h
	$(com) -c inotify-cxx.cpp
	
