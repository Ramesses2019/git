#pragma once
#include "git.h"
_CRT_BEGIN_C_HEADER

typedef enum
{
	NONE = 0,
	HELLO,						 //�ͻ��� ������� ���� ȷ�� ��������
	HEI,						//����������
	//CLIENT_ADDR,
	COMMIT,                    //�ύ��Ϣ
	COMMIT_VERSION_LIST,		//�ύ�汾�б�
	VERSION_LOG,				//�������� �汾�ύ��־
	VERSION_STATUS,				//�汾�޸����� ���ΰ汾״̬
	PROTOCAL_ERROR,				//�ڷ����������Ĵ�����Ϣ
	VERSION_PROTOCAL,			//��ȡ���·������汾��Ϣ
	VERSION_LIST,				//��ȡ���·������汾�б�		
	VERSION_NEXT,				//��ȡ�ڱ��汾�ŵĻ����ϵ���һ���汾��		
	VERSION_NEXT_COMPLETE,		//�������汾�б��ͳɹ�		
	VERSION_CLASH,				//��� ��ͻ		
	CLASH_CONTENT,				//��ȡ ��ͻ ����		
	NO_CLASH,					//�޳�ͻ		
}egit_protocal_type;

_CRT_END_C_HEADER