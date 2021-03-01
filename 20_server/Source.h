#pragma once
#include "git.h"

char git_project_path[MAX_PATH] = { 0 };
char git_server_cache_path[MAX_PATH] = { 0 };
char git_path[MAX_PATH] = { 0 };
char git_version_filename[MAX_PATH] = { 0 };
char git_version_list_filename[MAX_PATH] = { 0 };

//�ͻ��˵�ַ�Ľṹ��
typedef struct
{
	int size;
	char addr[1024][MAX_PATH];
}fclient;
fclient clients;

//�ύ�汾����Ϣ
typedef struct
{
	char name[_MAX_PATH];
	char commit[_MAX_PATH];
	char date[_MAX_PATH];
	simple_c_guid guid;
}fgit_commit;

typedef struct 
{
	unsigned int size;
	fgit_commit commits[1024];
}fgit_commits;
fgit_commits commits;

//�汾
typedef struct
{
	unsigned int file_size;
	char file_name[MAX_PATH];
	simple_c_guid crc;
}fgit_version;

//�����ļ��İ汾
typedef struct
{
	unsigned int size;
	fgit_version paths[MAX_PATH * 2];
}fgit_versions;

//�����ļ��汾���б�
typedef struct
{
	unsigned int size;
	fgit_versions paths[1024];
}fgit_versions_list;
fgit_versions_list versions_list;

char *git_commit_to_string(const fgit_commit *git_commit, char *buf);
void string_to_git_commit(fgit_commit *git_commit, const char *buf);
void string_to_git_commits(fgit_commits *git_commit, const char *buf);
void commits_to_string(const fgit_commits *git_commit, char *buf);
void commits_to_versions(const fgit_commits *git_commit, fgit_versions_list *in_versions);

void init_commit(fgit_commit *out_commit);
void init_versions(fgit_versions *in_versions);
int get_index_by_commit(const char *client_crc, fgit_commit **out_commit);

//���汾�б�ת��Ϊstring
void versions_to_string(char *buf, const fgit_versions *in_versions);
//��stringת��Ϊ�汾�б�
void string_to_versions(const char *buf, fgit_versions *in_versions);
void compare_list(const fgit_versions *in_versions_a, const fgit_versions *in_versions_b, fgit_versions *out_versions);

const char *get_project_path();
const char *get_git_path();
const char *get_git_server_cache_path();
//version.list
const char * get_git_version_filename();
//version_list.list
const char * get_git_current_version_filename();

void init_server();

void listening_clients();

void close_server();

unsigned char get_protocal(); //����255��Э��

void get_protocal_content(char *buf);