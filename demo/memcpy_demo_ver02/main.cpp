#include <cstdio>
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

// �ڴ濽��
// -----�ڶѣ���̬�ڴ棩�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
void* Mymemcpy(void* ,const void* ,size_t );
// ------��ջ�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
void * Mymemcpy02(void*, const void*, size_t);


// �ַ�������
// -----�ڶѣ���̬�ڴ棩�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
char* Mystrcpy(char*, const char*);
// ------��ջ�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
char* Mystrcpy02(char*, const char*);

int main(int argc, char *argv[])
{
    // for Mymemcpy
    char str[]="0123456789";
    printf("char str[] = %s\n", str);
    Mymemcpy(str+1,str+0,9);
    printf("Mymemcpy(str+1,str+0,9) :\nstr = %s\n",str);
    Mymemcpy(str,str+6,4);
    printf("Mymemcpy(str,str+6,4) :\nstr = %s\n",str);

    printf("%s\n", "--------------------------");

    // for Mymemcpy02
    char* c = const_cast<char*>(string("").c_str());
    const char* d = "1234567890";
    printf("Mymemcpy02(c, d) : %s\n", Mymemcpy02(c, d, 11));
    printf("c = %s\n", c);
    printf("d = %s\n", d);

    printf("%s\n", "----------------------------");

    // for Mymemcpy02
    char strr[]="0123456789";
    Mymemcpy02(strr+1,strr+0,9);
    printf("%s\n",strr);
    Mymemcpy02(strr,strr+6,4);
    printf("%s\n",strr);

    printf("%s\n","-----------------------------------------");

    // for Mystrcpy
    char* a = const_cast<char*>(string("12").c_str());
    const char* b = "abcdefgh";
    printf("Mystrcpy(a, b) : %s\n", Mystrcpy(a, b));
    printf("a = %s\n", a);
    printf("b = %s\n", b);

    printf("%s\n", "----------------------------");

    // for Mystrcpy02
    char* aa = const_cast<char*>(string("").c_str());
    const char* bb = "abcdefghi";
    printf("Mystrcpy02(aa, bb) : %s\n", Mystrcpy02(aa, bb));
    printf("aa = %s\n", aa);
    printf("bb = %s\n", bb);

    return 0;
}

// �ڴ濽��
// -----�ڶѣ���̬�ڴ棩�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
char* Mystrcpy(char* dst, const char* src) {
    // �жϲ�����Ϊ��ָ��
    assert(dst != nullptr);
    assert(src != nullptr);
    char* pdst = dst;
    const char* psrc = src;

    char* strTmp = new char[strlen(src) + 1];
    char* pTmp = strTmp;    // �����׵�ַ

    // ��src�����ݿ�������ʱָ�������
    while ((*strTmp++ = *psrc++) != '\0');
    strTmp = pTmp;  // �ָ������

    // ���Ƶ�Ŀ���ַ���
    while ((*pdst++ = *strTmp++) != '\0');
    delete [] pTmp;
    return dst;
}

char* Mystrcpy02(char* dest, const char* src) {
    // Ԥ�����ڼ� ����������β�Ϊ0
    assert(dest != nullptr && src != nullptr);
    char* pdest = dest;
    const char* psrc = src;

    char strTmp[strlen(src) + 1] = {0,};
    char* pTmp = strTmp;
    char* ppTmp = pTmp;
    while ((*pTmp++ = *psrc++) != '\0') ;

    while ((*pdest++ = *ppTmp++) != '\0') ;

    return dest;
}


// �ַ�������
// -----�ڶѣ���̬�ڴ棩�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
void* Mymemcpy(void* dest,const void* src,size_t count)
{
      assert(dest != nullptr && src != nullptr && count > 0);
      char* pdest=static_cast<char*>(dest);//��ȫת��
      const char* psrc=static_cast<const char*>(src);

      int countTmp = count;
      char* strTmp = new char[strlen(psrc) + 1];
      char* pTmp = strTmp;  // �����׵�ַ
       while (count--)
           *strTmp++ = *psrc++;
       strTmp = pTmp;
       while (countTmp--)
           *pdest++ = *strTmp++;
       delete [] pTmp;
}

// ------��ջ�������ڴ棬��ʱ���src�����ݣ�Ȼ���ٿ�����dest��ָ���ڴ�������
void * Mymemcpy02(void* dest, const void* src, size_t count) {
    assert(dest != nullptr && src != nullptr && count > 0);

    char* pdest = static_cast<char*>(dest); // ��ȫת��
    const char* psrc = static_cast<const char*>(src);

    int countTmp = count;

    char strTmp[count] = {0,};
    char* pTmp = strTmp;
    char* ppTmp = pTmp;

    while (count--)
        *pTmp++ = *psrc++;
    while (countTmp--)
        *pdest++ = *ppTmp++;
}



