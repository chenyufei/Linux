#ifndef _COPY_H_
#define _COPY_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>


typedef enum
{
	NOT_EXIST=1,
	DIRECTORY,
	FILES
}FILE_ENUM;

extern FILE_ENUM check_file(char *var, struct stat *st);    //检查文件类型

extern int cp_file(char *src_var, char *dest_var, struct stat *st);     //copy文件

extern int cp_dir(char *src, char *dest);       //copy目录
#endif
