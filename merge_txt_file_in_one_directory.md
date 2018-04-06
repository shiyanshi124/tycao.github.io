��ָ����Ŀ¼�µ�����`.txt`�ļ��ϲ���ͬĿ¼�µ�һ��.txt�ļ��ڣ�������ֱ�Ӵ���һ��merge.txt�ļ�����������.txt�ļ����ݺϲ��ڴ��ļ��ڣ�
==============
#### ʱ��: 2018-04-06
#### �����������ڼ���û�лؼң�Ȼ��ͺ����ġ���������Ȧ������ѧ������ʦ��Javaʵ���˴˹��ܣ�����ͼ����������Ȧ����֮ǰ��C++�����ļ���صĳ���Ҳд�˲��٣�����`C++ Primer 5th.`Ҳ��`�ھ���Page390`��`�ڰ���Page290`��ϸ�ĸ����˺ü��������ı�����ľ������ӡ������Һ���ʵ��һ��ͬ�����ܵ�C++�汾����ʱ����Ӧ���߼��ϲ��ѡ����Ǵ����ַ������ļ���

Talk is cheap, show me the code! �ã��������ϴ��룺
### merge_txt.h
```cpp
#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <io.h>
using namespace std;
class MergeTxt{
public:
    MergeTxt(const string& path) : filepath(path) {  }
    ~MergeTxt() {}
    void merge();
    void listFiles(const char*);
private:
    const string filepath;
    std::vector<string> strVec; // store all file names in the dir
    ofstream out;
    ifstream in;
};
inline void MergeTxt::merge() {
    listFiles(filepath.c_str());
    out.open(filepath.substr(0, filepath.find_first_of("\\") + 1) + "merge.txt");
    if (!out)
        cout << "out not open!!!\n";
    for (auto& f : strVec) {
        if (f.find(".txt") == string::npos) // only file names in ".txt" format can be handled
            continue;
        string ff = filepath.substr(0, filepath.find_first_of("\\") + 1) + f;
        string line;
        in.open(ff);
        if (!in.is_open())
            continue;
        else {
            out << "-----------------------" << ff << "-------------------\n";
            while (getline(in, line)) {
                out << line << "\n";
            }
            in.close(); // close file handle, and will open next file
        }
    }
}
inline void MergeTxt::listFiles(const char * dir) {
    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dir, &findData);    // ����Ŀ¼�еĵ�һ���ļ�
    if (handle == -1) {
        cout << "Failed to find first file!\n";
        return;
    }
    do {
        if (findData.attrib & _A_SUBDIR
            && strcmp(findData.name, ".") == 0
            && strcmp(findData.name, "..") == 0
            )    // �Ƿ�����Ŀ¼���Ҳ�Ϊ"."��".."
            cout << findData.name << "\t<dir>\n";
        else
            //cout << findData.name << "\t" << findData.size << endl;
            strVec.push_back(findData.name);
    } while (_findnext(handle, &findData) == 0);    // ����Ŀ¼�е���һ���ļ�
    cout << "Done!\n";
    _findclose(handle);    // �ر��������
}
```

### main.cpp
```cpp
#include "merger_txt.h"

int main()
{
    MergeTxt m("d:\\*.*");
    m.merge();

    return 0;
}
```

### �������к�Ľ�ͼ��<br />
!["result"](https://github.com/tycao/tycao.github.io/blob/master/merge_txt_src/result.png "result")<br />

### ���Ǵ����ͼ��<br />
!["code"](https://github.com/tycao/tycao.github.io/blob/master/merge_txt_src/merge_done2.png "code")<br />

�������ͬһ·��������`merge.txt`���ļ�����Ŀ¼�µ�����������`.txt`�ļ�������ȫ�����ϲ����˴������ɵ��ļ����ˡ�

### �ܽ᣺
����Ҳ�Ǿ�������һ���ܼ򵥵Ĺ��ܣ������ڴ���ʵ�ֵĹ����У���ȻҲ�ȿ��ˣ��Ǿ���·�����⡣һ��ʼһֱ����û��`merge.txt`���ɣ��������ð죬�ǿ϶���ofstream out��ִ��out.open(filename)ʱʧ���ˡ�������������µ�������֣�**merge.txt�ļ���һֱû���κκϲ�֮������ݡ�**
Ҳû�б�������debug�����е�ʱ��`while (getline(in, line))`�� ��һ��ֱ��������û��ȥִ�С������Ҽ����жϣ�`if (!in.is_open()) cout << "file not opened!\n"`�����ʱ��ŷ���ԭ�����ļ���ifstreamû�д��ļ����ҵ�����ԭ�����Ϊû��������һ���ʱ�䣩��������Ϊʲôû���ļ���Ȼ��ŷ���ԭ����**û�жѸ��ļ����Ͼ���·����**��ƾ���Ͼ���·����һ�ж������ˡ�




