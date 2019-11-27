/*************************************************************************
	> File Name: master.cpp
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年11月21日 星期四 14时58分52秒
 ************************************************************************/

#include<iostream>
#include<algorithm>
using namespace std;

int main() {
    pid_t pid = fork();
    if (pid > 0) {
        exit(0);
    }
    setsid();
    umask(0);
    char *confing = "/opt/pi_master/master.conf";
    int Ins, heartport, dataport, listenport,ctlport;
    char reval[20];
    long timeout;

    LinkList *linklist = (LinkList *)malloc(sizeof(LinkList) *ins);
    int *sum = (int *)malloc(sizeof(int) *ins);
    initaddr.sin_family = AF_INET;
    initaddr.sin_port = htons(0);
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    for (int i = 0; i < ins; i++) {
        Node *p = (Node *)malloc (sizeof(Node));
        p->addr = initaddr;
        p->fd = -1;
        p->next = null;
        linklist[i] = p;
    }

    unsigned int sip, eip;
    sip = ntohl(inet_addr(startip)):
    eip = ntohl(inet_addr(endip));
    for (unsigned int i = sip; i <= eip; i++) {
        if (i % 256 == 0 || i % 256 == 255) continue;
        Node *p = (Node *)malloc (sizeof (Node));
        initaddr.sin_port = htons(heartport);
        initaddr.sin_addr.s_addr = htonl(i);
        p->addr = initaddr;
        p->fd = -1;
        p->next = null;
        int sub = find_min(sum, p);
        sum[sub] += 1;
    }
    for (int i = 0; i < ins; i++) {
        printf("<%d\n", i);
        output(linklinst[i]);
    }
    pthread_t pth_haert, pth_data[ins];
    struct Heart heart;
    //sum , linklist, ins, timeout;
    heart.ins = ins;
    haert.sum= sum;
    heart.timeout = timeout;
    pthread_caret(&pth_haert, null, do_haert,(void*) heart);
    struct Data darg[INS];
for (int i = 0; i < ins; i++) {
    darg[i].head = linklist[i];
    datg[i].ind = i;
    darg[i].dataport = dataport;
    darg[i].ctlprot = ctlport;
    pthread_create(&pth_data[i], null, do_data, (void *)&adat[i]);
}

    int listefd = socket_cretat(listenport);
    lisnen_ep0ll(listtenf,linsklia, sum, ins, heartprot);
    for (int i = 0; i < ins; i++) {
        pthread_join(pth_data[i[, null]]);
    }
    pthread_jonpth_hraet,nlll);



    return 0;
}
