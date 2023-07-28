## About:
* **Platform**&nbsp;&nbsp;&nbsp;: HackTheBox
* **Challenge**&nbsp;: Beep
* **Difficulty**&nbsp;&nbsp;: Easy
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://app.hackthebox.com/machines/Beep](https://app.hackthebox.com/machines/Beep)

## Description:
This is a proof of concept of FreePBX 2.10.0 / Elastix 2.2.0 - Remote Code Execution vulnerability identified as [CVE-2012-4869](https://nvd.nist.gov/vuln/detail/CVE-2012-4869)

## Reference:
[+] [https://www.exploit-db.com/exploits/18650](https://www.exploit-db.com/exploits/18650)

## Usage:
1. Start a netcat listener on a port number specified as `LPORT` (e.g.; `9999`) 
```bash
$ nc -nlvp 9999
```
2. Run the POC:
```bash
python3 cve-2012-4869.py
```
3. After some time, you should get back a reverse shell (when the extension is equal to 233):
![2023-07-28 17_17_29-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/3a182079-f545-40ba-a806-616d356bcb56)

* Reverse shell:
![2023-07-28 17_18_31-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/013581c6-6343-4c57-a4ed-2b76fa187033)


