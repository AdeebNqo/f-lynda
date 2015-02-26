#include<iostream>
#include <stdio.h>
#include "Cacher.hpp"
#include <sstream>
using namespace std;
namespace fs = boost::filesystem;
Cacher::Cacher(){

}
void Cacher::setWatchFolder(string &folder){
  watchFolder = folder;
}
void Cacher::setCacheFolder(string &folder){
  cacheFolder = folder;
}
void Cacher::cacheAndWAit(){
  Inotify notify;
  cout << "watching: "<<watchFolder<<endl;
  InotifyWatch watch(watchFolder, IN_ALL_EVENTS);
  notify.SetNonBlock(false);
  notify.Add(watch);

  while(true){
    notify.WaitForEvents();
    //dealing with fired events
    int num_events = notify.GetEventCount();
    cout << "got "<< num_events<<" events"<< endl;
    for (int i=num_events; i>0; --i){
      InotifyEvent event;
      //getting current fired event
      if (notify.GetEvent(&event)){
        string filename = event.GetName();
        uint32_t mask = event.GetMask();
        //do something here
        //cout << filename << " : " << mask << endl;
        switch(mask){
          case IN_ACCESS:
            cout << "file accessed!"<< endl;
            break;
          case IN_ATTRIB:
            //Metadata changed—for example, permissions,
                                  //timestamps, extended attributes, link count
            cout << "changed attributes of " << filename<< endl;
            break;
          case IN_CLOSE_WRITE:
            //File opened for writing was closed
            cout << "closed " << filename<< endl;
            break;
          case IN_CLOSE_NOWRITE:
            //File not opened for writing was closed
            cout << "closed w/o writing " << filename<< endl;
            break;
          case IN_CREATE:
            //File/directory created in watched directory
            cout << "created " << filename<< endl;
            cout << exec(const_cast<char*>((string("file ")+watchFolder+filename).c_str())) << endl;
            break;
          case IN_DELETE:
            //File/directory deleted from watched directory
            cout << "deleted " << filename<< endl;
            break;
          case IN_DELETE_SELF:
            cout << "deleted folder " << filename<< endl;
            //Watched file/directory was itself deleted.
            break;
          case IN_MODIFY:
            cout << "modified " << filename<< endl;
            //File was modified
            break;
          case IN_MOVE_SELF:
            cout << "moved folder " << filename<< endl;
            //Watched file/directory was itself moved.
            break;
          case IN_MOVED_FROM:
            cout << "renamed " << filename<< endl;
            //Generated for the directory containing the old
                                  //filename when a file is renamed
            break;
          case IN_MOVED_TO:
            cout << "renamed " << filename<< endl;
            //Generated for the directory containing the new
                                  //filename when a file is renamed
            break;
          case IN_OPEN:
            cout << "opened " << filename<< endl;
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
}

std::string Cacher::exec(char* cmd){
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

int Cacher::getFirefoxPID(){
  string cmd("ps -A | grep firefox | cut -d' ' -f 1");
  istringstream buffer(exec(const_cast<char*>(cmd.c_str())));
  int result;
  buffer >> result;
  return result;
}

string Cacher::getFirefoxMediaFileNewName(std::string oldname){
  string cmd("ls -l /proc/26849/fd | grep "+oldname+" | tr -s ' ' | cut -d' ' -f 9");
  return exec(const_cast<char*>(cmd.c_str()));
}
