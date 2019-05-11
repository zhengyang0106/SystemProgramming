/*************************************************************************
	> File Name: 2-cp.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年05月10日 星期五 13时53分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>

#define BUF_SIZE 1024
#define PATH_LEN 128

void my_err(char *err_string, int line) {
    fprintf(stderr, "line : %d", line);
    perror(err_string);
    exit(1);
}



void copy_data(const int frd, const int fwd) {
    int read_len = 0;
    int write_len = 0;
    unsigned char buf[BUF_SIZE]; //读入字符暂时存放处
    unsigned char *p_buf;
    while ( (read_len = read(frd, buf,BUF_SIZE)) ) {//read() 函数返回读取的字节数
        if ( -1 == read_len ) {
            my_err("读取错误", __LINE__);
        } else if (read_len > 0){
            p_buf = buf;
            while ( (write_len == write(fwd, p_buf, read_len)) ) {
                if(write_len == read_len)
                break;
                else if (write_len == -1) {
                     my_err("写入错误",__LINE__);
                }
                else if (write_len > 0) {
                    p_buf += write_len;
                    read_len -= write_len;
                }
            }
            if(write_len == -1) break;

        }

    }
}

void cp_1(int argc, char *argv[]) {

 int frd , fwd; //文件读写描述符
    int len = 0;
    char *pwd_y , *pwd_m, *p;   // 源文件  和  目的文件路径
    struct stat stat_y, stat_m;   // 文件信息结构体
    if(argc < 3) {
        my_err("请确定 参数个数", __LINE__);   // __LINE__系统宏定义参数 显示所在行数
    }
    //打开文件
    int file_num = argc-1;
    int i = 0;
while(++i < file_num) {

    frd = open(argv[i],O_RDONLY);//不成功返回-1  成功返回一个最小的没出现过数字
    if (frd == -1) {
        my_err("不能打开文" ,__LINE__);
    }
    //获取文件详情
    if(fstat(frd, &stat_y) == -1) {
        my_err("不能获取文件详情",__LINE__);

    }
    //检查源文件路径是否是目录
    if (S_ISDIR(stat_y.st_mode)) {// 暂时没成功
      //  cp_2(3,argv);
        my_err("略过目录",__LINE__);

    }

    //处理目标文件
    pwd_m = argv[file_num];
    stat(argv[file_num],&stat_m);
    
    //如果目标路径是 目录则使用源文件的名字
    if(S_ISDIR(stat_m.st_mode)) {
        len = strlen(argv[i]);
        pwd_y = argv[i] + (len -1); //指向最后一个字符
    
        while (pwd_y >= argv[i] && *pwd_y != '/') {
            pwd_y--;  //指向 / 位置
        }
        pwd_y++; // 指向文件名开始

//printf("%s\n",pwd_y);
//printf("%s\n",argv[2]);
        len = strlen(argv[file_num]);
        if(!(strcmp(argv[file_num], pwd_y))) 
           my_err("chonfu",__LINE__);
        if (len == 1 && *(argv[file_num]) == '.') {        
           // len = 0;
           // pwd_m = pwd_y;
        my_err("在当前目录下",__LINE__);

        } else {//复制到 目的文件夹下 使用源文件名
                pwd_m = (char *)malloc(sizeof(char)*PATH_LEN);
                if (pwd_m == NULL) {
                    my_err("malloc error",__LINE__);
                }
                strcpy(pwd_m, argv[file_num]);
                if(*(pwd_m + (len -1)) != '/') {
                    strcat(pwd_m, "/"); // strcat() 函数拼接的是两个字符串  即使是单个字符也用“ ”追加
                }
                strcat(pwd_m + len, pwd_y);  

        }

        
    } else if(!strcmp(argv[file_num], argv[i])){
        my_err("CP参数为同一文件",__LINE__);
    }
    //打开目标文件， 权限与 源文件一样
    fwd = open(pwd_m,O_WRONLY | O_CREAT | O_TRUNC,stat_y.st_mode);
    //O_CREAT 代表不存在就创建一个 第三个参数的权限文件
    //O_TRUNC 若文件存在长度被截为0  属性不变 
    if(fwd == -1) {
        my_err("不能创建新文件", __LINE__);

    }
    copy_data(frd, fwd);
    if (len > 0 && pwd_m != NULL)
        free(pwd_m);
    close(frd);
    close(fwd);

}    
   
}
/*
void cp_2(int argc, char *argv[]) {
DIR *dp = NULL;
    struct dirent *dirp;
    char MULU[PATH_LEN][PATH_LEN];
    int i = 1;
    if((dp = opendir(argv[1])) == NULL)
        my_err("目录打开失败",__LINE__);
    else {
        while((dirp = readdir(dp)) != NULL) {
            if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
            continue;
            strcpy(MULU[i++], dirp->d_name);
          
        }
    }


int k= i;
int j=i;
int l= i;
    char MULU1[PATH_LEN][PATH_LEN];
    while(--j) strcpy(MULU1[j], argv[1]);
    while(--l) strcat(MULU1[l], "/");
    while(--k) strcat(MULU1[k], MULU[k]);

        cp_1(i,&MULU1[1]);
*/
 /*  printf("%d\n",i);
    while(--i){
       // strcat(MULU[i],"***");
        printf("%s\n",MULU1[i]);
    }*/
//}


/*
void cp_2(int argc, char*argv[]) {
    DIR *dp;
    struct dirent *entry;
    char srcinside[500];
    char desinside[500];
    if((dp = opendir(argv[1])) == NULL) {
        my_err("打开文件出错",__LINE__);

    }
    while((entry = readdir(dp)) != NULL) {
        if(!(strcmp(entry->d_name, ".")) || !(strcmp(entry->d_name, "..")))
            continue;
        strcpy(srcinside, argv[1]);
        strcpy(desinside, argv[2]);
        strcat(srcinside, "/");
        strcat(desinside, "/");
        strcat(srcinside, entry->d_name);
        strcat(desinside, entry->d_name);
        int a[3][PATH_LEN];
        strcpy(a[1], srcinside);
        strcpy(a[2], desinside);
        cp_1(3,a);
       

    }
    
}

*/



int main(int argc, char *argv[]) {
   cp_1(argc, argv);   
// cp_2(argc, argv); //  文件夹 --> 文件夹
return 0;

}
