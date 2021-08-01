#include <windows.h>
#include <Tlhelp32.h>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;


typedef struct _CLIENT_ID { HANDLE UniqueProcess; HANDLE UniqueThread; }
CLIENT_ID, *PCLIENT_ID; 

typedef struct _OBJECT_ATTRIBUTES { ULONG Length; HANDLE RootDirectory; PVOID ObjectName; ULONG Attributes; PVOID SecurityDescriptor; PVOID SecurityQualityOfService; } 
OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES; 
//ZwOpenProcess 函数定义 

typedef DWORD (__stdcall *ZWOPENPROCESS)( OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID PCLIENT_ID OPTIONAL );
 //ZwTerminateProcess 函数定义 

 typedef DWORD (__stdcall *ZWTERMINATEPROCESS)( HANDLE hProcess, // handle to the process 
 UINT uExitCode // exit code for the process 
 ); //切忌：以上两个函数要用__stdcall调用方式 //返回0为失败，非零为成功 

 BOOL KillProcess(DWORD dwProcessId)//进程ID 
 { 
 	HMODULE hModule = GetModuleHandle("ntdll.dll"); //获取NTDLL.DLL句柄 
 	if(hModule == NULL) return FALSE; 
 	ZWOPENPROCESS ZwOpenProcess = (ZWOPENPROCESS)GetProcAddress(hModule,"ZwOpenProcess"); //获取ZwOpenProcess地址 
 	if(ZwOpenProcess == NULL) return FALSE; 
 	ZWTERMINATEPROCESS ZwTerminateProcess = (ZWTERMINATEPROCESS)GetProcAddress(hModule,"ZwTerminateProcess"); //获取ZwTerminateProcess地址 
 	if(ZwTerminateProcess == NULL) return FALSE; 
 	HANDLE hProcess = NULL; 
 	CLIENT_ID ClientId; //填入要终止的ID,强制转换为HANDLE类型，虽然UniqueProcess为HANDLE类型，但是接受进程ID 
 	ClientId.UniqueProcess = (HANDLE)dwProcessId; 
 	ClientId.UniqueThread = 0; 
 	OBJECT_ATTRIBUTES oa; 
 	oa.Length = sizeof(oa); 
 	oa.RootDirectory = 0; 
 	oa.ObjectName = 0; 
 	oa.Attributes = 0; 
 	oa.SecurityDescriptor = 0; 
 	oa.SecurityQualityOfService = 0; //打开进程，返回句柄 
 	ZwOpenProcess(&hProcess,PROCESS_TERMINATE,&oa,&ClientId); 
 		if(hProcess == NULL) 
 			return FALSE; //关闭进程 
 	return ZwTerminateProcess(hProcess,0); 
 }
 

class Cfuck{	
public:		
DWORD  code;		
};
vector<Cfuck>fuck;
// 根据进程名称获取进程

DWORD  GetProcessIDByName(const char*name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	
	if (INVALID_HANDLE_VALUE == hSnapshot) 
		{		return NULL;	} 	
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {		
		if (strstr(pe.szExeFile,name ) != 0) {
				Cfuck saved;saved.code=pe.th32ProcessID;
				fuck.push_back(saved);	
				printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
				CloseHandle(hSnapshot);
			
	}		

	} 
	CloseHandle(hSnapshot); 	
	return 0;}

void KillPro(){	
	GetProcessIDByName("jfglzs");
	GetProcessIDByName("pfn");
	GetProcessIDByName("srvany");
	GetProcessIDByName("zmserv");
        GetProcessIDByName("StudentMain")
	for( vector<Cfuck>::iterator it = fuck.begin();it<fuck.end();it++){		
	  //if (!(*it).code < 0){
	   			
		cout<<(*it).code<<"  return code: "<<KillProcess((*it).code)<<endl;}			 
            //}	
	return;
}   
int main(){	
		KillPro(); 
		getchar();
return 0;}  
