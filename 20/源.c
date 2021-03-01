#include "source.h"

const char * get_git_path()
{
	if (git_path[0] == '\0')
	{
		const char *p = get_git_project_path();
		strcpy(git_path, p);
		strcat(git_path, "\\.git\\");
	}
	return git_path;
}

const char * get_git_version_filename()
{
	if (git_version_filename[0] == '\0')
	{
		const char *p = get_git_path();
		strcpy(git_version_filename, p);

		strcat(git_version_filename, "version.list");
	}

	return git_version_filename;
}

const char * get_git_current_version_filename()
{
	if(git_current_version_filename[0] == '\0')
	{
		const char *p = get_git_path();
		strcpy(git_current_version_filename, p);

		strcat(git_current_version_filename, "version.info");
	}

	return git_current_version_filename;
}

char * get_git_client_cache_path()
{
	if (git_client_cache_path[0] == '\0')
	{
		_getcwd(git_client_cache_path, MAX_PATH);
		strcat(git_client_cache_path, "\\git_client");
	}

	return git_client_cache_path;
}

char *get_current_time()
{
	return ctime(__TIME__);
}



//单例模式 获取版本
char *get_git_project_path()
{
	if (git_project_path[0] == '\0')
	{
		char buf_project_path[MAX_PATH] = { 0 };
		_getcwd(buf_project_path, MAX_PATH);

		simple_c_string c_string;
		dismantling_string(buf_project_path,"\\", &c_string);

		char buf_path[MAX_PATH] = { 0 };
		for (int i = 0; i < c_string.size; i++)
		{
			char *p = get_string(i, &c_string); 

			strcat(buf_path, p);
			strcat(buf_path, "\\");

			char tmp_buf[MAX_PATH] = { 0 };
			strcpy(tmp_buf, buf_path);
			strcat(tmp_buf, ".git\\git.txt");

			if (_access(tmp_buf, 0) == 0)
			{
				strcpy(git_project_path, buf_path);
				break;
			}
		}

		destroy_string(&c_string);

		if (git_project_path[0] == '\0')
		{
			printf("请先执行git init命令 初始化一个本地仓库\r\n");
		}
	}

	return git_project_path;
}

char *get_git_project_remote_url()
{
	if (git_project_remote_url[0] == '\0')
	{
		strcpy(git_project_remote_url, git_remote_origin);
		strcat(git_project_remote_url, "git_server");
	}

	return git_project_remote_url;
}

char * get_git_local_user_filename()
{
	if (git_local_cofg_filename[0] == '\0')
	{
		_mkdir(git_local_cofg_file);
		strcpy(git_local_cofg_filename, git_local_cofg_file);

		strcat(git_local_cofg_filename, "git_user.ini");
		if (_access(git_local_cofg_filename, 0) == -1)
		{
			create_file(git_local_cofg_filename);
		}

	}
	return git_local_cofg_filename;
}

char *get_git_commit(fgit_commit *git_commit)
{
	init_commit(git_commit);

	char buf_char[1024] = { 0 };
	get_file_buf(get_git_version_filename(), buf_char);

	string_to_git_commit(git_commit,buf_char);
}

char *git_commit_to_string(const fgit_commit *git_commit, char *buf)
{
	char buf_guid[MAX_PATH] = { 0 };
	guid_to_string(buf_guid, &git_commit->guid);
	get_printf_s(buf ,"%s\n%s\n%s\n%s\n", git_commit->name, git_commit->commit, git_commit->date, buf_guid);

	return buf;
}

void string_to_git_commit(fgit_commit *git_commit, const char *buf)
{
	simple_c_string c_string;
	dismantling_string(buf, "\n" , &c_string);
	{
		char *value = get_string(0, &c_string);
		strcpy(git_commit->name, value);
	}
	{
		char *value = get_string(1, &c_string);
		strcpy(git_commit->commit, value);
	}
	{
		char *value = get_string(2, &c_string);
		strcpy(git_commit->date, value);
	}
	{
		char *value = get_string(3, &c_string);
		string_to_guid(value, &git_commit->guid);
	}
	destroy_string(&c_string);
}

void string_to_git_commits(fgit_commits *git_commit, const char *buf)
{
	simple_c_string c_string;
	dismantling_string(buf, "\n", &c_string);

	for (int i = 0; i < c_string.size; i += 4)
	{
		fgit_commit *git_commit_elememt = &git_commit->commits[git_commit->size++];
		{
			char *value = get_string(i, &c_string);
			strcpy(git_commit_elememt->name, value);
		}
		{
			char *value = get_string(i + 1, &c_string);
			strcpy(git_commit_elememt->commit, value);
		}
		{
			char *value = get_string(i + 2, &c_string);
			strcpy(git_commit_elememt->date, value);
		}
		{
			char *value = get_string(i + 3, &c_string);
			string_to_guid(value, &git_commit_elememt->guid);
		}
	}
	destroy_string(&c_string);
}

