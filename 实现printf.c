/*************************************************************************
	> File Name: 实现printf.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年05月01日 星期三 16时50分40秒
 ************************************************************************/

#include<stdio.h>
#include<stdarg.h>
#include<inttypes.h>
#include<math.h>

int output_dnum(int x) {
    int temp = 0;
    int ret = 0;
    int digit = 0;
// printf("x= %d\n",x);
    do {//翻转x的顺序 并 存给temp
        temp = temp * 10 + x % 10;
        digit++;
        x /= 10;
    } while(x);
     ret = digit;
    while (digit--) {
        putchar(temp % 10 ^ 48);                        // ^48  48 是 字符0  ，这样可以把一个数字换成 字符  
                                                        //  a ^ b = c   a ^ c = b       切记  切记
        temp /= 10;
    }
    return ret;
    
}


int output_d(int x) {
    #define BASE 100000;
    #define BASE_D 5
    int ret = 0;
    unsigned int temp = (x >= 0 ? x : -x);
    if (x < 0) putchar('-'),ret += 1;
    int digit = ceil(log10(temp));
    int p1 = temp / BASE;
    int  p2 = temp % BASE;
    if(digit <= BASE_D) {
        ret += output_dnum(p2);
    } else {
        ret += output_dnum(p1);
        ret += output_dnum(p2);
    }
    #undef BASE
    #undef BASE_D
    return ret;
}



int my_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    int ret = 0;
    while (str[0]) {
        switch (str[0]) {
            case '%': {
                str++;
                switch (str[0]) {
                    case 'd': {
                        int x = va_arg(args,int);
                        ret += output_d(x);
                    }break;
                }

            }break;
            default : putchar(str[0]),ret++;break;

        }
        str++;
    }
    return ret;
}

int main() {
    printf("hello world : %d\n", -5);
    my_printf("hello world : %d\n", 10000);
    printf("hello world : %d\n", 10000);
    my_printf("hello world : %d\n", 0);
    printf("hello world : %d\n", 0);
    my_printf("hello world : %d\n", INT32_MIN);
    printf("hello world : %d\n", INT32_MIN);
    my_printf("hello world : %d\n", 1999999999);
    printf("hello world : %d\n", 1999999999);
    int n;
    scanf("%d", &n);
    my_printf(" has %d digits\n", my_printf("%d", n));
    printf(" has %d digits\n", printf("%d", n));
    return 0;
}

