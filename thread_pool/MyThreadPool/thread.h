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
