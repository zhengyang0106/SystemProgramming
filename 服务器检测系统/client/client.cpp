/*************************************************************************
	> File Name: client.cpp
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年11月20日 星期三 18时22分39秒
 ************************************************************************/

#include<iostream>
#include<algorithm>
using namespace std;




int main() {
    pid_t tpid = fork();
    if (pid > 0 ) {
        exit(0);
    }
    setsid();
    umask(0);
    for (int i = 0; i < NR_OPEN; i++) {
        close(i);
    }

    chdir("/");
    int heartPort, dataPort, loadPort, ctlPort;
    char tem[20] = {0};
    char *config = "/opt/pi_client/client.conf";
    char ip[20] = {0};
    get_conf_value(config, "IP", tmp);
    dataPort = atoi(tmp);


    pthread_mutexattr_t mattr;
    pthread_condattr_cattr;
    pthread_mutexattr_init(&mattr);
    pthread_condattr_init(&cattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    
    struct Share *share = NULL;
    int shmid = shmget(IPC_PRIVATE, sizeof(struct Share), IPC_CREATE | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    share = (struct Share*) shmat(shmid, NULL, 0);
    if (share == (void *)-1) {
        return -1;
    }
    pthread_mutex_init(&share->smutex, &matrr);
    pthread_cond_init(&share->sready, &cattr);
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        return -1;
    } 
    if (pid >0 ) {
    //父进程 用来接受数据
        recv_data(dataPort, ctlPort, share);
        wait(&pid);
    } else {
        pid_t pid_1;
        pid_1 = fork();
        if (pid < 0) {
            return -1;
        }
        if (pid_1 > 0) {
            recv_heart(heartPort, share);
            wait(&pid_1);
            exit(0);
        }else if (pid == 0) {
            pid_t pid_2;
            int inx;
            for (int i = 0; i < 6; i++) {
                inx = i;
                pid_2 = fork();
                if (pid_2 < 0) {
                    return -1;
                }
                if (pid_2 == 0) break;
            }
            if (pid_2 > 0) {
                while(1) {
                    //条件触发
                    pthread_mutex_lock(&share->smutex);
                    pthread_cond_wait(&share->sready, &share->smutex);
                    pthread_mutex_unlock(&share->smutex);
                    do_load(ip, loadPort, share);
                    
                    share->sharecnt = 0;
                    pthread_mutex_unlock(&share->smutex);
                }
                for (int i = 0 ; i < 6; i++) {
                    wait(&pid_2);
                }
                exit(0);

            }
            if (pid_2 == 0) {
                int cnt = 0;
                while (1) {
                    cnt += 1;
                    do_check(inx, share, cnt);
                    if (cnt == 5) cnt = 0;
                    sleep(2);
                }
                exit(0);
            }
        }
    }


    return 0;
}
