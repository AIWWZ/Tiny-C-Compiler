#include "toyc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******** 定义全局变量 ********/
Vector Token_Table;		//单词表
HashTable Hash_Table;	//哈希表
String Str;				//字符串
Token token;			//单词
char ch;

/* 读字符 */
void NextChar() {
	ch = fgetc(FP);
}

/* 带过滤的读字符 */
void NextAvailChar() {
	NextChar();
	while (1) {
		if (Is_Layout(ch)) {
			NextChar();
		}
		else if (ch == '/') {
			NextChar(); //多读一个判断是不是注释
			if (ch == '*') {
				RecognizeNote();
			}
			else { //回写
				if (ungetc(ch, FP) == EOF) {
					Error("C_error:回写失败!");
				}
				ch = '/';
				break;
			}
		}
		else
			break;
	}
}

/* lex初始化 */
void InitLex() {
	Token_Table = InitVector(TKSIZE);
	Hash_Table = InitHash(HASHSIZE);
	Str = InitString(STRSIZE);
	/*先将部分单词插入单词表*/
	struct TokenWord TkWords[] = {
		{ BREAK,	"break",	NULL },
		{ CHAR,		"char",		NULL },
		{ CASE,		"case",		NULL },
		{ CONTINUE,	"continue", NULL },
		{ DEFAULT,	"default",	NULL },
		{ DOUBLE,	"double",	NULL },
		{ ELSE,		"else",		NULL },
		{ FOR,		"for",		NULL },
		{ IF,		"if",		NULL },
		{ INCLUDE,	"#include",	NULL },
		{ INT,		"int",		NULL },
		{ RETURN,	"return",	NULL },
		{ VOID,		"void",		NULL },
		{ STRING,	"string",	NULL },
		{ SWITCH,	"switch",	NULL },
		{ WHILE,    "while",	NULL },
		{ PLUS,		"+",		NULL },
		{ MINUS,	"-",		NULL },
		{ MULTI,	"*",		NULL },
		{ DIVIDE,	"/",		NULL },
		{ MOD,		"%",		NULL },
		{ ASSIGN,	"=",		NULL },
		{ EQ,		"==",		NULL },
		{ NEQ,		"!=",		NULL },
		{ LT,		"<",		NULL },
		{ GT,		">",		NULL },
		{ LE,		"<=",		NULL },
		{ GE,		">=",		NULL },
		{ OR,		"||",		NULL },
		{ AND,		"&&",		NULL },
		{ L_PAREN,	"(",		NULL },
		{ R_PAREN,	")",		NULL },
		{ L_BRACK,	"[",		NULL },
		{ R_BRACK,	"]",		NULL },
		{ L_BRACE,	"{",		NULL },
		{ R_BRACE,	"}",		NULL },
		{ COMMA,	",",		NULL },
		{ COLON,	":",		NULL },
		{ SEMI,		";",		NULL },
		{ C_INT,	"c_int",	NULL },
		{ C_DOUBLE,	"c_double",	NULL },
		{ C_CHAR,	"c_char",	NULL },
		{ C_STR,	"c_string",	NULL },
		{ C_HEADER,	"c_header",	NULL },
		{ _EOF,		"EOF",		NULL },
		{ FUNC,		"func",		NULL },
		{ 0,		0,			0 }
	};
	int i;
	Token Tk;
	for (i = 0; TkWords[i].String != 0; i++) {
		Tk = malloc(sizeof(struct TokenWord));
		if (Tk == NULL) {
			FatalError("Out of space!");
		}
		Tk->String = malloc(sizeof(char) * strlen(TkWords[i].String) + 1);
		if (Tk->String == NULL) {
			FatalError("Out of space!");
		}
		strcpy(Tk->String, TkWords[i].String);
		Tk->TkCode = TkWords[i].TkCode;
		Tk->Next = Hash_Table->TheTokens[Hash(Tk->String, Hash_Table)];
		Hash_Table->TheTokens[Hash(Tk->String, Hash_Table)] = Tk;
		Token_Table->Data[Token_Table->Size++] = Tk;
	}
}

/* 解析#include */
void RecognizeInclude() {
	StringReset(Str);
	while (ch == '#' || IsLetter(ch)) {
		StringAdd(ch);
		NextChar();
	}
	if (ungetc(ch, FP) == EOF) { /*回写*/
		Error("C_error:回写失败!");
	}
	token = VectorAdd(Str);
}