void init_versions(fgit_versions *in_versions)
{
	in_versions->size = 0;
	memset(in_versions->paths, 0, sizeof(in_versions->paths));
}

void init_commit(fgit_commit *out_commit)
{
	out_commit->b_init = true;
	memset(out_commit->name, 0, sizeof(out_commit->name));
	memset(out_commit->commit, 0, sizeof(out_commit->commit));
	memset(out_commit->date, 0, sizeof(out_commit->date));
	nomalization_guid(&out_commit->guid);
}

void init_git_commits(fgit_commits *git_commits)
{
	git_commits->size = 0;
	memset(git_commits->commits, 0, sizeof(git_commits->commits));
}

void init_git_path_2ds(fgit_path_2ds *path_2ds)
{
	path_2ds->size = 0;
	memset(path_2ds->paths, 0, strlen(path_2ds->paths));
}

void add_git_versions(const char *in_path, fgit_versions *out_versions)
{
	//获取相对路径
	char new_path[MAX_PATH] = { 0 };
	strcpy(new_path, in_path);

	const char *p = get_git_project_path();
	remove_string_start(new_path, p);

	strcpy(out_versions->paths[out_versions->size].file_name, new_path);
	create_guid(&out_versions->paths[out_versions->size].crc);
	out_versions->paths[out_versions->size].file_size = get_file_size_by_filename(in_path);

	out_versions->size++;
}

void versions_to_string(char *buf, const fgit_versions *in_versions)
{
	for (int i = 0; i < in_versions->size; i++)
	{
		char tmp_buf[MAX_PATH * 2] = { 0 };
		char guid_buf[MAX_PATH] = { 0 };
		guid_to_string(guid_buf, &in_versions->paths[i].crc);
		get_printf_s(tmp_buf, "%s\n%s\n%d\n", in_versions->paths[i].file_name, guid_buf, in_versions->paths[i].file_size);

		strcat(buf, tmp_buf);
	}
}

void string_to_versions(const char *buf, fgit_versions *in_versions)
{
	simple_c_string c_string;
	dismantling_string(buf, "\n", &c_string);

	for (int i = 0; i < c_string.size; i += 3)
	{
		char *p = get_string(i, &c_string);
		strcpy(in_versions->paths[in_versions->size].file_name, p);

		p = get_string(i + 1, &c_string);
		string_to_guid(p, &in_versions->paths[in_versions->size].crc);
		
		p = get_string(i + 2, &c_string);
		in_versions->paths[in_versions->size++].file_size = atoi(p);
	}
	destroy_string(&c_string);
}

unsigned char get_protocal()
{
	char path_tmp[MAX_PATH] = { 0 };
	unsigned char ret = 0;

	const char *p = get_git_client_cache_path();
	strcpy(path_tmp, p);
	strcat(path_tmp, "\\type.protocal");
	if (_access(path_tmp, 0) == 0)
	{
		char buf[MAX_PATH] = { 0 };
		get_file_buf(path_tmp, buf);
		remove(path_tmp);
		ret = atoi(buf);
	}
	return ret;
}

//比较不一样的
//比较服务器没有但客户端有 就添加到服务器
//同时计算客户端没有的 服务器有的 通知服务器删除此文件
void compare_list(const fgit_versions *server_version_list, const def_paths *local_paths, def_paths *out_paths)
{
	const char *client_url = get_git_project_path();

	//添加（不一样的）
	for (int i = 0; i < local_paths->index; i++)
	{
		char path_tmp[MAX_PATH] = { 0 };
		strcpy(path_tmp, local_paths->paths[i]);
		remove_string_start(path_tmp, client_url);

		int size = get_file_size_by_filename(local_paths->paths[i]);

		bool b_exit = false;
		for (int j = 0; j < server_version_list->size; j++)
		{
			if (strcmp(server_version_list->paths[j].file_name, path_tmp) == 0) //比较相对路径
			{
				if (size != server_version_list->paths[j].file_size)
				{
					strcpy(out_paths->paths[out_paths->index++], local_paths->paths[i]); //绝对路径
				}
				b_exit = true;
				break;
			}		
		}

		if (!b_exit)
		{
			strcpy(out_paths->paths[out_paths->index++], local_paths->paths[i]); //绝对路径
		}
	}
}

