#include <iostream>
#include "thread.h"

class CMyTask : public CTask {
public:
    CMyTask(){}     /** constructor */
    int Run() {
        printf("%s\n", static_cast<char*>(this->m_ptrData));
        Sleep(5000);
        return 0;
    }
};


int main() {
    /** �����������֮������CThreadPool::m_vecTaskList�� */
    CMyTask taskObj;

    char szTmp[] = "this is the first thread running!";
    taskObj.SetData(static_cast<void*>(szTmp));

    /** �����̳߳أ�����10���߳� */
    CThreadPool threadPool;

    /** ��20������Ž��̳߳��е�����أ������б��� */
    for (int i = 0; i != 20; ++i)
        threadPool.AddTask(&taskObj);

    while (1) {
        printf("there are still %d tasks need to be handled.\n", threadPool.GetTaskSize());
        if (threadPool.GetTaskSize() == 0) { // ���������û���κ�����֮�������̶߳��ᴦ��pthread_cond_wait״̬������˯��״̬��
            if (threadPool.StopAll() == -1) {   // ��ִ��StopAll()֮�����е��̶߳��ᣨ��˯��״̬�������ѣ�Ȼ������
                printf("Now i will exit from main.\n");
                exit(0);
            }
        }
        Sleep(2000);
    }
    return 0;
}
