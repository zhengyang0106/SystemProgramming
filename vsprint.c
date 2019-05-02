/*
 *  linux/kernel/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (!base) {                                                                //判断进制基数不为0
		base = 10;
		if (*cp == '0') {                                                       //如果第一个字符为0 为8进制   base赋值8
			base = 8;
			cp++; 
			if ((*cp == 'x') && isxdigit(cp[1])) {                              //如果第一个字符为0 第二个为x 且判断cp[1]是16进制字符   base赋值16
				cp++;
				base = 16;
			}
		}
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)     //  判断是不是cp指向 16 进制字符    并且 判断字符满足 自己的进制base  
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;                                           //把字符转换为 base进制的 无符号长整形
		cp++;
	}
	if (endp)                                                                   //使尾指针指向结束地址
		*endp = (char *)cp;
	return result;
}

/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')                                  // 判断是不是数字

static int skip_atoi(const char **s)                                            //把给定字符转换为整型   
{
	int i=0;

	while (is_digit(**s))                                                       // 把普通数字转换成字符数字
		i = i*10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus *
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define do_div(n,base) ({ \                                                     // 除法运算
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })

static char * number(char * str, int num, int base, int size, int precision
	,int type)
{
	char c,sign,tmp[36];
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";                  //定义一个digit指针 并初始化 指向“0-Z”大写字符串
	int i;

	if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";              //如果判断 输出是否为小写字符 ，小写字符串“0-z” 首地址复制给指针
	if (type&LEFT) type &= ~ZEROPAD;                                            //判断 类型是否要求左对齐  要是左对齐就取消 补0
	if (base<2 || base>36)                                                      //判断基数是否在合理范围
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;                                          //判断是否补0  还是 空格
	if (type&SIGN && num<0) {                                                   //判断如果是有符号数字  且 小于 0  就把sign 复制 -
		sign='-';
		num = -num;
	} else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);                      //如果不是有符号数或者 大于 0 就判断是否需要显示 + or 空格 或者 0
	if (sign) size--;                                                           //判断sign 不是 0 时 位数 -1
	if (type&SPECIAL)                                                           //判断 是否是 16进制数字 16进制 -2   因为 16进制输出要 +0x
		if (base==16) size -= 2;
		else if (base==8) size--;                                               //判断是否是八进制   八进制 -1   因为八进制输出 +0
	i=0;
	if (num==0)                                                                 // 判断数字是否为0 为0时直接复制给tmp数组 用于输出   
		tmp[i++]='0';
	else while (num!=0)                                                         // 不为0时 调用 do_div（）函数  从digit数组里判定是哪个字符 
		tmp[i++]=digits[do_div(num,base)];
	if (i>precision) precision=i;                                               // 判断精度 是否小于0；小于的当做0来处理 
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))                                                 //判断是否是左对齐和是否补0
		while(size-->0)                                                         //当不是左对齐和补0时 输出空格补位
			*str++ = ' ';
	if (sign)                                                                   //输出符号位
		*str++ = sign;
	if (type&SPECIAL)                                                           //判断是否是 进制
		if (base==8)                                                            //如果是 8 进制 输出 输出一个0
			*str++ = '0';
		else if (base==16) {                                                    //如果 是16 进制 输出 0x
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type&LEFT))                                                           // 当不是左对齐时 输出 补c                   
		while(size-->0)
			*str++ = c;
	while(i<precision--)                                                        //精度部分补 0
		*str++ = '0';
	while(i-->0)                                                                //输出数字部分
		*str++ = tmp[i];
	while(size-->0)                                                             //剩余部分补充空格                                                     
		*str++ = ' ';
	return str;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	int len;
	int i;
	char * str;
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (str=buf ; *fmt ; ++fmt) {                                              // 把 % 之前的字符存到str数组里
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
		flags = 0;
		repeat:                                                                 // 标签 repeat：  用于  goto  语句的无条件跳转
			++fmt;		/* this also skips first '%' */                         // 判断%后面的字符 
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;                           // - 号   flags 标记 左对齐
				case '+': flags |= PLUS; goto repeat;                           // + 号   flags 标记 输出+
				case ' ': flags |= SPACE; goto repeat;                          // 空格   flags 标记 输出 空格
				case '#': flags |= SPECIAL; goto repeat;                        // # 号   flags 标记 输出 8 / 16 进制
				case '0': flags |= ZEROPAD; goto repeat;                        // 0      flags 标记 输出 0 补位
				}
		
		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))                                                     // 判断是fmt 指向是否是数字
			field_width = skip_atoi(&fmt);                                      // 把数字的整型值    赋值给 field_width 存储输出宽度
		else if (*fmt == '*') {                                                 // 判断如果是 * 字符  下一个整型参数是 输出宽度
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {                                              // 判断如果宽度 < 0 宽度 为 |宽度| 输出时左对齐   
				field_width = -field_width;
				flags |= LEFT; 
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {                                                      // 如果出现  .  进行精度判断
			++fmt;	
			if (is_digit(*fmt))                                                 //   如果小数点后面是数字 就把数字的整型 赋值 给精度
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {                                             // 如果 出现 * 代表 就把 下一个整型参数传给精度
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)                                                  // 如果小数点后面的数字 < 0  就把精度值 赋值为 0
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {                        // 判断是否是 短 / 长整形  符号
			qualifier = *fmt;
			++fmt;
		}

		switch (*fmt) {
		case 'c':                                                               //字符型
			if (!(flags & LEFT))                                                //如果不是左对齐  先输出空格
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);                         //把参数赋值到str数组里面
			while (--field_width > 0)                                           //判断是否符合输出宽度  不符合 在补空格
				*str++ = ' ';
			break;

		case 's':                                                               //字符串型
			s = va_arg(args, char *);                                           //把下一个参数的首地址赋值给s指针
			if (!s)                                                             //判断是否为空
				s = "<NULL>";
			len = strlen(s);                                                    //求字符串长度
			if (precision < 0)                                                  //如果精度 < 0 说明是左对齐 字符长度作为输出字符个数
				precision = len;
			else if (len > precision)                                           //如果字符长度大于输出的精度要求  就输入精度要求的字符个数
				len = precision;

			if (!(flags & LEFT))                                                //如果不是左对齐  并且字符长度小于输出宽度，   补充 （field_width - len） 个空格
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)                                           //把字符串赋值到str数组
				*str++ = *s++;
			while (len < field_width--)                                         //字符长度小于输出宽度，   补充 （field_width - len） 个空格
				*str++ = ' ';
			break;
            
        case 'o':                                                               // 八进制
			str = number(str, va_arg(args, unsigned long), 8,                   //调用number（）函数转换为八进制
				field_width, precision, flags);
			break;

		case 'p':                                                               // 地址型
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,                                                   // 调用number（）函数转换为十六进制
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':                                                               // 16进制
			flags |= SMALL;                                                     //标记为小写字符 
		case 'X':                                                               // 调用的numbe（）函数 转换为 16进制  并且 输出字符为 大写
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':                                                               //  有符号整型
		case 'i':
			flags |= SIGN;
		case 'u':                                                               // 无符号整型
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);                                 //调用number（） 函数 转换为无符号 10进制整型
			break;

		case 'n':                                                               //把 转换的字符个数 存储到指针ip所指的位置
			ip = va_arg(args, int *);
			*ip = (str - buf);
			break;

		default:
			if (*fmt != '%')                                                    // 如果格式转换符不是% 就赋值一个%
				*str++ = '%';
			if (*fmt)                                                           // 如果还没有指到结束继续赋值到str数组 否则 结束
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';                                                                //给str数组一个结束符
	return str-buf;                                                             //返回转换的字符个数


int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}

