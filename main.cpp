/*

	Please see LICENSE_flynda

*/
#include "inotify-cxx.h"
#include<string>
#include<iostream>
int main(){
	using namespace std;
	/*
	
	watching a directory
	
	*/
	string dir = "/tmp";
	string dir2 = "/proc/33/fd";
	
	//creating the watcher -- adding directory
	Inotify notify;
	InotifyWatch watch(dir, IN_ALL_EVENTS);
	notify.setNonBlock(false);
	notify.Add(watch);
	
	//wating for a response
	while(true){
		notify.WaitForEvents();
		//dealing with fired events
		num_events = notify.GetEventCount();
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
	return 0;
}
