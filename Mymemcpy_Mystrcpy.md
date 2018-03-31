�����Լ����ڴ濽���������Լ��ַ�������������
=======

### Talk is checp, show me the demo!�õģ��������ϴ��룺
```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>

using namespace std;

void* Mymemcpy(void* ,const void* ,size_t );
char* Mystrcpy(char*, const char*);


int main()
{
    // for Mymemcpy
    char str[]="0123456789";
    Mymemcpy(str+1,str+0,9);
    printf("%s\n",str);
    Mymemcpy(str,str+6,4);
    printf("%s\n",str);

    printf("%s\n","-----------------------------------------");

    // for Mystrcpy
    char* a = const_cast<char*>(string("1234567890").c_str());
    char* b = "abcdefgh";
    cout << Mystrcpy(a, b) << endl;
    cout << a << endl;

    return 0;
}

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


```


+++++
###�ܽ᣺
�������ַ��������������ڴ濽������ʵ˼·����һ���ģ�
* [x] ����Ҫ�����ж��Ƿ����Ϊ��ָ�루�Լ�size_t count�Ƿ�С��0�������ǣ�ֱ�ӷ��ش�����Ϣ�������׳��쳣��
* [x] ����������ʱָ�������char* pdest, char* psrc �������������ڴ����򡣣���Ҫֱ������������ϲ�����
* [x] �ڶѣ�Ҳ��Ϊ��̬�ڴ棩������һ���ڴ棬������ʱ���src��ָ�ڴ�����ݡ�Ȼ���ڰ������ʱ�ڴ�����ݿ�����dest��ָ���ڴ��С���������Ϊ�˷�ʽ�����ڴ�����ص�����
* [x] ���Ҫ���ǣ�������Ķ�̬�ڴ��ֶ��ͷš���ֹ�ڴ�й¶��

���ϣ