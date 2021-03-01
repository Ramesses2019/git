#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include "git.h"

char git_path[MAX_PATH] = { 0 };
char git_version_filename[MAX_PATH] = { 0 };
char git_current_version_filename[MAX_PATH] = { 0 };
char git_client_cache_path[MAX_PATH] = { 0 }; //本地缓存池
char git_project_remote_url[MAX_PATH] = { 0 }; //远端仓库
char git_project_path[_MAX_PATH] = { 0 };     //记录版本信息(git)的路径
//char git_log_path[_MAX_PATH] = { 0 }; //日志路径
char git_remote_origin[_MAX_PATH] = { 0 }; //远端路径 url
const char git_local_cofg_file[_MAX_PATH] = "C:\\local_git\\"; //写定一个存放文件配置的路径（用户配置信息）
char git_local_cofg_filename[_MAX_PATH] = { 0 }; //用户配置信息
int b_exit = false;

char remove_file_from_server[8196 * 1024] = { 0 };
///////////////////////////////////////////////////////////
//用户的信息
typedef struct fgit_user
{
	char name[_MAX_PATH];
	char password[_MAX_PATH];
	char email[_MAX_PATH];
};

struct fgit_user user;

//提交版本的信息
typedef struct 
{
	bool b_init;
	char name[_MAX_PATH];
	char commit[_MAX_PATH];
	char date[_MAX_PATH];
	simple_c_guid guid;
}fgit_commit;

fgit_commit commit;

typedef struct
{
	unsigned int size;
	fgit_commit commits[1024];
}fgit_commits;

//git 的路径
typedef struct 
{
	char path_src[MAX_PATH];
	char path_dis[MAX_PATH];
}fgit_path_2d;

//git 所有提交的路径
typedef struct
{
	int size;
	fgit_path_2d paths[MAX_PATH * 2];
}fgit_path_2ds;
fgit_path_2ds git_path_2ds;

//版本
typedef struct 
{
	unsigned int file_size;
	char file_name[MAX_PATH];
	simple_c_guid crc;
}fgit_version;

//整体文件的版本
typedef struct  
{
	unsigned int size;
	fgit_version paths[1024];
}fgit_versions;
fgit_versions versions;

//整体文件版本的列表
typedef struct
{
	unsigned int size;
	fgit_versions paths[1024];
}fgit_versions_list;
fgit_versions_list versions_list;

//引擎
///////////////////////////////////////////////////////////
char *git_commit_to_string(const fgit_commit *git_commit, char *buf);
void string_to_git_commit(fgit_commit *git_commit, const char *buf);
void string_to_git_commits(fgit_commits *git_commit, const char *buf);

//core
void init_engine();
void engine_loop();
void exit_engine();

//
void read_user();
void save_user();

//get
const char *get_git_path();
//version.list
const char * get_git_version_filename();
//version_list.list
const char * get_git_current_version_filename();
char *get_git_client_cache_path();
char *get_git_project_remote_url();
char *get_current_time();
char *get_git_project_path();
char *get_git_local_user_filename();
char *get_git_commit(fgit_commit *git_commit);
unsigned char get_protocal();

//commit
void init_versions(fgit_versions *in_versions);
void init_commit(fgit_commit *out_commit);
void init_git_commits(fgit_commits *git_commits);
void init_git_path_2ds(fgit_path_2ds *path_2ds);

//添加版本
void add_git_versions(const char *in_path, fgit_versions *out_versions);
//将版本列表转化为string
void versions_to_string(char *buf, const fgit_versions *in_versions);
//将string转化为版本列表
void string_to_versions(const char *buf, fgit_versions *in_versions);
void compare_list(const fgit_versions *server_version_list, const def_paths *local_paths, def_paths *out_paths);

