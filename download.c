#define SAVED_ENVIRONMENT "/tmp/cgicsave.env"

#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILE_LEN  (1024*1024)

int cgiMain() {
	FILE *fp;
	char filebuf[MAX_FILE_LEN];
	char cmd[512];
	char *download_file_path="/app/";
	char *download_file_name;
	char full_path_name[128];
	if(cgiFormSubmitClicked("alarm_download")==cgiFormSuccess){
		download_file_name="alarm.db";
		sprintf(cmd, "%s%s", download_file_path, download_file_name);
	}else if(cgiFormSubmitClicked("anyfile_download")==cgiFormSuccess){
		if(cgiFormString("anyfile",full_path_name,128)!=cgiFormSuccess){
			cgiHeaderLocation("../download_error.html");
			goto wrong;
		}else{
			if(strcmp(full_path_name,"/app/rtu")==0){
				sprintf(cmd, "%s", full_path_name);
				download_file_name="rtu";
			}else if(strcmp(full_path_name,"/app/RTUcfg.ini")==0){
				sprintf(cmd, "%s", full_path_name);
				download_file_name="RTUcfg.ini";
			}else if(strcmp(full_path_name,"/app/pro.ini.tar")==0){
				system("tar -cf /app/pro.ini.tar /app/*.pro.ini");
				sprintf(cmd, "%s", full_path_name);
				download_file_name="pro.ini.tar";
			}else{
				cgiHeaderLocation("../download_error.html");
				goto wrong;
			}
		}
	}else{
		cgiHeaderLocation("../download_error.html");
		goto wrong;
	}
	struct stat sb;
	stat(cmd, &sb); //取待下载文件的大小
	//输出HTTP头信息，输出附加下载文件、文件长度以及内容类型
	printf("Content-Disposition:attachment;filename=%s", download_file_name);
	printf("\r\n"); 
	printf("Content-Length:%d", sb.st_size);
	printf("\r\n");
	printf("Content-Type:application/octet-stream\r\n");
	printf("\r\n");
	if(fp=fopen(cmd, "r+b")){  
		//成功打开文件，读取文件内容
		do{
			int rs = fread(filebuf, 1, sizeof(filebuf), fp);
			fwrite(filebuf, rs, 1, stdout);
		}while(!feof(fp));
		fclose(fp);
	}
	wrong: return 0;
}






