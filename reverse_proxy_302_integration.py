import subprocess
import os
import signal

# Spawn a shell process to run the server with proxy
run_server_command = "./webserver proxy_302_config"
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE, shell=True, preexec_fn=os.setsid)

# Make a direct curl request to www.ucla.edu
direct_request_command = "curl -s www.ucla.edu"
direct_request_proc = subprocess.Popen(direct_request_command, stdout=subprocess.PIPE, shell=True)
direct_response, direct_err = direct_request_proc.communicate()

# Make a curl request to reverse proxy handler that will hit ucla.edu,
# which in turn returns a 302 response to our handler, redirecting us to www.ucla.edu
# Our handler should then make a request to www.ucla.edu, 
# and return its response to the client.
proxy_request_command = "curl -s localhost:12345/proxy"
proxy_request_proc = subprocess.Popen(proxy_request_command, stdout=subprocess.PIPE, shell=True)
proxy_response, proxy_err = proxy_request_proc.communicate()

# Terminate the server before exiting
os.killpg(os.getpgid(server_proc.pid), signal.SIGTERM)

if proxy_response != direct_response:
    print("ERROR: Proxy handler replied with a different response than a direct response")
    exit(1)
else:
    print("SUCCESS: Proxy handler replied with the same response as a direct response")
    exit(0)