#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE *input;
	FILE *output;
	char *rstr;
	char *rstr1;
	char str[1024];
	char str1[1024];
	int counter = 1;
	system("call D:\\proga\\База_Данных.exe");
	input = fopen("D:\\proga\\input_test.txt", "r");
	output = fopen("D:\\proga\\output2.txt", "r");
	//system("call D:\\proga\\База_Данных.exe");
	while (!feof(input)|| !feof(output)) {
		rstr = fgets(str, sizeof(str), input);
		rstr1 = fgets(str1, sizeof(str1), output);
		printf("input %s\n", str);
		printf("output %s\n", str1);
		if (strcmp(str, str1) == 0)
			printf("test%d:ok\n", counter);
		else
			printf("test%d:fail\n", counter);
		counter++;
	}
	fclose(input);
	fclose(output);
    return 0;
}

