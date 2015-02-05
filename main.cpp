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
#include<exception>
#include<vector>
//#define BOOST_NO_SCOPED_ENUMS //helped by http://www.robertnitsch.de/notes/cpp/cpp11_boost_filesystem_undefined_reference_copy_file
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include<boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

std::string exec(char* cmd);
void cachefile(std::string& process_folder, std::string& filename, int pid, std::string cachefolder);
int main(int argc, char** argv){
	namespace fs = boost::filesystem;
	using namespace std;

	vector<std::string> cache(1);//will store last deleted file
	int file_index = 1;
	/*

	Getting save directory from cmdline

	*/
	string copy_folder = "/tmp/lynda";
	int choice = getopt(argc,argv,"f:");
	if (choice=='f'){
		copy_folder = optarg;
		cout << copy_folder << endl;
	}
	//creating copy folder in case it does exist
	try{
		cout << "creating folder..." << endl;
		fs::create_directory(copy_folder);
		cout << "folder creation successfull" << endl;
	}catch(exception e){
		e.what();
		cout << "folder creation failed" << endl;
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

	cout << "watching folder..." << endl;

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
						{
						//File was accessed
						//cout << "acced " << event_name<< endl;

						//check if is new
						if (!cache.empty() && cache.back()!=event_name){
							//rename old file
							const fs::path oldname(copy_folder+"/"+cache.back());
							const fs::path newname(copy_folder+"/video"+to_string(file_index)+".avi");
							try{
								cout << "renaming..." <<endl;
								fs::rename(oldname, newname);
								cout << "rename successfull" <<endl;
							}catch(exception e){
								cout << e.what() << endl;
								cout << "rename failed" <<endl;
							}
						}
						cache.clear();
						cache.push_back(event_name);
						//caching the actual file
						string result = exec(const_cast<char*>(("ls -l /proc/"+to_string(pid)+"/fd").c_str()));
						std::cout << result << " " << event_name << std::endl;
						cachefile(result,event_name, pid, copy_folder);

						break;
						}
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
/*

	Method for caching the actual deleted file
*/
void cachefile(std::string& process_folder, std::string& filename, int pid, std::string cachefolder){

	namespace fs = boost::filesystem;
	//deleting current cached file
	//fs::path oldfile(cachefolder+"/"+filename+".avi");
	//fs::rename(oldfile, fs::path("/tmp/"+filename+".avi"));
	//fs::remove(oldfile);

	std::string proc_folder = "/proc/"+std::to_string(pid)+"/fd";
	std::stringstream stream(process_folder);
	std::string line;
	while(std::getline(stream, line, '\n')){
		if (line.find(filename)!=std::string::npos){
			std::stringstream tokens(line);
			std::string filenum;
			for (int i=0; i<9; ++i){
				tokens >> filenum;
			}
			std::cout << "Moving..." << std::endl;
			const fs::path from(proc_folder+"/"+filenum);
			const fs::path to(cachefolder+"/"+filename+".avi");
			std::cout << "Move successfull." << std::endl;
			try{
				fs::copy_file(from, to,  fs::copy_option::overwrite_if_exists);
			}catch(std::exception e){
				std::cout << e.what() << std::endl;
				std::cout << "Move failed." << std::endl;
			}
		}
	}
}
