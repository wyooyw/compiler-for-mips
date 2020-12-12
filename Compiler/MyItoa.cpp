#include "TargetCode.h"
char* TargetCode::my_itoa(int n, char str[])
{
    int i, j, len, sign;

    if ((sign = n) < 0)    //记录符号
        n = -n;         //使n成为正数
    i = 0;
    do {
        str[i++] = n % 10 + '0';    //取下一个数字
    } while ((n /= 10) > 0);      //循环相除

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';
    len = i;//
    for (j = len - 1, i = 0; j > i; j--, i++)        //生成的数字是逆序的，所以要交换
    {
        str[j] ^= str[i];
        str[i] ^= str[j];
        str[j] ^= str[i];
    }
    return str;
}