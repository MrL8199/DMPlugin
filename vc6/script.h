#ifndef __SCRIPT_H__
#define __SCRIPT_H__



typedef struct
{
	HANDLE thread_handle;    // �߳̾��
	int *  is_run;           // �߳̽�����־
	Idmsoft *dm;             // �߳��õ���dm����
}ScriptInfo;





ScriptInfo* StartOneScript(void);

BOOL        StopOneScript(ScriptInfo*  script_info);

BOOL        StopAllScript(vector<ScriptInfo*> & all_script_info);




#endif