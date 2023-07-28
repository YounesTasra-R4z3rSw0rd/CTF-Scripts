## About:
* **Platform**&nbsp;&nbsp;&nbsp;: HackTheBox
* **Challenge**&nbsp;: Beep
* **Difficulty**&nbsp;&nbsp;: Easy
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://app.hackthebox.com/machines/Beep](https://app.hackthebox.com/machines/Beep)

## Description:
This script brute forces the webmin login page at [http://10.10.10.7:1000/](http://10.10.10.7:1000/) using a [username](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/blob/main/HTB/Beep/Webmin/usernames.txt) and [password](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/blob/main/HTB/Beep/Webmin/passwords.txt) wordlist I created from the `/etc/amportal.conf` file.<br/>
This script employs a method to circumvent the rate limiting imposed on consecutive failed authentication attempts. It achieves this by deliberately introducing a 60-second delay (sleep) between each unsuccessful login attempt before continuing with the brute force process.

## Usage:
```bash
python3 bruteforce-login.py
```
![2023-07-28 19_43_46-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/795c7d79-3530-4ba2-b098-b75ef9a420af)

## Extra:
### Shell as root:
After getting access to the webmin dashboard, you can get a reverse shell by creating a command schedule via the `Scheduled Commands` feature under the `System` tab:
![2023-07-28 19_44_40-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/3e709754-0da8-4f3c-94b5-a37c70702319)
![2023-07-28 19_46_48-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/c7320490-97bd-46b0-a0eb-706c4236dc81)
<br/>After clicking on `Create`, go ahead and start a netcat listener on the port specified in the reverse shell command (e.g.; 9999) and then wait for the command to execute as `root`:
![2023-07-28 19_48_16-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/b53bc629-54a0-49c5-b9bf-cf5c4dde3df6)
