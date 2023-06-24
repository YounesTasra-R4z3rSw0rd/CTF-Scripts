import requests
import urllib.parse
import base64
from bs4 import BeautifulSoup
import re 

# Extract the value of the cookie named 'secret' -> URL decode it -> Base64 decode it -> return the result
def extract_cookie(url, session):
    response = session.get(f'{url}/index.php')
    encoded_cookie_value = response.cookies.get('secret')
    url_decoded_cookie = urllib.parse.unquote(encoded_cookie_value)
    cookie = base64.b64decode(url_decoded_cookie).decode('utf-8')
    return cookie

url = input('Enter the url of your instance (For example: http://wcamxwl32pue3e6m5l3n94wbq36op6v4w9lxixzg-web.cybertalentslabs.com): ')
# Establishing a session:
session = requests.Session()

# POST parameter 'Q'
POST_data = {'Q': extract_cookie(url, session)}
response = session.post(f'{url}/index.php', data=POST_data)

if (response.status_code == 404):
    print('The web instance you provided is no longer reachable, go back to the challenge and reset the instance.')

# Checking if the response contains the flag:
if 'You won against sonic' in response.text:
    soup = BeautifulSoup(response.text, 'html.parser')
    pattern = re.compile(r"You won against sonic.*?}")
    flag = soup.find(text=pattern)

    # Printing the flag
    print('')
    print(f'\t-> {flag}')
