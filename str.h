#ifndef __STR_H__
#define __STR_H__

char* strCreate(int size);
void strFill(char* str, int size, char ch);
int strLen(const char* str);
void strCopy(char* dst, const char* src);
char* strCat(const char* str1, const char* str2);

#endif