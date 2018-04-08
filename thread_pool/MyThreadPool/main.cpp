#include <iostream>
#include "thread.h"



class CMyTask : public csx::CTask {
public:
    CMyTask() {}
    int Run() {
        printf("%s\n", static_cast<char*>(this->m_ptrName));
        Sleep(3000);
        return 0;
    }
};

int main() {
    /** ����������������̳߳��е���������У����߳�ѭ��ȡ����ִ�� */
    CMyTask taskObj;
    char szTmp[] = "this is the first thread running!";
    taskObj.SetData(static_cast<void*>(szTmp));

    /** ����һ���̳߳أ�Ĭ�ϰ���10���߳� */
    csx::CThreadPool threadPool(10);

    /** ��20������Ž��̳߳��е�������� */
    for (int i = 0; i != 20; ++i)
        threadPool.AddTask(&taskObj);

    while (true) {
        printf("there are still %d tasks need to be handled\n", threadPool.GetTaskSize());
        if (threadPool.GetTaskSize() == 0)
            if (threadPool.StopAll() == -1) {
                printf("Now i will exit from main.\n");
                exit(0);
            }
        Sleep(2000);
    }
    return 0;
}
