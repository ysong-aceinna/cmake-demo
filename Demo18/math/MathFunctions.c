
#include<stdio.h>

void imu(char* fileName, unsigned char* data, int len)
{
	FILE* file = fopen("1.csv","wb"); //已只读方式打开
	if (file == NULL) 
    {
		return;
	}
	fwrite(data, 1, len, file); //写入操作
}

int addition_int(int a, int b)
{
    return a+b;
}