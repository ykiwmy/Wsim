//*******************************************************************************
//文件名称 :     DumpHelper.h                                                               
//版权信息 :     株洲南车时代电气股份有限公司版权所有                            
//创建日期 :     2015-02-12                                                            
//文件描述 :     该头文件用于提供一个接口专门获取系统异常退出点，自动生成DumpHelper.dmp文件，用于定位异常点出现的位置 
//               ProcessException()需要在主程序App的InitInstance函数以及各子线程循环函数入口处中调用即可；
//               注意事项：
//               要求主程序以及主程序附加的所有DLL库，在Release版本中都应能生成.pdb文件，注意pdb文件和.exe文件应保持一致
//修改履历 :      
// V0.0.0      2015-02-12    创建本文件 
//*******************************************************************************
#pragma once

class CDumpHelper
{
public:
	//***********************************************************************
	//函数名称:    ProcessException                                                       
	//功能描述:    该函数的功能:设置回调函数，专门用于处理系统异常退出的情况；                                             
	//前置条件:    无。                                
	//后置条件:    无。            
	//参数说明:    无。            
	//返回说明:    无。                                           
	//***********************************************************************
	static void ProcessException();
};

