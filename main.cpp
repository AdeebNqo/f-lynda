/*

	Please see LICENSE_flynda

*/
#include "inotify-cxx.h"
#include<string>
#include<iostream>
#include <stdio.h>
#include<thread>
#include <sstream>
void watch(std::string& folder);
std::string exec(char* cmd);
int main(){
	using namespace std;
	/*
	
	watching a directory
	
	*/
	string dir = "/tmp";
	string dir2 = "/proc/33/fd";

	//start by fidnding pid of the l**da applet thingie
	char cmd[] = "lsof -n | grep Flash";
	string result = exec(cmd);
	stringstream ss(result);
	string null_stream;
	int pid;	
	
	ss >> null_stream;
	ss >> pid;
	cout << result << endl;
	cout << pid << endl;
 	//thread(watch,dir);
	return 0;
}
/*
	Method for watching the provided directory
	for any event fired events

*/
void watch(std::string& folder){
	using namespace std;
	//creating the watcher -- adding directory
	Inotify notify;
	InotifyWatch watch(folder, IN_ALL_EVENTS);
	notify.SetNonBlock(false);
	notify.Add(watch);
	
	//wating for a response
	while(true){
		notify.WaitForEvents();
		//dealing with fired events
		int num_events = notify.GetEventCount();
		for (int i=num_events; i>0; --i){
			InotifyEvent event;
			//getting current fired event
			if (notify.GetEvent(&event)){
				string event_name = event.GetName();
				uint32_t mask = event.GetMask();
				//do something here
				cout << event_name << " : " << mask << endl;
			}
			else{
				//event retrieval failed
				cerr << "event retrieval failed" << endl;
			}
		}
	}
}
/*

	Method stolen from @waqas to save time
	http://stackoverflow.com/a/478960

*/
std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}
