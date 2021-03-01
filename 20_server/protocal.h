#pragma once
#include "git.h"
#define BUF_SIZE 260

//���߳̽��п���
HANDLE hwork;
HANDLE hrev;
egit_protocal_type recv_protocal;
char network_data_buf[512];
CRITICAL_SECTION mutex; //�� ��֤�߳�֮������ݲ��������

_CRT_BEGIN_C_HEADER
void init_network_data_buf_protocal();

void git_init_mutex();
void git_lock();
void git_unlock();
void git_close_mutex();

bool git_connect(const char *url);
bool git_send(const char *url, const char *local_path);
//�����߳� ����ȡ���ݵĲ���
void git_recv(egit_protocal_type *type, char *buf);

//��Ȿ�ص��ļ�type.protocal�����뵽��������Ȼ��ɾ�����صĸ��ļ�
bool git_send_protocal_type(const char *url, egit_protocal_type type);
bool git_send_content(const char *url, const char *buf);
_CRT_END_C_HEADER