#include "StdAfx.h"
#include "DumpHelper.h"
#include <imagehlp.h>
#pragma comment(lib, "dbghelp.lib")

//***********************************************************************
//��������:    GetAppPath                                                          
//��������:    �ú����Ĺ���:��ȡ�ļ�Ŀ¼Ϊ��ǰĿ¼                                            
//ǰ������:    �ޡ�                                
//��������:    �ޡ�            
//����˵��:    
//����˵��:    ��                                               
//***********************************************************************
CString GetAppPath()

{
	char szPathName[100];//Ӧ�ó����ļ�·��
	GetModuleFileNameA(NULL, &szPathName[0], 100);

	CString strPath(szPathName);//�ļ�·��
	int ni = strPath.ReverseFind('\\');
	strPath = strPath.Mid(0, ni + 1);

	return strPath;

}
//***********************************************************************
//��������:    CreateDumpFile                                                          
//��������:    �ú����Ĺ���: ���ļ������Զ�����Dump�ļ����ļ�Ŀ¼Ϊ��ǰĿ¼                                            
//ǰ������:    �ޡ�                                
//��������:    �ޡ�            
//����˵��:    CString strFileName�� ���룬 �ļ���
//             EXCEPTION_POINTERS *pException, ���룬�쳣����Ϣ
//����˵��:    ��                                               
//***********************************************************************
void CreateDumpFile(CString strFileName, EXCEPTION_POINTERS *pException)
{
	CString strPath = GetAppPath() + _T("\\") + strFileName;

	// ����Dump�ļ� 
	HANDLE hDumpFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump��Ϣ
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// д��Dump�ļ�����
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

//***********************************************************************
//��������:    ApplicationCrashHandler                                                          
//��������:    �ú����Ĺ���: ���ڴ����쳣�˳�ϵͳ�ĺ���                                            
//ǰ������:    �ޡ�                                
//��������:    �ޡ�            
//����˵��:    EXCEPTION_POINTERS *pException�� ���룬 �쳣����Ϣ                  
//����˵��:    ����:LONG    ֵ: EXCEPTION_EXECUTE_HANDLER�������쳣����                                                
//***********************************************************************
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	CreateDumpFile(_T("DumpHelper.dmp"), pException);
	//FatalAppExit(0xff,  _T("ϵͳ����δ֪���ϣ�����ϵϵͳ����Ա"));
	return EXCEPTION_EXECUTE_HANDLER;
}


//***********************************************************************
//��������:   ProcessException                                                           
//ʵ��Լ��:   ���ûص�����                                                          
//�޸�����:   2015-02-12  ��������                                                           
//***********************************************************************
void CDumpHelper::ProcessException()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)(::ApplicationCrashHandler));
}
