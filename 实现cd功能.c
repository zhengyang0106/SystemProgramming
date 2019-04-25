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
#define PATH_SIZE 10

int main(){
    char hostname[60] = {'\0'};
    struct passwd *pwd;
    char buf[80];//存放路径
    gethostname(hostname, sizeof(hostname));//获取主机名
    pwd = getpwuid(getuid());//先用getuid()获取id号  通过 getwuid()函数通过uid 查看用户的passwd数据  获取用户名
    getcwd(buf,sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里
    
    printf("\033[31m%s@%s:%s\n\033[0m",pwd->pw_name,hostname,buf);
    
   /* struct passwd *qwe;
    qwe = getpwuid(getuid());
    printf("%s\n",qwe->pw_passwd);   //获取密码
    */

    char str1[2];
    char path[PATH_SIZE];
    int res;
    memset(path,'\0',PATH_SIZE);//初始化ｐａｔｈ数组


     scanf("%s %s", str1, path);
    if (!strcmp(str1, "cd")) {
      res =  chdir(path);
      if(res != 0)
       printf("%s is not a path,please check again\n",path);
    }
    else printf("%s is not a command,please check\n",str1);

      getcwd(buf,sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里
      printf("\033[33m%s@%s:%s\n\033[0m",pwd->pw_name,hostname,buf);

    return 0;
}

