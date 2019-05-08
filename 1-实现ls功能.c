/*************************************************************************
	> File Name: 1-ls.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年05月06日 星期一 22时15分03秒
 ************************************************************************/
   
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<grp.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<math.h>
#define PATH_SIZE 100

char buf[80];

struct stat buf2;

//排序比较方式  qsort固定写法
int comp(const void* a, const void* b) {
    return (strcmp((char* )a, (char* )b));
}

void ls_al(){
    char name[PATH_SIZE][PATH_SIZE];
    char *p;
    DIR *dir;
    struct dirent *dir_info;
    int i = 0;
    struct passwd *pwd;
  //struct stat buf2;
    struct group *grp;
    char type;
    char permission[9];
    memset(permission,'-',9*sizeof(char) );
    memset(name,'\0',PATH_SIZE * sizeof(char) );
    dir = opendir(buf);
    while(NULL != (dir_info = readdir(dir)) )
        strcpy (name[i++],dir_info->d_name);
    closedir(dir);
    //文件排序
    qsort(name,i,sizeof(name[0]) , comp); 
    while(--i >= 0) {
        if(!stat(name[i],&buf2)) {
            //获取文件类型
            if(S_ISLNK(buf2.st_mode))
                type = 'l';
            else if(S_ISREG(buf2.st_mode)) {
                type = '-';
               // ls_w();
               // break;
            }
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
            printf("  %3lu  ",buf2.st_nlink);
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
}
void ls_w(char* test){
    char permission[9];
    memset(permission,'-',9*sizeof(char) );
    char type;
    struct passwd *pwd;
  //struct stat buf2;
    struct group *grp;
    stat(test, &buf2);
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
            printf("  %3lu  ",buf2.st_nlink);
            //文件名组名
            printf("%-4s  %-4s",pwd->pw_name,grp->gr_name);
            //文件大小
            printf("%8ld  ",buf2.st_size);
            //文件时间
            printf("%.12s  ",ctime(&buf2.st_mtime)+4);
            //文件名
            printf("%s\n",test);
            printf("\n");
    
}
/*
int name_max_len(char* name) {
    int j = 0;
    if(strlen(name) > j) j = strlen(name);
    return j;
}*/

/*

void print_row_col(char* name[0], int expect_row, int expect_col, int z, int name_max) {
    int i = 0;
    while(i < z){
        printf("%s  ",name[i]);
        if(!((i + 1) / expect_col)) printf("\n");
    }


}*/




void ls(){
  char name[PATH_SIZE][PATH_SIZE];
    //char *p;
    getcwd(buf,sizeof(buf));
    DIR *dir;
    struct dirent *dir_info;
    int i = 0;
    memset(name,'\0',PATH_SIZE * sizeof(char) );
    dir = opendir(buf);
    while(NULL != (dir_info = readdir(dir)) )
        strcpy (name[i++],dir_info->d_name);
    closedir(dir);
    //文件排序
    qsort(name,i,sizeof(name[0]) , comp); 
    int z = i;
    int p = z;
  unsigned int name_max = 0;
    while(--i >= 0){
     if(strlen(name[i]) > name_max)  
        name_max = strlen(name[i]);
    }
printf("%d\n", name_max);// 名字最宽值

  struct winsize size;  
    if (isatty(STDOUT_FILENO) == 0)  
        exit(1);  
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0) 
    {
        perror("ioctl TIOCGWINSZ error");
        exit(1);
    } 

printf("%d rows, %d columns\n", size.ws_row, size.ws_col); 
    int rows = size.ws_row;                             //屏幕长
printf("%d\n",rows);
    int columns = size.ws_col;//屏幕宽度
    int MIN_COL = 3;  //一个文件名最少占三个位置
    unsigned int col = floor(columns / MIN_COL); //输出的列数
printf("输出列数=%d\n",col);
printf("columns=%d\n",columns);
    while(1){ 
    int name_sum = 0;//宽度和
    int name_2 =0;    
        for(int i = 0; i < col; i++) {
    int name_1 = 0;//每列最大宽度
            for(int j = i; j < p; j += col) {
                if(strlen(name[j]) > name_1 && strlen(name[j])) 
                    name_1 = strlen(name[j]);
 printf("name1 =%d\n",name_1);

            }
            name_sum += name_1;
printf("name_12sum=%d\n",name_sum);
        }
        if(name_sum < columns && name_max < (columns / col)) break;
     //   if(!strlen(name[i]))  break;
        col--;
        
    }

printf("col--= %d",col);

/*
int col_name_max_len[col];   //存储每列 最长的名字宽度
for(int i = 0; i < col; i++){
    int name_len = 0;
    for(int j = i; j < z; j += col) {
        if(strlen(name[j]) > name_len) 
            name_len = strlen(name[j]);
    }
    col_name_max_len[i] = name_len + 2;//使得每列输出都会至少有2空格
}

for(int i = 0; i < z; i++) {
    int j = i + 1;
    if(!(j % col)) printf("\n");
    printf("%-s",name[i]);
    int b = col_name_max_len[i] - strlen(name[i]);
    printf("%d",b);
   //while((col_name_max_len[i] - strlen(name[i]))--)
   // putchar(' ');


}
*/
}












int main(int argc,char* argv[]){

    ls();
  /*  char buf1[80];
    getcwd(buf1,sizeof(buf1));
    int a = argc;
    if(a >1) { 
        while(a > 1) {   
        chdir(argv[--a]);
        getcwd(buf, sizeof(buf));
        if(!strcmp(buf,buf1)) {
            ls_w(argv[a]);
         //  printf("%s\n" ,argv[a]);
        } else {
            printf("%s :\n", argv[a]);
            ls_al();
            printf("\n");
            chdir(buf1);
            } 
        }
    }
    else {
        getcwd(buf,sizeof(buf));
        ls_al();
    }
*/
    return 0;
}
