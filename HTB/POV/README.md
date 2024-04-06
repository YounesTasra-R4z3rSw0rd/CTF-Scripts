## About:
* **Platform**&nbsp;&nbsp;&nbsp;: HackTheBox
* **Challenge**&nbsp;: Pov
* **Difficulty**&nbsp;&nbsp;: Medium
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://app.hackthebox.com/machines/Pov](https://app.hackthebox.com/machines/Pov)

## Description:
This C++ script abuses the SeDebugPrivilege token to gain a shell as NT AUTHORITY\SYSTEM on the box. This is done by injecting a reverse shell msfvenom-generated shellcode into the memory address space of a highly privileged process such as `lsass.exe` and creating a thread on the remote process that will run this shellcode.

## High Level Overview:

1. Retrieve the PID of the target process using the function `FindTargetProc()`. In this case, it would be system processes such as `lsass.exe`, `services.exe`, `winlogon.exe` etc.
2. Enable `SeDebugPrivilege` token by calling `AdjustTokenPrivileges`
3. Get handle to the target process, by calling `OpenProcess`
4. Allocate a buffer (**RW**) in the memory address space of the target process, by calling `VirtualAllocEx`
5. Write the shellcode in the allocated buffer, by calling `WriteProcessMemory`
6. Change memory protection to **RX**, by calling `VirtualProtectEx`
7. Create a thread that will execute our shellcode, by calling `CreateRemoteThread` and `WaitForSingleObject` APIs.


## Usage:

1. On your machine, compile the script using `compile.bat` batch file:
![2024-04-06 22_44_37-x64 Native Tools Command Prompt for VS 2022 (2)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/1e385750-2c1b-42f4-a124-86209ffb3dc3)

2. Upload the executable `pov.exe` to the target machine (C:\Temp folder)
3. Start a netcat listener on port `LPORT` from your msfvenom command.
4. Run the executable `pov.exe`

![2024-04-06 21_41_57-kali Linux - VMware Workstation 17 Player (Non-commercial use only)](https://github.com/YounesTasra-R4z3rSw0rd/CTF-Scripts/assets/101610095/185d1e43-7d49-433a-ab2f-cd448c25b5cf)

