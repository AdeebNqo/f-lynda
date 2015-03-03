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

  std::string exec(char* cmd);
  std::string getFirefoxPID();
  std::string getFirefoxMediaFileNewName(std::string oldname);
  void reallyCacheFile(std::string oldname, std::string newname);
  std::string getRandomString(int len); //Credit: thewall7, http://stackoverflow.com/a/11093644/1984350
  bool isOkForCaching(std::string name);
  void clean(std::string &value);
};
#endif
