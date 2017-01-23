#include "gtest/gtest.h"
#include "config.h"

TEST(ServerConfigTest, SimpleConfig) {
  ServerConfig config("test_config_1");
  EXPECT_EQ(config.GetPortNo(), 12345);
}

TEST(ServerConfigTest, PortTooLargeConfig) {
  try{
  	ServerConfig config("test_config_2");
  	EXPECT_EQ(config.GetPortNo(), 80);
  }
  catch (PortRangeException &e)
  {
	std::cout << e.what();
  }
}

TEST(ServerConfigTest, NoPortConfig) {
  try{
  	ServerConfig config("test_config_3");
  	EXPECT_EQ(config.GetPortNo(), 80);
  }
  catch (InvalidConfigException &e)
  {
	std::cout << e.what();
  }
}

TEST(ServerConfigTest, EmptyConfig) {
  try{
  	ServerConfig config("test_config_4");
  	EXPECT_EQ(config.GetPortNo(), 80);
  }
  catch (InvalidConfigException &e)
  {
	std::cout << e.what();
  }
}
