#include "StdAfx.h"
#include "DumpHelper.h"
#include <imagehlp.h>
#pragma comment(lib, "dbghelp.lib")

//***********************************************************************
//函数名称:    GetAppPath                                                          
//功能描述:    该函数的功能:获取文件目录为当前目录                                            
//前置条件:    无。                                
//后置条件:    无。            
//参数说明:    
//返回说明:    无                                               
//***********************************************************************
CString GetAppPath()

{
	char szPathName[100];//应用程序文件路径
	GetModuleFileNameA(NULL, &szPathName[0], 100);

	CString strPath(szPathName);//文件路径
	int ni = strPath.ReverseFind('\\');
	strPath = strPath.Mid(0, ni + 1);

	return strPath;

}
//***********************************************************************
//函数名称:    CreateDumpFile                                                          
//功能描述:    该函数的功能: 本文件用于自动生成Dump文件，文件目录为当前目录                                            
//前置条件:    无。                                
//后置条件:    无。            
//参数说明:    CString strFileName， 输入， 文件名
//             EXCEPTION_POINTERS *pException, 输入，异常点信息
//返回说明:    无                                               
//***********************************************************************
void CreateDumpFile(CString strFileName, EXCEPTION_POINTERS *pException)
{
	CString strPath = GetAppPath() + _T("\\") + strFileName;

	// 创建Dump文件 
	HANDLE hDumpFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump信息
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

//***********************************************************************
//函数名称:    ApplicationCrashHandler                                                          
//功能描述:    该函数的功能: 用于处理异常退出系统的函数                                            
//前置条件:    无。                                
//后置条件:    无。            
//参数说明:    EXCEPTION_POINTERS *pException， 输入， 异常点信息                  
//返回说明:    类型:LONG    值: EXCEPTION_EXECUTE_HANDLER：返回异常点句柄                                                
//***********************************************************************
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	CreateDumpFile(_T("DumpHelper.dmp"), pException);
	//FatalAppExit(0xff,  _T("系统出现未知故障，请联系系统管理员"));
	return EXCEPTION_EXECUTE_HANDLER;
}


//***********************************************************************
//函数名称:   ProcessException                                                           
//实现约束:   设置回调函数                                                          
//修改履历:   2015-02-12  创建函数                                                           
//***********************************************************************
void CDumpHelper::ProcessException()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)(::ApplicationCrashHandler));
}