/* 解析标识符 */
void RecognizeIden() {
	StringReset(Str);
	while (IsLetter(ch) || IsDigit(ch) || IsUnderline(ch)) {
		StringAdd(ch);
		NextChar();
	}
	if (ungetc(ch, FP) == EOF) { /*回写*/
		Error("C_error:回写失败!");
	}
	token = VectorAdd(Str);
}

/* 解析数字常量--整数，实数 */
void RecognizeNum() {
	StringReset(Str);
	int cnt = 0;
	while (IsDigit(ch) || IsDot(ch)) {
		if (IsDot(ch)) {
			cnt++;
		}
		StringAdd(ch);
		NextChar();
	}
	if (ungetc(ch, FP) == EOF) { /*回写*/
		Error("C_error:回写失败!");
	}
	if (cnt == 0) { //整型常量
		token = Find("c_int");
		
	}
	else if (cnt == 1) { //浮点型常量
		token = Find("c_double");
	}
	else {
		Error("C_error:词法错误!");
	}
}

/* 解析常量--字符,字符串,头文件*/
void RecognizeConst(){
	StringReset(Str);
	char c = ch;
	if (ch == '<') {
		 c = '>';
	}
	while (TRUE) {
		NextChar();
		if (ch == c) {
			break;
		}
		else {
			StringAdd(ch);
		}
	}
	if (c == '\'') {
		token = Find("c_char"); /*字符*/
	}
	else if(c == '\"'){
		token = Find("c_string"); /*字符串*/
	}
	else {
		token = Find("c_header"); /*头文件*/
	}
}


/* 解析注释 */
void RecognizeNote() {
	while (1) {
		NextChar();
		if (ch == '*') {
			NextChar();
			if (ch == '/') {
				NextChar();
				break;
			}
		}
	}
}

/* 获取token */
void NextToken(){
	NextAvailChar();
	switch (ch)
	{
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_':
	{
		RecognizeIden(); /*标识符*/
		break;
	}
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
	case '8': case '9':
		RecognizeNum(); /*数字常量*/
		break;
	case '#':
		RecognizeInclude(); /*#include*/
		break;
	case '\'':
	case '\"':
		RecognizeConst(); /*字符，字符串常量*/
		break;
	case '+':
		token = Find("+");
		break;
	case '-':
		token = Find("-");
		break;
	case '*':
		token = Find("*");
		break;
	case '/':
		token = Find("/");
		break;
	case '%':
		token = Find("%");
		break;
	case '=':
		NextChar();
		if (ch == '=') {
			token = Find("==");
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*回写*/
				Error("Write back error!");
			}
			token = Find("=");
		}
		break;
	case '!':
		NextChar();
		if (ch == '=') {
			token = Find("!=");
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*回写*/
				Error("Write back error!");
			}
		}
		break;
	case '<':
		if (token->TkCode != INCLUDE){
			NextChar();
			if (ch == '=') {
				token = Find("<=");
			}
			else {
				if (ungetc(ch, FP) == EOF) { /*回写*/
					Error("Write back error!");
				}
				token = Find("<");
			}
			break;
		}
		else {
			RecognizeConst();
			break;
		}
	case '>':
		NextChar();
		if (ch == '='){
			token = Find(">=");
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*回写*/
				Error("Write back error!");
			}
			token = Find(">");
		}
		break;
	case '&':
		NextChar();
		if (ch == '&') {
			token = Find("&&");
		}
		else {
			Error("C_error:词法错误!");
		}
		break;
	case '(':
		token = Find("(");
		break;
	case '[':
		token = Find("[");
		break;
	case '{':
		token = Find("{");
		break;
	case ')':
		token = Find(")");
		break;
	case ']':
		token = Find("]");
		break;
	case '}':
		token = Find("}");
		break;
	case ',':
		token = Find(",");
		break;
	case ':':
		token = Find(":");
		break;
	case ';':
		token = Find(";");
		break;
	case EOF:
		token = Find("EOF");
		break;
	default:
		printf("%c  ", ch);
		Error("C_error:无法识别的字符!");
		break;
	}

	printf("%s   ", token->String);
}