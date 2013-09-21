#include "StdAfx.h"


static void script_main(Idmsoft & dm,int * is_run)
{
	// ִ�нű����� �����Ҿ͵����汾��
	MessageBox(NULL,dm.Ver(),"",MB_OK);

	Sleep(1000);
}
static unsigned WINAPI EntryScript(LPVOID pParam)
{
    ScriptInfo *script_info  = (ScriptInfo *)pParam;


	// ��Ҫ���߳��ﴴ��dm����.����������ܿ����߳�.

	// ��ʼ�ű�ѭ��
	while (1)
	{
		// ���ű��Ƿ�Ҫ����
		if (*script_info->is_run == 0)
			break;
		
		script_main(*script_info->dm,script_info->is_run);
	}


	return 1;
	
}

ScriptInfo * malloc_script_info(void)
{
	ScriptInfo *script_info = (ScriptInfo *)malloc(sizeof(ScriptInfo));
	if (script_info == NULL)
		return NULL;
	

	script_info->is_run = (int *)malloc(sizeof(int));
	if (script_info->is_run == NULL)
	{
		free(script_info);
		return NULL;
	}
	
	script_info->dm = new Idmsoft;
	if (script_info->dm == NULL)
	{
		free(script_info->is_run);
		free(script_info);
		return NULL;
	}

	return script_info;
}

void free_script_info(ScriptInfo * script_info)
{
	if (script_info)
	{
		if (script_info->is_run)
			free(script_info->is_run);

		if (script_info->dm)
			delete script_info->dm;
		
		free(script_info);
	}
}
ScriptInfo*  StartOneScript(void)
{
	// ����ռ�
	ScriptInfo *script_info = malloc_script_info();

	if (script_info == NULL)
		return NULL;
	
	

	// ������Į����, �������߳��ⴴ���������̱߳�ǿ�ƹرգ����°󶨴��ڹҵ�
	if (script_info->dm->CreateDispatch("dm.dmsoft") == FALSE)
	{
		free_script_info(script_info);
		return NULL;
	}
	
	// ����Ϊ����״̬
	*script_info->is_run = 1;

	// �����߳�
	unsigned thread_id = 0;
	script_info->thread_handle = (HANDLE)_beginthreadex(NULL,0,EntryScript,(LPVOID)script_info,0,(unsigned *)&thread_id);
	
	if (script_info->thread_handle == INVALID_HANDLE_VALUE)
	{
		free_script_info(script_info);
		return NULL;
	}
	
	return script_info;
}

BOOL        StopOneScript(ScriptInfo  *script_info)
{
	// ���ñ�־λ
	*script_info->is_run = 0;
	// �ȴ����3��,�����ʱ,��ǿ�ƽ����߳�.Σ��.(����Դй©)
	if (WAIT_OBJECT_0 != WaitForSingleObject(script_info->thread_handle,3000))
	{
		// �Ƚ�󴰿�. ���ⴰ�ڱ���
		script_info->dm->UnBindWindow();
		TerminateThread(script_info->thread_handle,0);
	}
	CloseHandle(script_info->thread_handle);


	free_script_info(script_info);
	
	return TRUE;
}

BOOL        StopAllScript(vector<ScriptInfo *> & all_script_info)
{
	// �Ȱ����б�־λ��0
	int i;
	for (i = 0;i < all_script_info.size();++i)
	{
		*all_script_info[i]->is_run = 0;
	}
	
	// �ȴ�����
	for (i = 0;i < all_script_info.size();++i)
	{
		// �ȴ����3��,�����ʱ,��ǿ�ƽ����߳�.Σ��.(����Դй©)
		if (WAIT_OBJECT_0 != WaitForSingleObject(all_script_info[i]->thread_handle,3000))
		{
			// �Ƚ�󴰿�. ���ⴰ�ڱ���
			all_script_info[i]->dm->UnBindWindow();
			TerminateThread(all_script_info[i]->thread_handle,0);
		}
		CloseHandle(all_script_info[i]->thread_handle);
		free_script_info(all_script_info[i]);
	}
	
	all_script_info.clear();
	return TRUE;
}