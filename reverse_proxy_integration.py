import subprocess
import os
import signal

# Spawn a shell process to run the server with proxy
run_server_command = "./webserver config"
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE, shell=True, preexec_fn=os.setsid)

# Spawn a shell process to curl to proxy handler endpoint
proxy_request_command = "curl -i localhost:123/proxy"
proxy_request_proc = subprocess.Popen(proxy_request_command, stdout=subprocess.PIPE, shell=True)
proxy_response = proxy_request_proc.stdout.read().decode('utf-8')


