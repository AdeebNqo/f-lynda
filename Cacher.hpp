#ifndef _CACHER
#define _CACHER

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include<boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include "inotify-cxx.h"
#include<string>
namespace fs = boost::filesystem;
class Cacher{
public:
  Cacher();
  void setWatchFolder(std::string &folder);
  void setCacheFolder(std::string &folder);
  void cacheAndWAit();
  std::string watchFolder;
  std::string cacheFolder;
private:
  std::string exec(char* cmd);
  int getFirefoxPID();
  std::string getFirefoxMediaFileNewName(std::string oldname);
};
#endif
