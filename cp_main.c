/*********************************************************
 *   File name:
 *   Description:
 *   Author: Jimmy_Nie
 *   Version     Modify      Time        
 *   v1.0        creat       2017-09-12
 *
 *********************************************************/
/*
主要功能包括两种：

源文件属性为文件，拷贝到其它文件(内容复制)或目录(作为目录子文件)
源文件属性为目录，拷贝到其它目录(作为子目录存在)
*/

#include "copy.h"

int main(int argc, char *argv[])
{
	//Check the arguments
	if( 3 != argc)
	{
		printf("%s(%d): arguments error!\n",__FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	//check the source file is a file or a directory
	struct stat src_stat;

	//destination file check
	struct stat dest_stat;

	//If source is a file
	if( FILES == check_file(argv[1], &src_stat) )
	{
		FILE_ENUM dest_enum;
		dest_enum = check_file( argv[2], &dest_stat );

		char cmd[100] = "";
		char ch;

		switch(dest_enum)
		{
			case NOT_EXIST:
				sprintf(cmd, "touch %s", argv[2]);
				system( cmd );

				check_file( argv[2], &dest_stat );
				cp_file(argv[1], argv[2], &dest_stat);

				break;

			case DIRECTORY:
				cp_file(argv[1], argv[2], &dest_stat);
				break;

			case FILES:
				fprintf(stdout, "Overwrite the dest file %s, [y/n]\n", argv[2]);
				ch = getchar();

				if( ch == 'Y' || ch == 'y' )
				{
					cp_file(argv[1], argv[2], &dest_stat);
				}
				else
					exit(0);

				break;
			default:
				fprintf(stderr, "%s(%d): file type error\n", __FILE__, __LINE__);
		}
	}

	//If source file is a directory
	else if( DIRECTORY == check_file(argv[1], &dest_stat) )
	{
		FILE_ENUM dest_enum;
		dest_enum = check_file( argv[2], &dest_stat );

		char cmd[100] = "";

		switch(dest_enum)
		{
			case NOT_EXIST:
				sprintf(cmd, "mkdir -p %s", argv[2]);
				system( cmd );
				cp_dir(argv[1], argv[2] );
				break;

			case DIRECTORY:
				cp_dir(argv[1], argv[2]); 
				break;

			case FILES:
				fprintf(stderr, "Can't copy a directory to a file\n");
				exit(EXIT_FAILURE);
				break;

			default:
				fprintf(stderr, "%s(%d): file type error\n", __FILE__, __LINE__);
				break;
		}
	}

	return 0;
}
