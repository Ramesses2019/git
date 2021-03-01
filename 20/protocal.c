#include "protocal.h"

void init_network_data_buf_protocal()
{
	memset(network_data_buf, 0, 512);
	recv_protocal = NONE;
}

void git_init_mutex()
{
	InitializeCriticalSection(&mutex);
}

void git_lock()
{
	//�����ٽ���
	EnterCriticalSection(&mutex);
}

void git_unlock()
{
	//�뿪�ٽ���
	LeaveCriticalSection(&mutex);
}

void git_close_mutex()
{

}

bool git_connect(const char *url, const char *addr)
{
	if (git_send_protocal_type(url, HELLO))
	{
		if (git_send_content(url, addr))
		{
			egit_protocal_type t = NONE;

			char buf[512] = { 0 };
			git_recv(&t, buf);

			if (t == HEI)
			{
				return true;
			}
		}
		//Sleep(1500);
		//char tmp[BUF_SIZE] = { 0 };
		//strcpy(tmp, url);
		//strcat(tmp, "\\git_server\\type.protocal");

		//if (_access(tmp, 0) == 0)
		//{
		//	return false;
		//}

	}
	return false;
}

bool git_send(const char *url, const char *local_path)
{
	if (copy_file(local_path, url) != 0)
	{
		return true;
	}
	return false;
}

void git_recv(egit_protocal_type *type, char *buf)
{
	//�������߳�
	SuspendThread(hwork);
	git_lock();

	strcpy(buf, network_data_buf);
	*type = recv_protocal;

	init_network_data_buf_protocal();

	git_unlock();
}

bool git_send_protocal_type(const char *url, egit_protocal_type type)
{
	char local_buf_path[BUF_SIZE] = { 0 };
	_getcwd(local_buf_path, BUF_SIZE);

	strcat(local_buf_path, "\\type.protocal");
	char buf[BUF_SIZE] = { 0 };

	if (create_file(local_buf_path))
	{
		char *content = _itoa(type, buf, 10);
		add_file_buf(local_buf_path, content);

		char url_param[BUF_SIZE] = { 0 };
		strcpy(url_param, url);

		strcat(url_param, REMOTE_T_URL);

		//�ѱ����ļ����뵽�������� Ȼ��ɾ�������ļ�
		if (git_send(url_param, local_buf_path))
		{
			remove(local_buf_path);
			return true;
		}
	}
	return false;
}

bool git_send_content(const char *url, const char *buf)
{
	char local_buf_path[BUF_SIZE] = { 0 };
	_getcwd(local_buf_path, BUF_SIZE);

	strcat(local_buf_path, "\\content.protocal");

	if (create_file(local_buf_path))
	{
		add_file_buf(local_buf_path, buf);

		char url_param[BUF_SIZE] = { 0 };
		strcpy(url_param, url);

		strcat(url_param, REMOTE_C_URL);

		//�ѱ����ļ����뵽�������� Ȼ��ɾ�������ļ�
		if (git_send(url_param, local_buf_path))
		{
			remove(local_buf_path);
			return true;
		}
	}
	return false;
}

void git_is_exist_type(const char *url_param)
{
	bool b_exit = true;
	while (b_exit)
	{
		if (_access(url_param, 0) == 0)
		{
			Sleep(100);
		}
		else
		{
			b_exit = false;
		}
	}
}

void git_is_server_exist_type(const char *url)
{
	char url_param[BUF_SIZE] = { 0 };
	strcpy(url_param, url);

	strcat(url_param, REMOTE_T_URL);

	git_is_exist_type(url_param);
}

void git_is_server_exist_content(const char *url)
{
	char url_param[BUF_SIZE] = { 0 };
	strcpy(url_param, url);

	strcat(url_param, REMOTE_C_URL);

	git_is_exist_type(url_param);
}

void get_protocal_content(char *buf)
{
	char path_buf[MAX_PATH] = { 0 };
	strcpy(path_buf, get_git_client_cache_path());
	strcat(path_buf, "\\content.protocal");
	get_file_buf(path_buf, buf);

	remove(path_buf);
}

