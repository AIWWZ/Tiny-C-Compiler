#include <stdio.h>

#include "pcc.h"
FILE *FP;
int a = 0, b = 1;

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
	printf("\n�﷨�����ɹ�!\n");
	return 0;
}