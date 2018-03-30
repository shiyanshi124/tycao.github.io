#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
using namespace std;

char* strcpy_s(char*, const char*);


int main(int argc, char *argv[])
{
   char* a;
   char* b = "abcdefgh";
    cout << strcpy_s(a, b) << endl;
    return 0;
}


char* strcpy_s(char* dst, const char* src) {
    // �жϲ�����Ϊ��ָ��
    assert(dst != nullptr);
    assert(src != nullptr);

    char* strTmp = new char[strlen(src) + 1];
    char* pTmp = strTmp;    // �����׵�ַ

    // ��src�����ݿ�������ʱָ�������
    while ((*strTmp++ = *src++) != '\0');
    strTmp = pTmp;  // �ָ������

    char* pdst = dst;
    // ���Ƶ�Ŀ���ַ���
    while ((*pdst++ = *pTmp++) != '\0');
    delete [] strTmp;
    return dst;
}
