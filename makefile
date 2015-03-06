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
	@rm -f *.o $(app_name) test
test: Cacher.o inotify-cxx.o
	wget http://googletest.googlecode.com/files/gtest-1.7.0.zip
	unzip gtest-1.7.0.zip
	rm -f gtest-1.7.0.zip
	mkdir -p tests/gtest
	cp -Rf gtest-1.7.0/* tests/gtest
	rm -rf gtest-1.7.0
	cd ./tests/gtest && ./configure
	make -C ./tests/gtest/
	@g++ tests/main.cpp Cacher.o inotify-cxx.o -o test $(opt) -lgtest -Itests/gtest/include -Ltests/gtest/lib/.libs && export LD_LIBRARY_PATH=tests/gtest/lib/.libs && ./test
