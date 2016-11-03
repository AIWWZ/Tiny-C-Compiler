#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

//�����洢�ռ�洢����
struct {
	char *iden_name;
	int val;
}Memory[100];
int mp = 0; //��ʾMemory�б����ĸ���

			//���ݱ�������Memory����ö�Ӧval;
int fetch(char *name) {
	int i = 0;
	while (strcmp(Memory[i].iden_name, name) != 0 && i < mp) {
		++i;
	}
	if (i == mp) {
		Error("Variable does not exist.!\n");

	}
	else
		return Memory[i].val;
}
//�ӿ���̨�������
int read() {
	int t;
	scanf("%d", &t);
	return t;
}

//����������Memory��
void updata(char * id, int n) {
	int i = 0;
	while (i < mp && strcmp(Memory[i].iden_name, id))
		++i;
	if (i < mp)
		Memory[i].val = n;
	else {
		Memory[i].iden_name = id;
		Memory[mp].val = n;
		++mp;
	}
}

#endif // !MEMORY_H
