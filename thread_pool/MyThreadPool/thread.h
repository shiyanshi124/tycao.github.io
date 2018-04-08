#pragma once

#include <vector>
#include <string>
#include <cstdio>   /** for printf() */
#include <pthread.h>    /** for pthread_t */
#ifdef WIN32_
#include <windows.h>    /** for windows Sleep() */
#else
#include <unistd.h>     /** for Linux sleep(seconds) & usleep(millionseconds) */
#endif
using namespace std;

namespace csx {


/**
 * ִ��������� �� �������ݲ�ִ��
*/
class CTask {
public:
    CTask(){}   /** Ĭ�Ϲ��캯�� */
    CTask(string taskName) : m_strTaskName(taskName) {} /** ����һ��string�����Ĺ��캯�� */
    ~CTask(){}  /** �������� */

    virtual int Run(void) = 0;  /** ���麯����˵������CTask����һ��������࣬����ֱ�ӳ�ʼ����Run������Ҫ�ڸ�����������ʵ�ָ����ض��Ĺ��� */
    void SetData(void*);    /** ������������ */
protected:
    string m_strTaskName;   /** ��������� */
    void* m_ptrName = nullptr;  /** Ҫִ�е�����ľ������� */
};

/**
 * �̳߳ع�����
*/
class CThreadPool {
private:
    static vector<CTask*>   m_vecTaskList;      /** ����أ�������У� */
    static bool             shutdown;           /** �߳��˳���־ */
    int                     m_iThreadNum = 0;   /** �̳߳����������߳����� */
    pthread_t*              pthread_id = nullptr;
    static pthread_mutex_t  m_pthreadMutex;     /** �߳�ͬ���� */
    static pthread_cond_t   m_pthreadCond;      /** �߳�ͬ������������ */
protected:
    static void*    ThreadFunc(void*);          /** ���̵߳��̻߳ص����� */
    static int      MoveToIdle(pthread_t);      /** �߳�ִ�н����󣬰��Լ����뵽�����߳��� */
    static int      MoveToBusy(pthread_t);      /** ���뵽æµ�߳���ȥ */
    int             Create(void);               /** ���̳߳��д����߳� */
public:
    CThreadPool(int threadNum = 10);    /** ���̳߳��д����̣߳�Ĭ��10������ÿ���̶߳���ѭ���أ���ͣ�أ��������б���ȡ������Ȼ��ִ�� */
    int AddTask(CTask*);    /** ��������ӵ���������� */
    int StopAll();          /** ʹ�̳߳��е��߳��˳� */
    int GetTaskSize();      /** ��ȡ��ǰ�����б��е��������� */
};


}
