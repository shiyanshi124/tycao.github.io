ʹ��C++д�̳߳�
=========
### ������
���̱߳����C++�����ߵ�һ�������������Ǻܶ࿪���߶���ֱ��ʹ�ù�˾��װ�õ��߳̿⣬��û��ȥ�˽����ʵ�֡�����Щ�߳̿��ʵ��Ҳ����Ϊ�߶��Ż�����д�ĺ��Ѷ����ó�ѧ�߿��ĺܳ�����
���Ľ��Լ򵥵ķ�ʽ����ʵ��ԭ���Ӷ��ܹ���ѧϰ���̱߳�̵�C++��ѧ���ܹ�������
### C++��д�̳߳ص�ԭ��
_**���̳߳����ȴ��������������̣߳�������Щ�߳�һֱ���ϵش��̳߳��е������б���ȡ����Ȼ��ִ�У��̴߳��������ȡ����Ĺ����У�����Ҫʵ��ͬ�������Ƶģ������������û������֮�����е��̶߳��ᴦ��`wait״̬`���ֳ�Ϊ`˯��״̬`����**_
֮�󣬻Ὣ���е��߳����ٵ�������֮ǰ����Ҫ��`����˯��״̬�������߳�` **����**��������ͬ���źŻ�������˯�ߣ����ɻ�ģ��̣߳�Ȼ��������١�<br />
:question: ������Ҳ����ʣ�ΪʲôҪ�����ô�鷳�����ÿ���ͻ������µ�����ʱ���Ҿʹ���һ���µ��̲߳������ˣ���Ҳ���Ǹ�����ķ�������Ϊ����ʹ�����д�����������һЩ������ȴ������һ����Ҫ�����⡪�� **����**��
���Ϊÿ���ͻ������󴴽�һ�����̵߳Ļ����Ǻķѵ�CPUʱ����ڴ潫�Ǿ��˵ģ��������һ��ӵ��200���̵߳��̳߳أ��ǽ����Լ�����ĵ�ϵͳ��Դ��ʹ�ø����CPUʱ����ڴ���������ʵ�ʵ���ҵӦ�ã�������Ƶ�����̴߳��������١�
 
### C++�̳߳�
**Talk is cheap, show me the demo!** �ã����������ȷ�����δ����̳߳صĴ��룺
#### thread.h
```cpp
#pragma once

#include <vector>
#include <string>
#include <pthread.h>    // for class pthread_t
#include <windows.h>    // for Sleep(millionseconds)
using namespace std;

/**
 * ִ��������� �������������ݲ�ִ��
*/
class CTask {
public:
    CTask() {}  /** Ĭ�Ϲ��캯�� */
    CTask(string taskName) : m_strTaskName(taskName){ } /** ����һ�������Ĺ��캯�� */
    virtual int Run() = 0;  /** ���麯�� : ˵��������һ��������ࣨ����ֱ�ӳ�ʼ����*/
    void SetData(void*);   /** ������������ */
    ~CTask() {}

protected:
    string m_strTaskName;   /** ��������� */
    void* m_ptrData = nullptr;  /** Ҫִ�е�����ľ�������*/
};

/**
 * �̳߳ع�����
*/
class CThreadPool {
private:
    static          vector<CTask*> m_vecTaskList;    /** �����б� ������أ�*/
    static          bool shutdown;      /** �߳��˳���־*/
    int             m_iThreadNum = 0;       /** �̳߳����������߳��� */
    pthread_t*      pthread_id;

    static pthread_mutex_t m_pthreadMutex;  /** �߳�ͬ���� */
    static pthread_cond_t m_pthreadCond;    /** �߳�ͬ������������ */
protected:
    static void* ThreadFunc(void*);     /** ���̵߳��̻߳ص����� */
    static int MoveToIdle(pthread_t);       /** �߳�ִ�н����󣬰��Լ���������߳��� */
    static int MoveToBusy(pthread_t);       /** ���뵽æµ�߳���ȥ */

    int Create();       /** ���̳߳��д����߳� */
public:
    CThreadPool(int threadNum = 10);    /** Ĭ�����̳߳��д���10���߳� : ÿ���̶߳���ѭ���أ���ͣ�أ��������б���ȡ������Ȼ��ֱ������ */
    int AddTask(CTask*);    /** ��������ӵ���������� */
    int StopAll();      /** ʹ�̳߳��е��߳��˳� */
    int GetTaskSize();  /** ��ȡ��ǰ�����б��е��������� */
};

```

