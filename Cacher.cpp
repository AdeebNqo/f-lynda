#include<iostream>
#include <stdio.h>
#include "Cacher.hpp"
#include<exception>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;
namespace fs = boost::filesystem;
Cacher::Cacher(){

}
void Cacher::setWatchFolder(string &folder){
  watchFolder = folder;
}
void Cacher::setCacheFolder(string &folder){
  cacheFolder = folder;
  //creating cache folder
  fs::path rootPath ( cacheFolder+"/flynda/" );
  boost::system::error_code returnedError;
  fs::create_directories( rootPath, returnedError );
  if ( returnedError )
     //did not successfully create directories
     cout << "could not create folder"<< endl;
}
void Cacher::cacheAndWAit(){
  Inotify notify;
  InotifyWatch watch(watchFolder, IN_ALL_EVENTS);
  notify.SetNonBlock(false);
  notify.Add(watch);
  cout << "Watching "<< watchFolder << "..."<< endl;

  string currentSongName = getRandomString(20);

  while(true){
    notify.WaitForEvents();
    //dealing with fired events
    int num_events = notify.GetEventCount();
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
            if (isOkForCaching(filename)){
              cout << "caching "<< filename<< endl;
              reallyCacheFile(filename, currentSongName);
            }
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
            if (isOkForCaching(filename)){
              cout << "caching "<< filename<< endl;
              currentSongName = getRandomString(20);
              reallyCacheFile(filename, currentSongName);
            }
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
            //cout << "modified " << filename<< endl;
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
  *std::remove(cmd, cmd+strlen(cmd), '\n') = '\0';
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

string Cacher::getFirefoxPID(){
  string cmd("ps -A | grep firefox | awk '{$2=$2}1' | cut -d ' ' -f 1");
  string result = exec(const_cast<char*>(cmd.c_str()));
  return result;
}

string Cacher::getFirefoxMediaFileNewName(std::string oldname){
  string cmd("ls -l /proc/"+string(getFirefoxPID())+"/fd | grep "+oldname+" | tr -s ' ' | cut -d' ' -f 9");
  return exec(const_cast<char*>(cmd.c_str()));
}

void Cacher::reallyCacheFile(std::string oldname, std::string newname){
  string newpath = cacheFolder+"/flynda/"+newname;
  try{
    string oldpath = "/proc/"+getFirefoxPID()+"/fd/"+getFirefoxMediaFileNewName(oldname);
    clean(oldpath);
    clean(newpath);
    fs::path resolved(oldpath);
    fs::path newfilepath(newpath);
    cout <<"copying from "<<oldpath << endl;
    cout << "copying to "<<newpath<< endl;
    if (fs::file_size(resolved)>fs::file_size(newpath)){
      fs::copy_file(resolved,newfilepath,fs::copy_option::overwrite_if_exists);
    }
    //fs::copy_file(resolved,newfilepath,fs::copy_option::overwrite_if_exists);
  }catch(exception &e){
    string reason = e.what();
    cout << reason << endl;

    std::string prefix("boost::filesystem::file_size: No such file or directory:");
    if (!reason.compare(0, prefix.size(), prefix)){
      std::ofstream outfile (newpath.c_str());
      outfile.close();
    }
  }
}

string Cacher::getRandomString(int len)
{
   srand(time(0));
   string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   int pos;
   while(str.size() != len) {
    pos = ((rand() % (str.size() - 1)));
    str.erase (pos, 1);
   }
   return str;
}
bool Cacher::isOkForCaching(std::string name){
  clean(name);
  cout << "testing "<<name<< endl;
  string firefoxPID = getFirefoxPID();
  string newname = getFirefoxMediaFileNewName(name);
  string cmd("file /proc/"+firefoxPID+"/fd/"+newname);
  string expectedResult = "/proc/"+firefoxPID+"/fd/"+newname+": broken symbolic link to `/tmp/"+name+" (deleted)'";
  string finalResult = exec(const_cast<char*>(cmd.c_str()));
  clean(expectedResult);
  clean(finalResult);
  return finalResult == expectedResult;
}

void Cacher::clean(std::string &value){
  value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
}
