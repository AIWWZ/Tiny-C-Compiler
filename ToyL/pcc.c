#include <stdio.h>
#include "pcc.h"
FILE *FP;

int main(int argc, char *argv[]) {
	FP = fopen(argv[1], "r");
	if (!FP) {
		Error("Open file fail!");
	}

	InitLex();
	/*
	NextToken();
	while (token->TkCode != _EOF) {
		NextToken();
	}
	*/
	Parser();
	printf("�﷨�����ɹ�!\n");
	return 0;
}