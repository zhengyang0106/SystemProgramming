/*************************************************************************
	> File Name: 001.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年04月25日 星期四 11时49分21秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>
#include<math.h>
#include<string.h>
#include<signal.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<grp.h>
#define PATH_SIZE 1000


char buf1[80];
char buf[80];//存放路径
struct passwd *pwd;
char hostname[60] = {'\0'};
int home_length;

void ls_command() {
    char name[PATH_SIZE][PATH_SIZE];
    char *p;
    DIR *dir;
    struct dirent *dir_info;
    int i = 0;
    struct stat buf2;
    struct group *grp;
    char type;
    char permission[9];
    memset(permission,'-',9*sizeof(char) );
    memset(name,'\0',PATH_SIZE * sizeof(char) );

    dir = opendir(buf);
    while(NULL != (dir_info = readdir(dir)) )
        strcpy (name[i++],dir_info->d_name);
    closedir(dir);
    while(--i >= 0) {
        if(!stat(name[i],&buf2)) {
            //获取文件类型
            if(S_ISLNK(buf2.st_mode))
                type = 'l';
            else if(S_ISREG(buf2.st_mode))
                type = '-';
            else if(S_ISDIR(buf2.st_mode))
                type = 'd';
            else if(S_ISCHR(buf2.st_mode))
                type = 'c';
            else if(S_ISBLK(buf2.st_mode))
                type = 'b';
            else if(S_ISFIFO(buf2.st_mode))
                type = 'p';
            else if(S_ISSOCK(buf2.st_mode))
                type = 's';
            //获取权限
            if(buf2.st_mode & S_IRUSR)
                permission[0] = 'r';
            if(buf2.st_mode & S_IWUSR)
                permission[1] = 'w';
            if(buf2.st_mode & S_IXUSR)
                permission[2] = 'x';
            if(buf2.st_mode & S_IRGRP)
                permission[3] = 'r';
            if(buf2.st_mode & S_IWGRP)
                permission[4] = 'w';
            if(buf2.st_mode & S_IXGRP)
                permission[5] = 'x';
            if(buf2.st_mode & S_IROTH)
                permission[6] = 'r';
            if(buf2.st_mode & S_IWOTH)
                permission[7] = 'w';
            if(buf2.st_mode & S_IXOTH)
                permission[8] = 'x';
            //获取用户名和组名
            pwd = getpwuid(buf2.st_uid);
            grp = getgrgid(buf2.st_gid);
            int j = 0;
            printf("%c",type);
            while(j<9){
                printf("%c",permission[j]);
                j++;
            }
            //链接文件数
            printf("  %3d  ",buf2.st_nlink);
            //文件名组名
            printf("%-4s  %-4s",pwd->pw_name,grp->gr_name);
            //文件大小
            printf("%8ld  ",buf2.st_size);
            //文件时间
            printf("%.12s  ",ctime(&buf2.st_mtime)+4);
            //文件名
            printf("%s\n",name[i]);


            
        }
   }
  //  while(--i>0)
    //printf("%s\n",name[i]);

}







void  cd_command() {

    char str1[10];
    char path[PATH_SIZE];
    int res;
    memset(path, '\0', PATH_SIZE);                                         //初始化ｐａｔｈ数组

    while(~ scanf("%s", str1)) {
        if (!strcmp(str1,"exit")) break;
        scanf(" %s", path);
        if (!strcmp(str1, "cd")) {
             res =  chdir(path);
            if(res != 0)
               printf("%s is not a path,please check again\n", path);
         }
        else  printf("%s is not a command,please check\n", str1);

    getcwd(buf, sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里 
    if(strlen(buf) < home_length) 
    printf("\033[34m%s@%s:%s\n\033[0m",pwd->pw_name,hostname,buf);
    else printf("\033[33m%s@%s:~%s\n\033[0m", pwd->pw_name, hostname, buf + home_length);
         
   }
}

void get_password() {
    
   struct passwd *qwe;
    qwe = getpwuid(getuid());
    printf("%s\n", qwe->pw_passwd);                                       //获取密码
}


void get_username_hostname_path() {  

    gethostname(hostname, sizeof(hostname));                             //获取主机名
    getcwd(buf, sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里 
    if(strlen(buf) < home_length) 
    printf("\033[31m%s@%s:%s\n\033[0m", pwd->pw_name, hostname, buf);
    else printf("\033[31m%s@%s:~%s\n\033[0m", pwd->pw_name, hostname, buf + home_length);
}

int main(){
    pwd = getpwuid(getuid());                                            //先用getuid()获取id号  通过 getwuid()函数通过uid 查看用户的passwd数据  获取用户名
  //home_length = strlen(getcwd(buf1, sizeof(buf1)));
    home_length = strlen(pwd->pw_dir);                                   //在passwd结构体中有有一个pw_dir 成员变量－－>家目录    
    signal(SIGINT, SIG_IGN);
    get_username_hostname_path();
    cd_command();
    char ls[5];
    scanf("%s",ls);
    if(!strcmp(ls,"ls"))
    ls_command();
    else return 0;
    return 0;
}