#### thread.cpp
```cpp
#include <cstdio>   // for printf
#include "thread.h"


void CTask::SetData(void* data) {
    m_ptrData = data;
}

// static��Ա���������������ʼ�������������ⲻ�ܼ���static��
vector<CTask*> CThreadPool::m_vecTaskList;         //�����б�
bool CThreadPool::shutdown = false;

pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;  /** �߳�ͬ���� */
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;    /** �߳�ͬ������������ */

// �̳߳ع��캯�� �� �������̶������ģ��߳�
CThreadPool::CThreadPool(int threadNum) {
    this->m_iThreadNum = threadNum;
    printf("I will create %d threads.\n", threadNum);
    Create();
}

// �����̳߳��е��߳�
int CThreadPool::Create() {
    this->pthread_id  = static_cast<pthread_t*>(malloc(sizeof(pthread_t) * m_iThreadNum));
    for (int i = 0; i != m_iThreadNum; ++i)
        pthread_create(&pthread_id[i], nullptr, ThreadFunc, nullptr);
    return 0;
}
// �̻߳ص�����
void* CThreadPool::ThreadFunc(void* threadData) {
    pthread_t tid = pthread_self(); // ��ȡ��ǰ�̵߳��߳�id
    while (true) {
        pthread_mutex_lock(&m_pthreadMutex);    /** ��ס�߳�ͬ���� */

        /** ������������������ȫ������������ˣ����̳߳��е������û���κ������ˣ�, ���̴߳���˯��״̬*/
        while (m_vecTaskList.size() == 0 && !shutdown) {
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
        }

        if (shutdown) {
            pthread_mutex_unlock(&m_pthreadMutex);
            printf("thread %lu will exit\n", tid);
            pthread_exit(nullptr);
        }

        printf("tid %lu run\n", tid);

        /** ȡ��һ�����񣬲�����֮*/
        vector<CTask*>::iterator iter = m_vecTaskList.begin();
        CTask* task = nullptr;
        if (iter != m_vecTaskList.end()) {
            task = *iter;
            m_vecTaskList.erase(iter);
        }
        pthread_mutex_unlock(&m_pthreadMutex);  /** �����߳�ͬ���� */

        task->Run();    /** ִ������ */
        printf("tid : %lu idle\n", tid);
    }
    return 0;
}

// �������б�����������񣬲�����ͬ���߳�ͬ���ź�
int CThreadPool::AddTask(CTask* task) {
    pthread_mutex_lock(&m_pthreadMutex);
    this->m_vecTaskList.push_back(task);
    pthread_mutex_unlock(&m_pthreadMutex);
    pthread_cond_signal(&m_pthreadCond);
    return 0;
}

// ֹͣ�����߳�
int CThreadPool::StopAll() {
    /** �����ظ����� */
    if (shutdown)
        return -1;
    printf("Now i will end all threads!\n");
    /** �������У�����pthread_cond_wait״̬�ģ��̣߳� �̳߳�Ҫ������ */
    shutdown = true;
    pthread_cond_broadcast(&m_pthreadCond);

    /** �����ȴ��߳��˳��� ����ͳɽ�ʬ�� */
    for (int i = 0; i != m_iThreadNum; ++i)
        pthread_join(pthread_id[i], nullptr);

    free(pthread_id);
    pthread_id = nullptr;

    /** ������������ �� ������ */
    pthread_cond_destroy(&m_pthreadCond);
    pthread_mutex_destroy(&m_pthreadMutex);

    return 0;
}

/**
 * ��ȡ��ǰ������������
 */
int CThreadPool::GetTaskSize()
{
    return m_vecTaskList.size();
}

```

#### main.cpp
```cpp
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
```

### �������к�Ľ�ͼ��
!["C++ʵ���̳߳�"](https://github.com/tycao/tycao.github.io/blob/master/thread_pool/result02.png "C++ʵ���̳߳�")<br />
!["C++ʵ���̳߳�2"](https://github.com/tycao/tycao.github.io/blob/master/thread_pool/result01.png "C++ʵ���̳߳�2")<br />


