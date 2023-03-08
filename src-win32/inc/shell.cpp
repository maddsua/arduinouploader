//  2022 maddsua | https://github.com/maddsua
//	No warranties are given, etc...
//	This file is a component of the AVR Firmware Uploader

#include "shell.hpp"

void launcher(const char* use_mcu, const char* use_prog, const char* use_speed, const char* use_port, const char* filepath, bool* running, DWORD* exitcode){
	
	char loaderCommand[dudecmdlen] = {0};
		sprintf(loaderCommand, "avrdude.exe -v -p%s -c%s -P %s -b%s -D -U flash:w:\"%s\":a", use_mcu, use_prog, use_port, use_speed, filepath);
								
	STARTUPINFO stinf = {0};
	stinf.cb = sizeof(stinf);
	PROCESS_INFORMATION prcinf = {0};
	
	if(CreateProcess(NULL, loaderCommand, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &stinf, &prcinf)){

      if(WaitForSingleObject(prcinf.hProcess, dude_run_timeout) == WAIT_TIMEOUT){
      	
      	TerminateProcess(prcinf.hProcess, EC_DUDE_TIMEOUT);
	  }

      GetExitCodeProcess(prcinf.hProcess, exitcode);

      CloseHandle(prcinf.hThread);
      CloseHandle(prcinf.hProcess);
	}
	else{
		*exitcode = EC_DUDE_NOEXEC;
	}
								
	*running = false;
	return;
}

void killProcessByName(const char *filename){
	
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 pEntry;
	
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
	
    while(hRes){
        if(strcmp(pEntry.szExeFile, filename) == 0){
			
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD) pEntry.th32ProcessID);
            if (hProcess != NULL){
				
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
	
    CloseHandle(hSnapShot);
}
