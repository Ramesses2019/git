#pragma once
#include "git.h"
#define BUF_SIZE 260

//对线程进行控制
HANDLE hwork;
HANDLE hrev;
egit_protocal_type recv_protocal;
char network_data_buf[512];
CRITICAL_SECTION mutex; //锁 保证线程之间的数据不会出问题

_CRT_BEGIN_C_HEADER
void init_network_data_buf_protocal();

void git_init_mutex();
void git_lock();
void git_unlock();
void git_close_mutex();

bool git_connect(const char *url);
bool git_send(const char *url, const char *local_path);
//工作线程 进行取数据的操作
void git_recv(egit_protocal_type *type, char *buf);

//检测本地的文件type.protocal并拷入到服务器，然后删除本地的该文件
bool git_send_protocal_type(const char *url, egit_protocal_type type);
bool git_send_content(const char *url, const char *buf);
_CRT_END_C_HEADER