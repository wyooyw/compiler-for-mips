#include "TargetCode.h"
char* TargetCode::my_itoa(int n, char str[])
{
    int i, j, len, sign;

    if ((sign = n) < 0)    //��¼����
        n = -n;         //ʹn��Ϊ����
    i = 0;
    do {
        str[i++] = n % 10 + '0';    //ȡ��һ������
    } while ((n /= 10) > 0);      //ѭ�����

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';
    len = i;//
    for (j = len - 1, i = 0; j > i; j--, i++)        //���ɵ�����������ģ�����Ҫ����
    {
        str[j] ^= str[i];
        str[i] ^= str[j];
        str[j] ^= str[i];
    }
    return str;
}