DWORD WINAPI git_client_receive(LPVOID lpParam)
{
	bool b_quit = false;
	while (!b_quit)
	{
		git_lock();

		unsigned char protocal = get_protocal();
		switch ((egit_protocal_type)protocal)
		{
		case NONE:
			break;
			//客户端不接受HELLO
		//case HELLO://客户端进行发送
		//{
		//	printf("server 收到的协议号是 %i\r\n", protocal);
		//	break;
		//}		
		case HEI: //服务端进行返回确认
		{
			recv_protocal = protocal;
			ResumeThread(hwork);
			break;
		}	
		case VERSION_LOG:
		case VERSION_STATUS:
		case VERSION_PROTOCAL:
		case VERSION_LIST:
		case VERSION_NEXT:
		case VERSION_NEXT_COMPLETE:
		case CLASH_CONTENT:
		case NO_CLASH:
		{ 
			Sleep(100);
			init_network_data_buf_protocal();
			get_protocal_content(network_data_buf);

			recv_protocal = (egit_protocal_type)protocal;
			ResumeThread(hwork);
			break;
		}
		case PROTOCAL_ERROR:
		{
			log_error("服务 错误~");
			ResumeThread(hwork);
			break;
		}
		default:
			break;
		}

		Sleep(500);

		git_unlock();
	}

	return 0;
}

DWORD WINAPI git_client_send(LPVOID lpParam)
{
	//循环
	engine_loop();

	return 0;
}

void init_engine()
{
	//初始化日志的项目路径
	init_log_system(get_git_project_path());

	char *p = get_git_client_cache_path();
	_mkdir(p);

	//初始化锁
	git_init_mutex();

	init_network_data_buf_protocal();

	commit.b_init = false;

	//读取用户配置表
	read_user();

	//创建线程
	//参数2 堆栈大小（随系统设置中提供的值设定）
	//参数5 0代表线程启动时就会自己执行
	hwork = CreateThread(NULL, 100000000, git_client_send, NULL, 0, NULL);
	hrev = CreateThread(NULL, 100000000, git_client_receive, NULL, 0, NULL);
}

void read_user()
{
	//C:\\local_git\\git_cofig.ini
	char *cofg_path_buf = get_git_local_user_filename();

	char buf_cofg[1024 * 10] = { 0 };

	if (get_file_buf(cofg_path_buf, buf_cofg))
	{
		simple_c_string c_str_sentences;
		//通过换行符对每一行的内容进行切割
		dismantling_string(buf_cofg, "\n", &c_str_sentences);

		for (int i = 0; i < c_str_sentences.size; i++)
		{
			char *tmp = get_string(i, &c_str_sentences);
			//如果是账户
			if (strstr(tmp, "account="))
			{
				dismantling_string_by_index(1, tmp, "=", user.name);
			}
			//如果是密码
			else if (strstr(tmp, "password="))
			{
				dismantling_string_by_index(1, tmp, "=", user.password);
			}
			//如果是邮箱
			else if (strstr(tmp, "email="))
			{
				dismantling_string_by_index(1, tmp, "=", user.email);
			}
			//如果是远端数据
			else if (strstr(tmp, "url="))
			{
				dismantling_string_by_index(1, tmp, "=", git_remote_origin);
			}
		}
		destroy_string(&c_str_sentences);
	}	
}

void save_user()
{
	char *cofg_path_buf = get_git_local_user_filename();
	FILE *f = NULL;
	if ((f = fopen(cofg_path_buf, "w+")) != NULL)
	{
		fprintf(f, "account=%s\n", user.name);
		fprintf(f, "password=%s\n",user.password);
		fprintf(f, "email=%s\n",user.email);
		fprintf(f, "url=%s\n",git_remote_origin);

		fclose(f);
	}
}

