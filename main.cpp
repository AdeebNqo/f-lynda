/*

	Please see LICENSE_flynda

*/
#include<iostream>
#include<string>
#include "Cacher.hpp"
#include<exception>

int main(int argc, char** argv){
	Cacher cacher;
	std::string cwd(".");
	std::string watchFolder("/tmp/");
	cacher.setCacheFolder(cwd);
	cacher.setWatchFolder(watchFolder);
	try{
		cacher.cacheAndWAit();
	}catch(std::exception &e){
		e.what();
	}
	return 0;
}
