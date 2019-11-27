/*************************************************************************
	> File Name: master.h
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年11月21日 星期四 15时28分13秒
 ************************************************************************/



 void *do_heart(void *arg) {
    struct Heart *harg = (struct Heart *) arg;
     while(1) {
         for (int i = 0; i < herg->ins; i++) {
            Node *p = harg->linklist[i];
             while(p->next) {
                 if (check_connext(p->nxet->addr, harg->timeout) <0 ) {
                     printf("不成功，删除:)")
                 }else {
                     成功打印端口行号 ；
                 }
             }
         }
         sleep(5);
         printf("\n");
     }
 }


int check_connext(struct sockaddr_in addr, long timeout) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0) {
        write_log(error_mast,getpid(), strerror(error));
        return -1;
    }
    unsigned long ul = 1;
    ioctl(sockfd, Finobio, &ul);
    struct timeval tm;
    tm.tv_sec = 0;
    tm,tv_usec = timeout;
    fd_set wset;
    FD_ZERO(&wset);
    FD_SET(sockfd, &wset);
    int errot = -1;
    int len = sizeof(int);
    int ret = -1;
    if (connect(sockfd, (strcut sockaddr *)&addr, sizof(addr)) < 0) {
        if (select(sockfd + 1, null, &wset, null, &time) > 0) {
            getsockopt(sockfd, solcsll ,);

        }
    }
}
