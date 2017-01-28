#!/bin/bash

# Compiling the webserver
echo "Building webserver."
make

# starting webserver
{	
	echo "Starting webserver."
	timeout 10 ./webserver test_config_1
	echo "Stopping webserver."
}&
sleep 3

# testing webserver
echo "Running command curl -D headers.txt http://localhost:12345 > response.txt"
curl -D headers.txt http://localhost:12345 > response.txt

# checking reponse
echo "Checking response."
if grep -q "HTTP/1.1 200 OK" headers.txt;then
	if grep -q "Content-Type: text/plain" headers.txt;then
		#Checking if content-length is correct
		count=`wc -c response.txt | awk '{print $1}'`
		if grep -q "Content-Length: $count" headers.txt;then
			echo "All response headers are correct."
		else
			echo "HTTP response does not contain correct Content-Length."
			exit 12
		fi
	else
		echo "HTTP response does not contain Content-Type or is not set to text/plain."
		exit 11
	fi
else
	echo "HTTP response does not contain 200 OK status code."
	exit 10
fi

# waiting for webserver to stop
sleep 8

# removing temporary headers file
echo "Removing headers.txt and response.txt."
rm headers.txt
rm response.txt
sleep 1

# cleaning up
echo "Removing webserver executable and cleaning up."
make clean
sleep 1

# exiting with success message
echo "All tests passing. Well done!"
exit 0
