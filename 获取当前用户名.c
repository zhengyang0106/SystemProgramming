/*************************************************************************
	> File Name: 获取当前用户名.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年04月15日 星期一 17时49分45秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pwd.h>

int main(){
    char hostname[60] = {'\0'};
    struct passwd *pwd;
    char buf[80];//存放路径
    gethostname(hostname, sizeof(hostname));//获取主机名
    pwd = getpwuid(getuid());//先用getuid()获取id号  通过 getwuid()函数通过uid 查看用户的passwd数据  获取用户名
    getcwd(buf,sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里
    
    printf("\033[32m %s@%s:%s\n\033[0m",pwd->pw_name,hostname,buf);

   /* struct passwd *qwe;
    qwe = getpwuid(getuid());
    printf("%s\n",qwe->pw_passwd);   //获取密码
    */




    return 0;
}