void version_iteration_update()
{
	//阻塞
	git_is_server_exist_type(git_remote_origin);
	git_is_server_exist_content(git_remote_origin);
	//1 检测当前版本是否为最新 获取服务器最新版本号 然后比较看是否相同
	if (git_send_protocal_type(git_remote_origin, VERSION_PROTOCAL)&&
		git_send_content(git_remote_origin,get_git_project_path()))
	{
		log_success("获取服务器 最新版本号\r\n");

		egit_protocal_type type = NONE;
		char buf_log[8196] = { 0 };
		//阻塞工作线程
		git_recv(&type, buf_log);

		if (buf_log[0] == '\0')
		{
			log_error("buf log 为空");
			return;
		}

		if (type == VERSION_PROTOCAL)
		{
			fgit_commit server_latest_version;
			init_commit(&server_latest_version);
			string_to_git_commit(&server_latest_version, buf_log);

			fgit_commit client_latest_version;
			init_commit(&client_latest_version);
			get_git_commit(&client_latest_version);

			//比较版本是否一样
			if (!guid_equal(&server_latest_version.guid, &client_latest_version.guid))
			{
				const char *client_url = get_git_project_path();

				char str_content[8196] = { 0 };
				guid_to_string(str_content, &client_latest_version.guid);
				strcat(str_content, "|");
				strcat(str_content, client_url);
				strcat(str_content, "|");

				//阻塞
				git_is_server_exist_type(git_remote_origin);
				git_is_server_exist_content(git_remote_origin);

				char path_tmp[MAX_PATH] = { 0 };
				strcpy(path_tmp, client_url);
				remove_char_end(path_tmp, '\\');

				def_paths paths;
				init_def_paths(&paths);
				find_files(path_tmp, &paths, true);

				for (int i = 0; i < paths.index; i++)
				{
					int size = get_file_size_by_filename(paths.paths[i]);
					char size_buf[MAX_PATH] = { 0 };
					char *size_p = _itoa(size, size_buf, 10);

					//按照version的格式拼接
					remove_string_start(paths.paths[i], client_url);
					strcat(str_content, paths.paths[i]);//filename
					strcat(str_content, "\n");
					strcat(str_content, "#"); //crc
					strcat(str_content, "\n");
					strcat(str_content, size_p);
					strcat(str_content, "\n");
				}
				remove_char_end(str_content, '\n');

				if (git_send_protocal_type(git_remote_origin, VERSION_CLASH) &&
					git_send_content(git_remote_origin, str_content))
				{
					type = NONE;
					memset(buf_log, 0, sizeof(buf_log));
					//阻塞工作线程
					git_recv(&type, buf_log);
					if (type == CLASH_CONTENT)
					{
						log_error("%s", buf_log);
						log_error("请解决以上冲突 再进行更新数据...");
					}
					else if (type == NO_CLASH)
					{
						//2 把当前的客户端版本发送到服务端
						//char str_content[1024] = { 0 };
						memset(str_content, 0, sizeof(str_content));
						guid_to_string(str_content, &client_latest_version.guid);

						strcat(str_content, "\n");
						strcat(str_content, get_git_project_path());
						//阻塞
						git_is_server_exist_type(git_remote_origin);
						git_is_server_exist_content(git_remote_origin);

						if (git_send_protocal_type(git_remote_origin, VERSION_NEXT) &&
							git_send_content(git_remote_origin, str_content))
						{
							log_success("获取下一个需要更新的版本\r\n");

							//3 接收新版本号 储存在本地 替换
							type = NONE;
							memset(buf_log, 0, sizeof(buf_log));
							//阻塞工作线程
							git_recv(&type, buf_log);

							if (buf_log[0] == '\0')
							{
								log_error("buf log 为空");
								return;
							}

							if (type == VERSION_NEXT)
							{
								add_new_file_buf(get_git_version_filename(), buf_log);
							}

							type = NONE;
							memset(buf_log, 0, sizeof(buf_log));
							//阻塞工作线程
							git_recv(&type, buf_log);

							if (buf_log[0] == '\0')
							{
								log_error("buf log 为空");
								return;
							}

							if (type == VERSION_NEXT_COMPLETE)
							{
								add_new_file_buf(get_git_current_version_filename(), buf_log);

								//4 检测当前版本是否是最新
								//先不考虑冲突
								//递归 一直检查直到当前的版本为最新
								version_iteration_update();
							}
						}
					}
					else
					{
						log_log("已经是最新了...");
					}
				}
			}		
		}
	}
}

