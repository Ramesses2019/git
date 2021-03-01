#include "source.h"
//simple_c_string remote_list; //�������ͻ��˵�addr

char *git_commit_to_string(const fgit_commit *git_commit, char *buf)
{
	char buf_guid[MAX_PATH] = { 0 };
	guid_to_string(buf_guid, &git_commit->guid);
	get_printf_s(buf, "%s\n%s\n%s\n%s\n", git_commit->name, git_commit->commit, git_commit->date, buf_guid);

	return buf;
}

void string_to_git_commit(fgit_commit *git_commit, const char *buf)
{
	simple_c_string c_string;
	dismantling_string(buf, "\n", &c_string);
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

void commits_to_string(const fgit_commits *git_commit, char *buf)
{
	for (int i = 0; i < git_commit->size; i++)
	{
		char buf_tmp[1024] = { 0 };
		git_commit_to_string(&git_commit->commits[i], buf_tmp);

		strcat(buf, buf_tmp);
	}
}

void init_commit(fgit_commit *out_commit)
{
	memset(out_commit->name, 0, strlen(out_commit->name));
	memset(out_commit->commit, 0, strlen(out_commit->commit));
	memset(out_commit->date, 0, strlen(out_commit->date));
	nomalization_guid(&out_commit->guid);
}

void init_versions(fgit_versions *in_versions)
{
	in_versions->size = 0;
	memset(in_versions->paths, 0, sizeof(in_versions->paths));
}

int get_index_by_commit(const char *client_crc, fgit_commit **out_commit)
{
	for (int i = 0; i < commits.size; i++)
	{
		if (guid_equal_str(client_crc, &commits.commits[i].guid))
		{
			*out_commit = &commits.commits[i + 1];
			return i;
		}
	}
	return -1;
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
	init_versions(in_versions);

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

void compare_list(const fgit_versions *in_versions_a, const fgit_versions *in_versions_b, fgit_versions *out_versions)
{
	for (int i = 0; i < in_versions_a->size; i++)
	{
		for (int j = 0; j < in_versions_b->size; j++)
		{
			if (strcmp(in_versions_a->paths[i].file_name, in_versions_b->paths[j].file_name) == 0)
			{
				if (in_versions_a->paths[i].file_size != in_versions_b->paths[j].file_size)
				{
					out_versions->paths[out_versions->size++] = in_versions_a->paths[i];
				}
			}
		}
	}
}

void commits_to_versions(const fgit_commits *git_commit, fgit_versions_list *in_versions)
{
	for (int i = 0; i < git_commit->size; i++)
	{
		char buf_guid[MAX_PATH] = { 0 };
		guid_to_string(buf_guid, &git_commit->commits[i].guid);

		char buf_path[MAX_PATH] = { 0 };
		const char *p = get_git_path();
		get_printf(buf_path, "%s\\%s.list", p, buf_guid);

		char buf_data[8192] = { 0 };
		get_file_buf(buf_path, buf_data);

		string_to_versions(buf_data, &in_versions->paths[in_versions->size++]);
	}
}

bool git_is_exist(const char *in_addr)
{
	for (int i = 0; i < clients.size; i++)
	{
		if (strcmp(clients.addr[i], in_addr) == 0)
		{
			return true;
		}
	}
	return false;
}

void git_add_client(const char *in_addr)
{
	strcpy(clients.addr[clients.size++], in_addr);
}

const char * get_project_path()
{
	if (git_project_path[0] == '\0')
	{
		_getcwd(git_project_path, MAX_PATH);
	}
	return git_project_path;
}

const char * get_git_path()
{
	if (git_path[0] == '\0')
	{
		char buf_project_path[MAX_PATH] = { 0 };
		_getcwd(buf_project_path, MAX_PATH);

		strcat(git_path, buf_project_path);
		strcat(git_path, "\\.git\\");
	}
	return git_path;
}

const char * get_git_server_cache_path()
{
	if (git_server_cache_path[0] == '\0')
	{
		_getcwd(git_server_cache_path, MAX_PATH);
		strcat(git_server_cache_path, "\\git_server");
	}

	return git_server_cache_path;
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
	memset(git_version_list_filename, 0, sizeof(git_version_list_filename));

	const fgit_commit *v_l = &commits.commits[commits.size - 1];

	if (v_l)
	{
		const char *p = get_git_path();
		strcpy(git_version_list_filename, p);

		char guid_buf[MAX_PATH] = { 0 };
		guid_to_string(guid_buf, &v_l->guid);

		strcat(git_version_list_filename, guid_buf);
		strcat(git_version_list_filename, ".list");
	}

	return git_version_list_filename;
}

void init_server()
{
	const char *p = get_git_server_cache_path();
	_mkdir(p);

	const char *p_path = get_git_path();
	_mkdir(p_path);

	if (_access(get_git_version_filename(), 0) != 0)
	{
		create_file(get_git_version_filename());
	}

	//��ʼ��commit(�ύ�汾��Ϣ)
	commits.size = 0;
	memset(commits.commits, 0, sizeof(fgit_commit) * 1024);

	//��ʼ��version��ʵ�ʰ汾�б���Ϣ��
	versions_list.size = 0;
	memset(versions_list.paths, 0, sizeof(fgit_version) * 1024);

	//��ʼ���ͻ��˵�ַ�Ľṹ��
	clients.size = 0;
	memset(clients.addr, 0, sizeof(clients.addr));

	init_network_data_buf_protocal();

	git_init_mutex();

	//��ʼ���ύ����Ϣ
	char buf_info[8196] = { 0 };
	get_file_buf(get_git_version_filename(), buf_info);
	string_to_git_commits(&commits, buf_info);

	//��ʼ���汾�б�
	commits_to_versions(&commits, &versions_list);
}

//�����ͻ��� �ĸ���Э��
void listening_clients()
{
	bool b_quit = false;
	while (!b_quit)
	{
		unsigned char protocal = get_protocal();
		switch ((egit_protocal_type)protocal)
		{
		case NONE:
			break;
		case HELLO: //�ͻ������ӵ������
		{
			Sleep(500); //��ʱ����
			char buf[MAX_PATH] = { 0 };
			get_protocal_content(buf);

			if (!git_is_exist(buf))
			{
				git_add_client(buf);
			}
			
			const char *client_addr = clients.addr[clients.size - 1];
			printf("�ɹ�����һ���ͻ��� ��ǰ�ͻ��˵�����Ϊ%d\r\n", clients.size);

			if (git_send_protocal_type(client_addr, HEI))
			{
				printf("��ͻ��˷���ָ�� ���Ѿ����ӵ���\r\n");
			}
			else
			{
				printf("��ַ���� �޷����� addr=[%s] \r\n", client_addr);
			}
			break;
		}
		case COMMIT:
		{
			Sleep(1000);

			char buf[512] = { 0 };
			get_protocal_content(buf);
			//������ݵ�commit��
			string_to_git_commit(&commits.commits[commits.size++], buf);
			add_file_buf(get_git_version_filename(), buf);
			break;
		}
		case COMMIT_VERSION_LIST:
		{
			Sleep(1000);

			char buf[1024 * 1024] = { 0 };
			get_protocal_content(buf);
			//������ݵ�version_list��
			versions_to_string(buf, &versions_list.paths[versions_list.size++]);
			add_file_buf(get_git_current_version_filename(), buf);
			break;
		}
		case VERSION_LOG:
		{
			Sleep(500);

			char client_addr[MAX_PATH] = { 0 };
			get_protocal_content(client_addr);

			if (git_send_protocal_type(client_addr, VERSION_LOG))
			{
				char buf_commits[8196] = { 0 };
				commits_to_string(&commits, buf_commits);

				if (git_send_content(client_addr, buf_commits))
				{
					printf("\n ��ͻ��˴��� commits �ɹ�\r\n");
				}
				else
				{
					printf("\n ��ͻ��˴��� commits ʧ��\r\n");
				}
			}
			break;
		}
		case VERSION_STATUS:
		{
			Sleep(1000);

			char client_buf[MAX_PATH] = { 0 };
			get_protocal_content(client_buf);

			if (client_buf[0] != '\0')
			{
				simple_c_string c_str;
				dismantling_string(client_buf, "\n", &c_str);
				const char *client_addr = get_string(0, &c_str);
				const char *client_crc = get_string(1, &c_str);

				for (int i = 0; i < commits.size; i++)
				{
					if (guid_equal_str(client_crc, &commits.commits[i].guid))
					{
						char buf_versions[8196] = { 0 };
						versions_to_string(buf_versions, &versions_list.paths[i]);
						if (git_send_protocal_type(client_addr, VERSION_STATUS) &&
							git_send_content(client_addr, buf_versions))
						{
							printf("��ͻ��˷��� �汾�б����ݳɹ�\r\n");
						}
						else
						{
							printf("��ͻ��˷��� �汾�б�����ʧ��\r\n");
							if (git_send_protocal_type(client_addr, PROTOCAL_ERROR))
							{
								printf("\n���ʹ�����Ϣ���ͻ��˳ɹ�\r\n");
							}
						}
					}
				}
			}
		}
		case VERSION_PROTOCAL:
		{
			fgit_commit *commit = &commits.commits[commits.size - 1];
			if (commit)
			{
				char client_addr[MAX_PATH] = { 0 };
				get_protocal_content(client_addr);

				char buf_tmp[1024] = { 0 };
				git_commit_to_string(commit, buf_tmp);

				if (git_send_protocal_type(client_addr, VERSION_PROTOCAL) &&
					git_send_content(client_addr, buf_tmp))
				{
					printf("\n �����������°汾���͵��ͻ��˳ɹ� \r\n");
				}
			}
			break;
		}
		case VERSION_NEXT:
		{
			char client_buf[MAX_PATH] = { 0 };
			get_protocal_content(client_buf);

			simple_c_string c_str;
			dismantling_string(client_buf, "\n", &c_str);
			const char *client_crc = get_string(0, &c_str);
			const char *client_addr = get_string(1, &c_str);

			//����ͻ���guid��commits�е�guid��ͬ ���ҵ���һ���汾����commit
			fgit_commit *commit = NULL;
			//int index = get_index_by_commit(client_crc, &commit) + 1;
			int index = -1;
			for (int i = 0; i < commits.size; i++)
			{
				if (guid_equal_str(client_crc, &commits.commits[i].guid))
				{				
					commit = &commits.commits[i + 1];
					index = i + 1;	
					break;
				}

			}

			if (commit)
			{
				char buf_tmp[1024] = { 0 };
				git_commit_to_string(commit, buf_tmp);
				if (git_send_protocal_type(client_addr, VERSION_NEXT) &&
					git_send_content(client_addr, buf_tmp))
				{
					printf("\n ����������һ�汾���Ϳͻ��˳ɹ� \r\n");
				
					fgit_versions *git_versions_tmp = &versions_list.paths[index];
					//�������ݣ��ļ��Ŀ�����
					for (int i = 0; i < git_versions_tmp->size; i++)
					{
						const char *file_path = git_versions_tmp->paths[i].file_name;
						char client_url_param[MAX_PATH] = { 0 };
						char server_url_param[MAX_PATH] = { 0 };

						strcpy(client_url_param, client_addr);
						strcpy(server_url_param, get_project_path());

						strcat(client_url_param, file_path);
						strcat(server_url_param, "\\");
						strcat(server_url_param, file_path);

						//Ƕ�׵�·����Ҫ�ֶ�����
						normalization_path(client_url_param);
						char d_path[MAX_PATH] = { 0 };
						normalization_directory(d_path, client_url_param);
						create_file_directory(d_path);

						if (git_send(client_url_param, server_url_param))
						{
							printf("\n ��·����%s ���䵽%s �ɹ�\r\n", server_url_param, client_url_param);
						}
						else
						{
							set_consle_w_color(SIMPLE_RED, 0);
							printf("\n ��·����%s ���䵽%s ʧ��\r\n", server_url_param, client_url_param);
							set_consle_w_color(SIMPLE_WHITE, 0);
						}
					}

					//˯�߷�ֹ���͵��ļ����ݱ�����
					Sleep(1000);

					char buf_tmp[8196] = { 0 };
					versions_to_string(buf_tmp, git_versions_tmp);

					if (git_send_protocal_type(client_addr, VERSION_NEXT_COMPLETE) &&
						git_send_content(client_addr, buf_tmp))
					{
						printf("\n ���ǵ�ǰ��һ�汾�������б���ɹ� \r\n");
					}
				}
			}
			break;
		}
		case VERSION_CLASH:
		{
			//crc addr version
			char client_buf[1024] = { 0 };
			get_protocal_content(client_buf);

			simple_c_string c_str;
			dismantling_string(client_buf, "|", &c_str);
			const char *client_crc = get_string(0, &c_str);
			const char *client_addr = get_string(1, &c_str);
			const char *git_versions_buf = get_string(2, &c_str);

			fgit_commit *commit = NULL;
			int index = get_index_by_commit(client_crc, &commit);

			fgit_versions client_versions;
			string_to_versions(git_versions_buf, &client_versions);

			fgit_versions *server_versions = &versions_list.paths[index];
			
			//�Ƚ��ҳ��ͻ��˺ͷ����versions�в���ͬ�Ĳ���
			fgit_versions out_versions;
			init_versions(&out_versions);
			compare_list(&client_versions, server_versions, &out_versions);
			
			if (out_versions.size == 0 || 
				index == commits.size - 1) //�Ѿ������°汾
			{
				if (git_send_protocal_type(client_addr, NO_CLASH))
				{
					printf(("\n û�г�ͻ \n"));
				}
			}
			else
			{
				char buf_tmp[8196 * 10] = { 0 };
				versions_to_string(buf_tmp, &out_versions);

				if (git_send_protocal_type(client_addr, CLASH_CONTENT) &&
					git_send_content(client_addr, buf_tmp))
				{
					printf("\n �г�ͻ \n");
				}
			}

			destroy_string(&c_str);

			break;
		}
		case VERSION_LIST:
		{
			char client_addr[MAX_PATH] = { 0 };
			get_protocal_content(client_addr);

			fgit_versions *server_versions = &versions_list.paths[versions_list.size - 1];

			char buf_versions[8196 * 1024] = { 0 };
			versions_to_string(buf_versions, server_versions);

			if (git_send_protocal_type(client_addr, VERSION_LIST) &&
				git_send_content(client_addr, buf_versions))
			{
				printf("\n �ɹ���ͻ��˷��������ǵİ汾�б� \n");
			}

			break;
		}
		default:
			break;
		}

		//������ÿ��0.5����������Ϣ���͹���
		Sleep(500);
	}

}

void close_server()
{
	
}

unsigned char get_protocal()
{
	char path_tmp[MAX_PATH] = { 0 };
	unsigned char ret = 0;

	const char *p = get_git_server_cache_path();
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

void get_protocal_content(char *buf)
{
	char path_buf[MAX_PATH] = { 0 };
	strcpy(path_buf, get_git_server_cache_path());
	strcat(path_buf, "\\content.protocal");
	get_file_buf(path_buf, buf);

	remove(path_buf);
}

//��ǰ��git�汾������
int main()
{
	init_server();

	listening_clients();

	close_server();
	return 0;
	system("pause");
}