#include "./task.h"


int getPidByName(pid_t *pid, char *task_name)
{

	////1. open the /proc , and every read, it will return the next dir struct dirent
	DIR * dirp;
	dirp = opendir("/proc");
	if(NULL == dirp)
	{
		perror("opendir");
		return -1;
	}

	//2.read the directory
	struct dirent *dp;
	while(NULL != (dp = readdir(dirp)))
	{
		//if direactory is . or .. or not a direct, skip
		if((dp->d_name == ".") || (dp->d_name == ".."))
			continue;
		if(DT_DIR != dp->d_type)
			continue;

		//open the /proc/pid/status file
		char fileName[128];
		FILE *fd ;
		sprintf(fileName, "/proc/%s/status", dp->d_name);
		fd = fopen(fileName, "r");

		if(NULL != fd)
		{
			//copy the /proc/pid/status content to buf
			char buf[128];
			fgets(buf, sizeof(buf), fd);

			//
			char cur_task[32] ;
			sscanf(buf, "%*s%s", cur_task);//"%*s%s" 表示读取"Name:   nginx" 中nginx到cur_task中

			if(!strcmp(cur_task, task_name))
			{
				sscanf(dp->d_name, "%d", pid);
			}
		}
	}

	return 0;
}



int getNameByPid(pid_t pid, char *task_name)
{
	char fileName[128];
	sprintf(fileName, "/proc/%d/status", pid);

	FILE *fd;
	fd  = fopen(fileName, "r");
	if(NULL == fd)
	{
		perror("fopen");
		return -1;
	}

	char buf[128];
	fgets(buf, sizeof(buf), fd);
	sscanf(buf, "%*s%s", task_name);

	return 0;

}
