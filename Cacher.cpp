#include "Cacher.hpp"
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
        string event_name = event.GetName();
        uint32_t mask = event.GetMask();
        //do something here
        //cout << event_name << " : " << mask << endl;
        switch(mask){
          case IN_ACCESS:
            cout << "file accessed!"<< endl;
            break;
          case IN_ATTRIB:
            //Metadata changed—for example, permissions,
                                  //timestamps, extended attributes, link count
            cout << "changed attributes of " << event_name<< endl;
            break;
          case IN_CLOSE_WRITE:
            //File opened for writing was closed
            cout << "closed " << event_name<< endl;
            break;
          case IN_CLOSE_NOWRITE:
            //File not opened for writing was closed
            cout << "closed w/o writing " << event_name<< endl;
            break;
          case IN_CREATE:
            //File/directory created in watched directory
            cout << "created " << event_name<< endl;
            break;
          case IN_DELETE:
            //File/directory deleted from watched directory
            cout << "deleted " << event_name<< endl;
            break;
          case IN_DELETE_SELF:
            cout << "deleted folder " << event_name<< endl;
            //Watched file/directory was itself deleted.
            break;
          case IN_MODIFY:
            cout << "modified " << event_name<< endl;
            //File was modified
            break;
          case IN_MOVE_SELF:
            cout << "moved folder " << event_name<< endl;
            //Watched file/directory was itself moved.
            break;
          case IN_MOVED_FROM:
            cout << "renamed " << event_name<< endl;
            //Generated for the directory containing the old
                                  //filename when a file is renamed
            break;
          case IN_MOVED_TO:
            cout << "renamed " << event_name<< endl;
            //Generated for the directory containing the new
                                  //filename when a file is renamed
            break;
          case IN_OPEN:
            cout << "opened " << event_name<< endl;
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