void git_pull_local_data(const fgit_versions *server_version_list,const char *value,const char *head_value)
{
	def_paths paths;
	init_def_paths(&paths);
	find_files(value, &paths, true);

	//只提交发生改变的文件
	def_paths out_paths;
	init_def_paths(&out_paths);
	compare_list(server_version_list, &paths, &out_paths);

	for (int i = 0; i < out_paths.index; i++)
	{
		char buf_tmp[MAX_PATH] = { 0 };
		strcpy(buf_tmp, out_paths.paths[i]);
		remove_string_start(buf_tmp, value);

		char buf_remote_path_tmp[MAX_PATH] = { 0 };
		strcpy(buf_remote_path_tmp, git_remote_origin);
		if (head_value != NULL)
		{
			strcat(buf_remote_path_tmp, "\\");
			strcat(buf_remote_path_tmp, head_value);
		}
		strcat(buf_remote_path_tmp, buf_tmp);

		//去掉文件 只保留路径名
		{
			char buf_remote_path_tmp1[MAX_PATH] = { 0 };
			strcpy(buf_remote_path_tmp1, buf_remote_path_tmp);
			//去掉末尾的最终文件（为了创建文件夹）
			get_path_directory_inline(buf_remote_path_tmp1);

			//创建所有的文件夹（除最终文件外）
			if (!create_file_directory(buf_remote_path_tmp1))
			{
				log_error("服务器 产生了一个冲突 %s\r\n", buf_remote_path_tmp1);
			}
		}

		git_path_2ds.size = out_paths.index + 1;
		strcpy(git_path_2ds.paths[i].path_src, out_paths.paths[i]);
		strcpy(git_path_2ds.paths[i].path_dis, buf_remote_path_tmp);
		/*	if (copy_file(paths.paths[i],buf_remote_path_tmp))
			{
				log_log("将路径下的 %s 拉取到 %s 成功\r\n", paths.paths[i], buf_remote_path_tmp);
			}
			else
			{
				log_error("将路径下的 %s 拉取到 %s 失败\r\n", paths.paths[i], git_remote_origin);
			}*/

		int b_value = ((double)i / (double)out_paths.index) * 100 + 1;
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("当前正在下载远端数据：%3d%%", b_value);
	}
	printf("当前正在下载远端数据：%3d%%", 100);
}

