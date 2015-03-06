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
  Cacher cacher;
  char  cmd[] = "echo hello";
  std::string result("hello\n");
  EXPECT_EQ(result, cacher.exec(cmd));
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
