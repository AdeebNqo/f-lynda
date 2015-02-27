#include "gtest/include/gtest/gtest.h"
#include "../Cacher.hpp"

/*

The methods that will be tested are as follows:

std::string exec(char* cmd);
std::string getFirefoxPID();
std::string getFirefoxMediaFileNewName(std::string oldname);
void reallyCacheFile(std::string oldname, std::string newname);
std::string getRandomString(int len); //Credit: thewall7, http://stackoverflow.com/a/11093644/1984350
bool isOkForCaching(std::string name);
void clean(std::string &value);

*/

TEST(EXEC, ECHO){
  EXPECT_EQ(std::string("hello"), Cacher::exec("echo hello"))
}

int main(){
  RUN_ALL_TESTS()
  return 0
}