void engine_loop()
{
	//git_send_protocal_type(git_remote_origin, GIT_PULL);

	char input_buf[1024] = { 0 };
	//循环
	//通过输入命令让程序退出
	while (b_exit == 0)
	{
		printf("\r\n");
		set_consle_w_color(SIMPLE_PALE_GREEN, 0);
		printf("%s ssh:",user.name);
		set_consle_w_color(SIMPLE_LAVENDER, 0);
		printf(" Windows 64 ");
		set_consle_w_color(SIMPLE_CANARY_YELLOW, 0);
		char buf_color[_MAX_PATH];
		_getcwd(buf_color, _MAX_PATH);
		printf(" %s \r\n", buf_color);
		set_consle_w_color(SIMPLE_WHITE, 0);
		printf("\r\n");
		printf(" $ ");
		fgets(input_buf, 1024, stdin);

		//如果包含exit字符串就退出
		if (strstr(input_buf, "exit"))
		{
			b_exit = 1;
		}
		//如果包含git init字符串就进行引擎初始化的操作
		else if (strstr(input_buf, "git init"))
		{
			if (git_project_path[0] == '\0')
			{
				char tmp_path[256] = { 0 };
				strcpy(tmp_path, ".\\.git\\");
				_mkdir(tmp_path);
				strcat(tmp_path, "git.txt"); //记录版本信息

				if (create_file(tmp_path))
				{
					log_success("本地初始化成功");
				}
				else
				{
					log_error("创建项目路径 .git 失败");
				}
			}
			/*char *p = get_git_path();
			log_success("初始化 [%s] 路径成功",p);*/
		}
		//查看版本信息等
		else if (strstr(input_buf, "git log"))
		{
			git_is_server_exist_type(git_remote_origin);
			git_is_server_exist_content(git_remote_origin);

			if (git_send_protocal_type(git_remote_origin, VERSION_LOG))
			{
				const char *client_addr = get_git_project_path();
				if (git_send_content(git_remote_origin, client_addr))
				{
					log_success("客户端地址 发送成功");

					egit_protocal_type type = NONE;
					char buf_log[8196] = { 0 };
					git_recv(&type, buf_log);
					if (type == VERSION_LOG)
					{
						if (buf_log[0] != '\0')
						{
							fgit_commits git_commits;
							init_git_commits(&git_commits);

							string_to_git_commits(&git_commits, buf_log);

							for (int i = 0; i < git_commits.size; i++)
							{
								char guid_buf[MAX_PATH] = { 0 };
								guid_to_string(guid_buf, &git_commits.commits[i].guid);
								
								set_consle_w_color(SIMPLE_YELLOW, 0);
								printf("\n\n版   本  码：%s \r\n", guid_buf);
								set_consle_w_color(SIMPLE_WHITE, 0);
								printf("\n提 交 人 员：%s \r\n", git_commits.commits[i].name);
								printf("\n提 交 信 息：%s \r\n", git_commits.commits[i].commit);
								printf("\n日       期：%s \r\n", git_commits.commits[i].date);
							}
						}
						else
						{
							log_error("接收服务端 发送的bug没有任何有价值的信息");
						}
					}
					else
					{
						log_error("客户端接收失败");
					}

				}
			}
		}
		//查看文件更改状态
		else if (strstr(input_buf, "git status"))
		{
			//阻塞
			git_is_server_exist_type(git_remote_origin);
			git_is_server_exist_content(git_remote_origin);

			if (git_send_protocal_type(git_remote_origin, VERSION_STATUS))
			{
				//..\\本地路径
				//guid
				fgit_commit git_commit;
				get_git_commit(&git_commit);

				char buf_guid[1024] = { 0 };
				guid_to_string(buf_guid, &git_commit.guid);

				const char *client_addr = get_git_project_path();
				char buf_content[1024] = { 0 };
				strcpy(buf_content, client_addr);
				remove_char_end(buf_content, '\\');

				strcat(buf_content, "\n");
				strcat(buf_content, buf_guid);

				if (git_send_content(git_remote_origin, buf_content))
				{
					log_success("地址 和版本 guid 发送成功");

					egit_protocal_type type = NONE;
					char buf_log[8196] = { 0 };
					//阻塞工作线程
					git_recv(&type, buf_log);
					if (type == VERSION_STATUS)
					{
						if (buf_log[0] != '\0')
						{
							fgit_versions git_versions;
							init_versions(&git_versions);

							string_to_versions(buf_log, &git_versions);

							set_consle_w_color(SIMPLE_RED, 0);
							for (int i = 0; i < git_versions.size; i++)
							{
								printf("\n修改的内容为%s\r\n", git_versions.paths[i].file_name);
							}
							set_consle_w_color(SIMPLE_WHITE, 0);
						}
					}
				}
			}
		}	
		//远端操作
		else if (strstr(input_buf, "git remote add origin"))
		{
			dismantling_string_by_index(4, input_buf, " ", git_remote_origin);
			log_success("设置git remote add origin成功 %s", git_remote_origin);

			save_user();
		}
		//邮件操作
		else if (strstr(input_buf, "git --global user.email"))
		{
			dismantling_string_by_index(3, input_buf, " ", user.email);
			log_success("设置 email成功 %s", user.email); //写入日志

			save_user();
		}
		//设置名字的操作
		else if (strstr(input_buf, "git --global user.name"))
		{
			dismantling_string_by_index(3, input_buf, " ", user.name);
			log_success("设置 name成功 %s", user.name); //写入日志

			save_user();
		}
		//设置密码的操作
		else if (strstr(input_buf, "git --global user.password"))
		{
			dismantling_string_by_index(3, input_buf, " ", user.password);
			log_success("设置 password成功 %s", user.password); //写入日志

			save_user();
		}
		//加密
		else if (strstr(input_buf, "ssh-keygen -t rsa -C"))
		{
			log_error("ssh-keygen -t rsa -C 该指令还未完成");
		}
		//克隆功能
		else if (strstr(input_buf, "git clone"))
		{
			simple_c_string c_string;
			dismantling_string(input_buf, " ", &c_string);
			char *value = get_string(2, &c_string);
			remove_char_end(value, '\n');

			//找到value下的所有文件
			def_paths paths;
			init_def_paths(&paths);
			find_files(value, &paths, true);

			//判断是哪种分隔符 把其分开
			simple_c_string c_file;
			if (strstr(value, "\\"))
			{
				dismantling_string(value, "\\", &c_file);
			}
			else if (strstr(value, "/"))
			{
				dismantling_string(value, "/", &c_file);
			}
			else
			{
				log_error("无效 非标准路径 %s", value);
				break;
			}

			//获取最后的文件名
			char *file_value = get_string(c_file.size - 1, &c_file);
			remove_char_end(file_value, '\n');

			//获取本地工作路径名
			char buf_path[MAX_PATH];
			_getcwd(buf_path, MAX_PATH);

			//把本地工作路径和远端提供的最后文件相加 让本地也创建一个该文件
			char buf_local_path[MAX_PATH] = { 0 };
			get_printf(buf_local_path,"%s\\%s", buf_path, file_value);

			char *buf_success[MAX_PATH] = { 0 };
			char *buf_error[MAX_PATH] = { 0 };
			for (int i = 0; i < paths.index; i++)
			{
				char buf_tmp[MAX_PATH] = { 0 };
				strcpy(buf_tmp, paths.paths[i]);
				//把所有远端地址的前部分删除 剩下所有子文件
				remove_string_start(buf_tmp, value);

				//粘贴所有子文件到添加好的本地文件上
				//创建一个新的临时存放本地地址得字符串 防止在本地字符串上操作被覆盖
				char buf_local_path_tmp[MAX_PATH] = { 0 };
				strcpy(buf_local_path_tmp, buf_local_path);
				strcat(buf_local_path_tmp, buf_tmp);

				//去掉文件 只保留路径名
				{
					char buf_local_path_tmp1[MAX_PATH] = { 0 };
					strcpy(buf_local_path_tmp1, buf_local_path_tmp);
					//去掉末尾的最终文件（为了创建文件夹）
					get_path_directory_inline(buf_local_path_tmp1);

					//创建所有的文件夹（除最终文件外）
					if (!create_file_directory(buf_local_path_tmp1))
					{
						log_error("产生了一个冲突 %s\r\n", buf_local_path_tmp1);
					}
				}

				//把远端提供的所有文件（最终）拷贝到本地创建的文件上
				if (copy_file(paths.paths[i], buf_local_path_tmp))
				{
					buf_success[i] = buf_local_path_tmp;
					//log_log("将路径下的 %s 拉取到 %s 成功\r\n", paths.paths[i], buf_local_path_tmp);
				}
				else
				{
					buf_error[i] = buf_local_path;
					//log_error("将路径下的 %s 拉取到 %s 失败\r\n", paths.paths[i], buf_local_path);
				}

				int b_value = ((double)i / (double)paths.index) * 100 + 1;
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				printf("当前正在下载远端数据：%3d%%", b_value);
			}

			//加载完成后打印日志
			for (int j = 0; j < paths.index; j++)
			{
				if (buf_success[j] != '\0')
				{
					log_log("将路径下的 %s 拉取到 %s 成功\r\n", paths.paths[j], buf_success[j]);
				}
				else if (buf_error[j] != '\0')
				{
					log_error("将路径下的 %s 拉取到 %s 失败\r\n", paths.paths[j], buf_error[j]);
				}
			}

			destroy_string(&c_file);
			destroy_string(&c_string);
		}
		//文件的上传
		else if (strstr(input_buf,"git add"))
		{
			git_is_server_exist_type(git_remote_origin);
			git_is_server_exist_content(git_remote_origin);
			//1 检测当前版本是否为最新 获取服务器最新版本号 然后比较看是否相同
			if (git_send_protocal_type(git_remote_origin, VERSION_PROTOCAL) &&
				git_send_content(git_remote_origin, get_git_project_path()))
			{
				log_success("获取服务器 最新版本号\r\n");

				egit_protocal_type type = NONE;
				char buf_log[8196] = { 0 };
				//阻塞工作线程
				git_recv(&type, buf_log);

				if (buf_log[0] == '\0')
				{
					log_error("buf log 为空");
					continue;
				}

				if (type == VERSION_PROTOCAL)
				{
					fgit_commit server_latest_version;
					init_commit(&server_latest_version);
					string_to_git_commit(&server_latest_version, buf_log);

					fgit_commit client_latest_version;
					init_commit(&client_latest_version);
					get_git_commit(&client_latest_version);

					//比较版本是否一样
					if (guid_equal(&server_latest_version.guid, &client_latest_version.guid))
					{
						git_is_server_exist_type(git_remote_origin);
						git_is_server_exist_content(git_remote_origin);
						
						//上传文件列表的协议
						if (git_send_protocal_type(git_remote_origin, VERSION_LIST) &&
							git_send_content(git_remote_origin, get_git_project_path()))
						{
							egit_protocal_type type = NONE;
							char buf_log[8196] = { 0 };
							//阻塞工作线程
							git_recv(&type, buf_log);

							if (buf_log[0] == '\0')
							{
								log_error("buf log 为空");
								continue;
							}

							if (type == VERSION_LIST)
							{
								fgit_versions server_version_list;
								init_versions(&server_version_list);
								string_to_versions(buf_log, &server_version_list);

								if (git_remote_origin[0] != '\0')
								{
									simple_c_string c_string;
									dismantling_string(input_buf, " ", &c_string);
									char *head_value = get_string(2, &c_string);
									remove_char_end(head_value, '\n');

									//全部上传
									init_git_path_2ds(&git_path_2ds);
									if (strstr(input_buf, "git add ."))
									{
										char value[MAX_PATH] = { 0 };
										_getcwd(value, MAX_PATH);

										git_pull_local_data(&server_version_list, value, NULL);
									}
									//指定路径上传
									else
									{
										char value[MAX_PATH] = { 0 };
										_getcwd(value, MAX_PATH);

										strcat(value, "\\");
										strcat(value, head_value);

										git_pull_local_data(&server_version_list, value, head_value);
									}
									destroy_string(&c_string);
								}
								else
								{
									log_error("我们需要通过git remote add origin来打印远端路径\r\n");
								}
							}
							else
							{
								log_error("协议出错 没有获取到列表协议");
							}
						}
					}
					else
					{
						log_error("本地的数据不是最新的数据 需要通过git pull 拉取最新数据~~");
					}
				}
			}
		}
		//打印本地上传到远端的路径
		else if (strstr(input_buf, "git push -u origin master"))
		{
			const char *client_addr = get_git_project_path();
			if (git_connect(git_remote_origin, client_addr))
			{
				log_success("链接服务器成功", client_addr);

				for (int i = 0; i < git_path_2ds.size; i++)
				{
					if (git_send(git_path_2ds.paths[i].path_dis, git_path_2ds.paths[i].path_src))
					{
						add_git_versions(git_path_2ds.paths[i].path_src, &versions);
						log_log("将路径下的 %s 拉取到 %s 成功\r\n", git_path_2ds.paths[i].path_src, git_path_2ds.paths[i].path_dis);
					}
					else
					{
						log_error("将路径下的 %s 拉取到 %s 失败\r\n", git_path_2ds.paths[i].path_src, git_path_2ds.paths[i].path_dis);
					}
				}

				//版本发送
				if (git_send_protocal_type(git_remote_origin, COMMIT))
				{
					char buf_content[8196 * 1024] = { 0 };
					git_commit_to_string(&commit, buf_content);
					if (git_send_content(git_remote_origin, buf_content))
					{
						//阻塞
						git_is_server_exist_type(git_remote_origin);
						git_is_server_exist_content(git_remote_origin);

						//存储到本地
						add_new_file_buf(get_git_version_filename(), buf_content);
						log_success("版本信息发送成功");
					}			
				} 

				//列表发送
				if (git_send_protocal_type(git_remote_origin, COMMIT_VERSION_LIST))
				{
					char buf_content[8196 * 1024] = { 0 };
					versions_to_string(buf_content, &versions);
					if (git_send_content(git_remote_origin, buf_content))
					{
						//阻塞
						git_is_server_exist_content(git_remote_origin);
						git_is_server_exist_type(git_remote_origin);

						string_to_versions(buf_content, &versions_list.paths[versions_list.size++]);
						//存储到本地
						add_new_file_buf(get_git_current_version_filename(), buf_content);
						log_success("版本数据发送成功");
					}
				}

			}	
			else
			{
				log_error("服务器链接失败\r\n");
			}
		}
		//版本提交的命令
		else if (strstr(input_buf, "git commit -m"))
		{
			simple_c_string c_string;
			dismantling_string(input_buf, " ", &c_string);
			char *value = get_string(3, &c_string);
			remove_char_end(value, '\n');

			init_commit(&commit);

			char *p = get_current_time();
			remove_char_end(p, '\n');

			strcpy(commit.name, user.name);
			strcpy(commit.date, p);
			strcpy(commit.commit, value);

			create_guid(&commit.guid);

			log_log("版本提交信息为 %s", value);
		}		
		//更新数据到服务器
		else if (strstr(input_buf, "git pull"))
		{
			//迭代版本信息
			version_iteration_update();
		}
		//帮助
		else if (strstr(input_buf, "git --help"))
		{
			log_log("git --global user.name 账户的意思");
			log_log("git --global user.email email的意思");
			log_log("");
			log_log("start a working area (see also: git help tutorial)");
			log_log("git clone 可以输入一个URL 直接复制它到当前的磁盘");
			log_log("git init 初始化一个仓库");
			log_log("");
			log_log("work on the current change (see also: git help everyday)");
			log_log("git add 添加一个文件内容 可以上传到指定URL仓库");
			log_log("");
			log_log("examine the historyand state(see also : git help revisions)");
			log_log("git log 查看提交的日志信息");
			log_log("");
			log_log("collaborate (see also: git help workflows)");
			log_log("git pull 拉取URL对应的 数据，更新数据 只更新本地版本与URL仓库对应不同的内容");
			log_log("git push 把本地修改过的文件推送到URL对应的仓库中");
		}
		//警告 如果输入了其他信息
		else
		{
			log_warning("没有找到该指令 :%s", input_buf);
			log_warning("你或许可以通过 git -- help 这种方式查看相应命令");
		}
	}
}

void exit_engine()
{

}

int main()
{
	//引擎初始化
	init_engine();

	while (!b_exit)
	{
		Sleep(1000);
	}

	//退出
	exit_engine();
	printf("引擎 退出\r\n");

	return 0;
	system("pause");
}

