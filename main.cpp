/*

	Please see LICENSE_flynda

*/
#include "inotify-cxx.h"
#include<string>
#include<iostream>
#include <stdio.h>
#include<thread>
#include <sstream>
#include<unistd.h>
#include<vector>
std::string exec(char* cmd);
int main(int argc, char** argv){
	using namespace std;

	vector<std::string> cache;//will store last deleted file

	/*
	
	Getting save directory from cmdline

	*/
	string copy_folder = "/tmp/lynda";
	int choice = getopt(argc,argv,"f:");
	if (choice=='f'){
		copy_folder = optarg;
		cout << copy_folder << endl;
	}
	/*
	
	watching a directory
	
	*/
	string folder = "/tmp";
	string folder2 = "/proc/33/fd";

	//finding pid of the l**da applet thingie
	char cmd[] = "lsof -n | grep Flash";
	string result = exec(cmd);
	stringstream ss(result);
	string null_stream;
	int pid;	
	
	ss >> null_stream;
	ss >> pid;
	cout << result << endl;
	cout << pid << "\n\nNow watching tmp folder..."<< endl;
	
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
				//cout << event_name << " : " << mask << endl;
				switch(mask){
					case IN_ACCESS:
						//File was accessed
						cout << "acced " << event_name<< endl;
						exec(const_cast<char*>(("mv /tmp/"+event_name+" "+copy_folder).c_str()));
						cache[0] = event_name;
						break;
					case IN_ATTRIB:
						//Metadata changed—for example, permissions,
                             			//timestamps, extended attributes, link count
						//cout << "changed attributes of " << event_name<< endl;
						break;
					case IN_CLOSE_WRITE:
						//File opened for writing was closed
						//cout << "closed " << event_name<< endl;
						break;
					case IN_CLOSE_NOWRITE:
						//File not opened for writing was closed
						//cout << "closed w/o writing " << event_name<< endl;
						break;
					case IN_CREATE:
						//File/directory created in watched directory
						//cout << "created " << event_name<< endl;
						break;
					case IN_DELETE:
						//File/directory deleted from watched directory
						//cout << "deleted " << event_name<< endl;
						break;
					case IN_DELETE_SELF:
						//cout << "deleted folder " << event_name<< endl;
						//Watched file/directory was itself deleted.
						break;
					case IN_MODIFY:
						//cout << "modified " << event_name<< endl;
						//File was modified
						break;
					case IN_MOVE_SELF:
						//cout << "moved folder " << event_name<< endl;
						//Watched file/directory was itself moved.
						break;
					case IN_MOVED_FROM:
						//cout << "renamed " << event_name<< endl;
						//Generated for the directory containing the old
                             			//filename when a file is renamed
						break;
					case IN_MOVED_TO:
						//cout << "renamed " << event_name<< endl;
						//Generated for the directory containing the new
                             			//filename when a file is renamed
						break;
					case IN_OPEN:
						//cout << "opened " << event_name<< endl;
						//File was opened
						break;
				}
			}
			else{
				//event retrieval failed
				cerr << "event retrieval failed" << endl;
			}
		}
	}
	
	return 0;
}
/*

	Method stolen from @waqas to save time
	http://stackoverflow.com/a/478960

	Method for executing a linux cmd

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
