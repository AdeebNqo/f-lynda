app_name = "flynda"
opt = "-std=c++11"
com = g++
obj = inotify-cxx.o
$(app_name): main.cpp inotify-cxx.o
	$(com) $(opt) -o $(app_name) $(obj) main.cpp
$(obj): inotify-cxx.h
	$(com) -c inotify-cxx.cpp
	
