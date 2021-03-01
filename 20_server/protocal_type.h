#pragma once
#include "git.h"
_CRT_BEGIN_C_HEADER

typedef enum
{
	NONE = 0,
	HELLO,						 //客户端 向服务器 发送 确认 用于链接
	HEI,						//服务器反馈
	//CLIENT_ADDR,
	COMMIT,                    //提交信息
	COMMIT_VERSION_LIST,		//提交版本列表
	VERSION_LOG,				//请求服务端 版本提交日志
	VERSION_STATUS,				//版本修改内容 本次版本状态
	PROTOCAL_ERROR,				//在服务器发生的错误信息
	VERSION_PROTOCAL,			//获取最新服务器版本信息
	VERSION_LIST,				//获取最新服务器版本列表		
	VERSION_NEXT,				//获取在本版本号的基础上的下一个版本号		
	VERSION_NEXT_COMPLETE,		//服务器版本列表发送成功		
	VERSION_CLASH,				//检测 冲突		
	CLASH_CONTENT,				//获取 冲突 内容		
	NO_CLASH,					//无冲突		
}egit_protocal_type;

_CRT_END_C_HEADER