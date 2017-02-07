#!/bin/bash

GTEST_DIR=googletest/googletest
case `uname` in
  Linux)
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    #g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_test.cc config.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test 
    #g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread server_test.cpp reply_test.cpp reply.cpp server.cpp -lboost_system ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_test ;;
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread request_parser_test.cpp request_parser.cpp -lboost_filesystem -lboost_system ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_parser_test ;;
  Darwin)
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test ;;
  *)
    echo "Unknown operating system." ;;
esac
