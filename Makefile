CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11
SRCFILES = server.cpp reply.cpp request_parser.cpp config.cc config_parser.cc
GTEST_DIR = googletest/googletest

all: CXXFLAGS += $(CXXOPTIMIZE)
all: SRCFILES += main.cpp
all: webserver

test: CXXFLAGS += -isystem ${GTEST_DIR}/include
test: SRCFILES += config_parser_test.cc config_test.cc server_test.cpp reply_test.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a
test: | clean webserver_test

run:
	./webserver

run_coverage:
	./webserver_test
	gcov -r server.cpp reply.cpp config.cc config_parser.cc

webserver:
	$(CXX) -o $@ $^ $(CXXFLAGS) $(SRCFILES) -lboost_filesystem -lboost_system

webserver_test: 
	$(CXX) $(CXXFLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CXX) -o $@ $^ $(CXXFLAGS) -fprofile-arcs -ftest-coverage $(SRCFILES) -lboost_filesystem -lboost_system


clean:
	rm -rf *.o webserver webserver_test

