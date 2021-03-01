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



//����ģʽ ��ȡ�汾
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
			printf("����ִ��git init���� ��ʼ��һ�����زֿ�\r\n");
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
	//��ȡ���·��
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

//�Ƚϲ�һ����
//�ȽϷ�����û�е��ͻ����� ����ӵ�������
//ͬʱ����ͻ���û�е� �������е� ֪ͨ������ɾ�����ļ�
void compare_list(const fgit_versions *server_version_list, const def_paths *local_paths, def_paths *out_paths)
{
	const char *client_url = get_git_project_path();

	//��ӣ���һ���ģ�
	for (int i = 0; i < local_paths->index; i++)
	{
		char path_tmp[MAX_PATH] = { 0 };
		strcpy(path_tmp, local_paths->paths[i]);
		remove_string_start(path_tmp, client_url);

		int size = get_file_size_by_filename(local_paths->paths[i]);

		bool b_exit = false;
		for (int j = 0; j < server_version_list->size; j++)
		{
			if (strcmp(server_version_list->paths[j].file_name, path_tmp) == 0) //�Ƚ����·��
			{
				if (size != server_version_list->paths[j].file_size)
				{
					strcpy(out_paths->paths[out_paths->index++], local_paths->paths[i]); //����·��
				}
				b_exit = true;
				break;
			}		
		}

		if (!b_exit)
		{
			strcpy(out_paths->paths[out_paths->index++], local_paths->paths[i]); //����·��
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
			//�ͻ��˲�����HELLO
		//case HELLO://�ͻ��˽��з���
		//{
		//	printf("server �յ���Э����� %i\r\n", protocal);
		//	break;
		//}		
		case HEI: //����˽��з���ȷ��
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
			log_error("���� ����~");
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
	//ѭ��
	engine_loop();

	return 0;
}

void init_engine()
{
	//��ʼ����־����Ŀ·��
	init_log_system(get_git_project_path());

	char *p = get_git_client_cache_path();
	_mkdir(p);

	//��ʼ����
	git_init_mutex();

	init_network_data_buf_protocal();

	commit.b_init = false;

	//��ȡ�û����ñ�
	read_user();

	//�����߳�
	//����2 ��ջ��С����ϵͳ�������ṩ��ֵ�趨��
	//����5 0�����߳�����ʱ�ͻ��Լ�ִ��
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
		//ͨ�����з���ÿһ�е����ݽ����и�
		dismantling_string(buf_cofg, "\n", &c_str_sentences);

		for (int i = 0; i < c_str_sentences.size; i++)
		{
			char *tmp = get_string(i, &c_str_sentences);
			//������˻�
			if (strstr(tmp, "account="))
			{
				dismantling_string_by_index(1, tmp, "=", user.name);
			}
			//���������
			else if (strstr(tmp, "password="))
			{
				dismantling_string_by_index(1, tmp, "=", user.password);
			}
			//���������
			else if (strstr(tmp, "email="))
			{
				dismantling_string_by_index(1, tmp, "=", user.email);
			}
			//�����Զ������
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
	//����
	git_is_server_exist_type(git_remote_origin);
	git_is_server_exist_content(git_remote_origin);
	//1 ��⵱ǰ�汾�Ƿ�Ϊ���� ��ȡ���������°汾�� Ȼ��ȽϿ��Ƿ���ͬ
	if (git_send_protocal_type(git_remote_origin, VERSION_PROTOCAL)&&
		git_send_content(git_remote_origin,get_git_project_path()))
	{
		log_success("��ȡ������ ���°汾��\r\n");

		egit_protocal_type type = NONE;
		char buf_log[8196] = { 0 };
		//���������߳�
		git_recv(&type, buf_log);

		if (buf_log[0] == '\0')
		{
			log_error("buf log Ϊ��");
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

			//�Ƚϰ汾�Ƿ�һ��
			if (!guid_equal(&server_latest_version.guid, &client_latest_version.guid))
			{
				const char *client_url = get_git_project_path();

				char str_content[8196] = { 0 };
				guid_to_string(str_content, &client_latest_version.guid);
				strcat(str_content, "|");
				strcat(str_content, client_url);
				strcat(str_content, "|");

				//����
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

					//����version�ĸ�ʽƴ��
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
					//���������߳�
					git_recv(&type, buf_log);
					if (type == CLASH_CONTENT)
					{
						log_error("%s", buf_log);
						log_error("�������ϳ�ͻ �ٽ��и�������...");
					}
					else if (type == NO_CLASH)
					{
						//2 �ѵ�ǰ�Ŀͻ��˰汾���͵������
						//char str_content[1024] = { 0 };
						memset(str_content, 0, sizeof(str_content));
						guid_to_string(str_content, &client_latest_version.guid);

						strcat(str_content, "\n");
						strcat(str_content, get_git_project_path());
						//����
						git_is_server_exist_type(git_remote_origin);
						git_is_server_exist_content(git_remote_origin);

						if (git_send_protocal_type(git_remote_origin, VERSION_NEXT) &&
							git_send_content(git_remote_origin, str_content))
						{
							log_success("��ȡ��һ����Ҫ���µİ汾\r\n");

							//3 �����°汾�� �����ڱ��� �滻
							type = NONE;
							memset(buf_log, 0, sizeof(buf_log));
							//���������߳�
							git_recv(&type, buf_log);

							if (buf_log[0] == '\0')
							{
								log_error("buf log Ϊ��");
								return;
							}

							if (type == VERSION_NEXT)
							{
								add_new_file_buf(get_git_version_filename(), buf_log);
							}

							type = NONE;
							memset(buf_log, 0, sizeof(buf_log));
							//���������߳�
							git_recv(&type, buf_log);

							if (buf_log[0] == '\0')
							{
								log_error("buf log Ϊ��");
								return;
							}

							if (type == VERSION_NEXT_COMPLETE)
							{
								add_new_file_buf(get_git_current_version_filename(), buf_log);

								//4 ��⵱ǰ�汾�Ƿ�������
								//�Ȳ����ǳ�ͻ
								//�ݹ� һֱ���ֱ����ǰ�İ汾Ϊ����
								version_iteration_update();
							}
						}
					}
					else
					{
						log_log("�Ѿ���������...");
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

	//ֻ�ύ�����ı���ļ�
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

		//ȥ���ļ� ֻ����·����
		{
			char buf_remote_path_tmp1[MAX_PATH] = { 0 };
			strcpy(buf_remote_path_tmp1, buf_remote_path_tmp);
			//ȥ��ĩβ�������ļ���Ϊ�˴����ļ��У�
			get_path_directory_inline(buf_remote_path_tmp1);

			//�������е��ļ��У��������ļ��⣩
			if (!create_file_directory(buf_remote_path_tmp1))
			{
				log_error("������ ������һ����ͻ %s\r\n", buf_remote_path_tmp1);
			}
		}

		git_path_2ds.size = out_paths.index + 1;
		strcpy(git_path_2ds.paths[i].path_src, out_paths.paths[i]);
		strcpy(git_path_2ds.paths[i].path_dis, buf_remote_path_tmp);
		/*	if (copy_file(paths.paths[i],buf_remote_path_tmp))
			{
				log_log("��·���µ� %s ��ȡ�� %s �ɹ�\r\n", paths.paths[i], buf_remote_path_tmp);
			}
			else
			{
				log_error("��·���µ� %s ��ȡ�� %s ʧ��\r\n", paths.paths[i], git_remote_origin);
			}*/

		int b_value = ((double)i / (double)out_paths.index) * 100 + 1;
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("��ǰ��������Զ�����ݣ�%3d%%", b_value);
	}
	printf("��ǰ��������Զ�����ݣ�%3d%%", 100);
}

void engine_loop()
{
	//git_send_protocal_type(git_remote_origin, GIT_PULL);

	char input_buf[1024] = { 0 };
	//ѭ��
	//ͨ�����������ó����˳�
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

		//�������exit�ַ������˳�
		if (strstr(input_buf, "exit"))
		{
			b_exit = 1;
		}
		//�������git init�ַ����ͽ��������ʼ���Ĳ���
		else if (strstr(input_buf, "git init"))
		{
			if (git_project_path[0] == '\0')
			{
				char tmp_path[256] = { 0 };
				strcpy(tmp_path, ".\\.git\\");
				_mkdir(tmp_path);
				strcat(tmp_path, "git.txt"); //��¼�汾��Ϣ

				if (create_file(tmp_path))
				{
					log_success("���س�ʼ���ɹ�");
				}
				else
				{
					log_error("������Ŀ·�� .git ʧ��");
				}
			}
			/*char *p = get_git_path();
			log_success("��ʼ�� [%s] ·���ɹ�",p);*/
		}
		//�鿴�汾��Ϣ��
		else if (strstr(input_buf, "git log"))
		{
			git_is_server_exist_type(git_remote_origin);
			git_is_server_exist_content(git_remote_origin);

			if (git_send_protocal_type(git_remote_origin, VERSION_LOG))
			{
				const char *client_addr = get_git_project_path();
				if (git_send_content(git_remote_origin, client_addr))
				{
					log_success("�ͻ��˵�ַ ���ͳɹ�");

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
								printf("\n\n��   ��  �룺%s \r\n", guid_buf);
								set_consle_w_color(SIMPLE_WHITE, 0);
								printf("\n�� �� �� Ա��%s \r\n", git_commits.commits[i].name);
								printf("\n�� �� �� Ϣ��%s \r\n", git_commits.commits[i].commit);
								printf("\n��       �ڣ�%s \r\n", git_commits.commits[i].date);
							}
						}
						else
						{
							log_error("���շ���� ���͵�bugû���κ��м�ֵ����Ϣ");
						}
					}
					else
					{
						log_error("�ͻ��˽���ʧ��");
					}

				}
			}
		}
		//�鿴�ļ�����״̬
		else if (strstr(input_buf, "git status"))
		{
			//����
			git_is_server_exist_type(git_remote_origin);
			git_is_server_exist_content(git_remote_origin);

			if (git_send_protocal_type(git_remote_origin, VERSION_STATUS))
			{
				//..\\����·��
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
					log_success("��ַ �Ͱ汾 guid ���ͳɹ�");

					egit_protocal_type type = NONE;
					char buf_log[8196] = { 0 };
					//���������߳�
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
								printf("\n�޸ĵ�����Ϊ%s\r\n", git_versions.paths[i].file_name);
							}
							set_consle_w_color(SIMPLE_WHITE, 0);
						}
					}
				}
			}
		}	
		//Զ�˲���
		else if (strstr(input_buf, "git remote add origin"))
		{
			dismantling_string_by_index(4, input_buf, " ", git_remote_origin);
			log_success("����git remote add origin�ɹ� %s", git_remote_origin);

			save_user();
		}
		//�ʼ�����
		else if (strstr(input_buf, "git --global user.email"))
		{
			dismantling_string_by_index(3, input_buf, " ", user.email);
			log_success("���� email�ɹ� %s", user.email); //д����־

			save_user();
		}
		//�������ֵĲ���
		else if (strstr(input_buf, "git --global user.name"))
		{
			dismantling_string_by_index(3, input_buf, " ", user.name);
			log_success("���� name�ɹ� %s", user.name); //д����־

			save_user();
		}
		//��������Ĳ���
		else if (strstr(input_buf, "git --global user.password"))
		{
			dismantling_string_by_index(3, input_buf, " ", user.password);
			log_success("���� password�ɹ� %s", user.password); //д����־

			save_user();
		}
		//����
		else if (strstr(input_buf, "ssh-keygen -t rsa -C"))
		{
			log_error("ssh-keygen -t rsa -C ��ָ�δ���");
		}
		//��¡����
		else if (strstr(input_buf, "git clone"))
		{
			simple_c_string c_string;
			dismantling_string(input_buf, " ", &c_string);
			char *value = get_string(2, &c_string);
			remove_char_end(value, '\n');

			//�ҵ�value�µ������ļ�
			def_paths paths;
			init_def_paths(&paths);
			find_files(value, &paths, true);

			//�ж������ַָ��� ����ֿ�
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
				log_error("��Ч �Ǳ�׼·�� %s", value);
				break;
			}

			//��ȡ�����ļ���
			char *file_value = get_string(c_file.size - 1, &c_file);
			remove_char_end(file_value, '\n');

			//��ȡ���ع���·����
			char buf_path[MAX_PATH];
			_getcwd(buf_path, MAX_PATH);

			//�ѱ��ع���·����Զ���ṩ������ļ���� �ñ���Ҳ����һ�����ļ�
			char buf_local_path[MAX_PATH] = { 0 };
			get_printf(buf_local_path,"%s\\%s", buf_path, file_value);

			char *buf_success[MAX_PATH] = { 0 };
			char *buf_error[MAX_PATH] = { 0 };
			for (int i = 0; i < paths.index; i++)
			{
				char buf_tmp[MAX_PATH] = { 0 };
				strcpy(buf_tmp, paths.paths[i]);
				//������Զ�˵�ַ��ǰ����ɾ�� ʣ���������ļ�
				remove_string_start(buf_tmp, value);

				//ճ���������ļ�����Ӻõı����ļ���
				//����һ���µ���ʱ��ű��ص�ַ���ַ��� ��ֹ�ڱ����ַ����ϲ���������
				char buf_local_path_tmp[MAX_PATH] = { 0 };
				strcpy(buf_local_path_tmp, buf_local_path);
				strcat(buf_local_path_tmp, buf_tmp);

				//ȥ���ļ� ֻ����·����
				{
					char buf_local_path_tmp1[MAX_PATH] = { 0 };
					strcpy(buf_local_path_tmp1, buf_local_path_tmp);
					//ȥ��ĩβ�������ļ���Ϊ�˴����ļ��У�
					get_path_directory_inline(buf_local_path_tmp1);

					//�������е��ļ��У��������ļ��⣩
					if (!create_file_directory(buf_local_path_tmp1))
					{
						log_error("������һ����ͻ %s\r\n", buf_local_path_tmp1);
					}
				}

				//��Զ���ṩ�������ļ������գ����������ش������ļ���
				if (copy_file(paths.paths[i], buf_local_path_tmp))
				{
					buf_success[i] = buf_local_path_tmp;
					//log_log("��·���µ� %s ��ȡ�� %s �ɹ�\r\n", paths.paths[i], buf_local_path_tmp);
				}
				else
				{
					buf_error[i] = buf_local_path;
					//log_error("��·���µ� %s ��ȡ�� %s ʧ��\r\n", paths.paths[i], buf_local_path);
				}

				int b_value = ((double)i / (double)paths.index) * 100 + 1;
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				printf("��ǰ��������Զ�����ݣ�%3d%%", b_value);
			}

			//������ɺ��ӡ��־
			for (int j = 0; j < paths.index; j++)
			{
				if (buf_success[j] != '\0')
				{
					log_log("��·���µ� %s ��ȡ�� %s �ɹ�\r\n", paths.paths[j], buf_success[j]);
				}
				else if (buf_error[j] != '\0')
				{
					log_error("��·���µ� %s ��ȡ�� %s ʧ��\r\n", paths.paths[j], buf_error[j]);
				}
			}

			destroy_string(&c_file);
			destroy_string(&c_string);
		}
		//�ļ����ϴ�
		else if (strstr(input_buf,"git add"))
		{
			git_is_server_exist_type(git_remote_origin);
			git_is_server_exist_content(git_remote_origin);
			//1 ��⵱ǰ�汾�Ƿ�Ϊ���� ��ȡ���������°汾�� Ȼ��ȽϿ��Ƿ���ͬ
			if (git_send_protocal_type(git_remote_origin, VERSION_PROTOCAL) &&
				git_send_content(git_remote_origin, get_git_project_path()))
			{
				log_success("��ȡ������ ���°汾��\r\n");

				egit_protocal_type type = NONE;
				char buf_log[8196] = { 0 };
				//���������߳�
				git_recv(&type, buf_log);

				if (buf_log[0] == '\0')
				{
					log_error("buf log Ϊ��");
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

					//�Ƚϰ汾�Ƿ�һ��
					if (guid_equal(&server_latest_version.guid, &client_latest_version.guid))
					{
						git_is_server_exist_type(git_remote_origin);
						git_is_server_exist_content(git_remote_origin);
						
						//�ϴ��ļ��б��Э��
						if (git_send_protocal_type(git_remote_origin, VERSION_LIST) &&
							git_send_content(git_remote_origin, get_git_project_path()))
						{
							egit_protocal_type type = NONE;
							char buf_log[8196] = { 0 };
							//���������߳�
							git_recv(&type, buf_log);

							if (buf_log[0] == '\0')
							{
								log_error("buf log Ϊ��");
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

									//ȫ���ϴ�
									init_git_path_2ds(&git_path_2ds);
									if (strstr(input_buf, "git add ."))
									{
										char value[MAX_PATH] = { 0 };
										_getcwd(value, MAX_PATH);

										git_pull_local_data(&server_version_list, value, NULL);
									}
									//ָ��·���ϴ�
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
									log_error("������Ҫͨ��git remote add origin����ӡԶ��·��\r\n");
								}
							}
							else
							{
								log_error("Э����� û�л�ȡ���б�Э��");
							}
						}
					}
					else
					{
						log_error("���ص����ݲ������µ����� ��Ҫͨ��git pull ��ȡ��������~~");
					}
				}
			}
		}
		//��ӡ�����ϴ���Զ�˵�·��
		else if (strstr(input_buf, "git push -u origin master"))
		{
			const char *client_addr = get_git_project_path();
			if (git_connect(git_remote_origin, client_addr))
			{
				log_success("���ӷ������ɹ�", client_addr);

				for (int i = 0; i < git_path_2ds.size; i++)
				{
					if (git_send(git_path_2ds.paths[i].path_dis, git_path_2ds.paths[i].path_src))
					{
						add_git_versions(git_path_2ds.paths[i].path_src, &versions);
						log_log("��·���µ� %s ��ȡ�� %s �ɹ�\r\n", git_path_2ds.paths[i].path_src, git_path_2ds.paths[i].path_dis);
					}
					else
					{
						log_error("��·���µ� %s ��ȡ�� %s ʧ��\r\n", git_path_2ds.paths[i].path_src, git_path_2ds.paths[i].path_dis);
					}
				}

				//�汾����
				if (git_send_protocal_type(git_remote_origin, COMMIT))
				{
					char buf_content[8196 * 1024] = { 0 };
					git_commit_to_string(&commit, buf_content);
					if (git_send_content(git_remote_origin, buf_content))
					{
						//����
						git_is_server_exist_type(git_remote_origin);
						git_is_server_exist_content(git_remote_origin);

						//�洢������
						add_new_file_buf(get_git_version_filename(), buf_content);
						log_success("�汾��Ϣ���ͳɹ�");
					}			
				} 

				//�б���
				if (git_send_protocal_type(git_remote_origin, COMMIT_VERSION_LIST))
				{
					char buf_content[8196 * 1024] = { 0 };
					versions_to_string(buf_content, &versions);
					if (git_send_content(git_remote_origin, buf_content))
					{
						//����
						git_is_server_exist_content(git_remote_origin);
						git_is_server_exist_type(git_remote_origin);

						string_to_versions(buf_content, &versions_list.paths[versions_list.size++]);
						//�洢������
						add_new_file_buf(get_git_current_version_filename(), buf_content);
						log_success("�汾���ݷ��ͳɹ�");
					}
				}

			}	
			else
			{
				log_error("����������ʧ��\r\n");
			}
		}
		//�汾�ύ������
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

			log_log("�汾�ύ��ϢΪ %s", value);
		}		
		//�������ݵ�������
		else if (strstr(input_buf, "git pull"))
		{
			//�����汾��Ϣ
			version_iteration_update();
		}
		//����
		else if (strstr(input_buf, "git --help"))
		{
			log_log("git --global user.name �˻�����˼");
			log_log("git --global user.email email����˼");
			log_log("");
			log_log("start a working area (see also: git help tutorial)");
			log_log("git clone ��������һ��URL ֱ�Ӹ���������ǰ�Ĵ���");
			log_log("git init ��ʼ��һ���ֿ�");
			log_log("");
			log_log("work on the current change (see also: git help everyday)");
			log_log("git add ���һ���ļ����� �����ϴ���ָ��URL�ֿ�");
			log_log("");
			log_log("examine the historyand state(see also : git help revisions)");
			log_log("git log �鿴�ύ����־��Ϣ");
			log_log("");
			log_log("collaborate (see also: git help workflows)");
			log_log("git pull ��ȡURL��Ӧ�� ���ݣ��������� ֻ���±��ذ汾��URL�ֿ��Ӧ��ͬ������");
			log_log("git push �ѱ����޸Ĺ����ļ����͵�URL��Ӧ�Ĳֿ���");
		}
		//���� ���������������Ϣ
		else
		{
			log_warning("û���ҵ���ָ�� :%s", input_buf);
			log_warning("��������ͨ�� git -- help ���ַ�ʽ�鿴��Ӧ����");
		}
	}
}

void exit_engine()
{

}

int main()
{
	//�����ʼ��
	init_engine();

	while (!b_exit)
	{
		Sleep(1000);
	}

	//�˳�
	exit_engine();
	printf("���� �˳�\r\n");

	return 0;
	system("pause");
}

