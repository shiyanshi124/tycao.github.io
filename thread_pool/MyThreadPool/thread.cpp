#include "thread.h"

namespace csx {



void CTask::SetData(void* data) {
    m_ptrName = data;
}

/** static ��Ա���������������ʼ���������ⲻ�ܼ�������static */
vector<CTask*>   CThreadPool::m_vecTaskList;                                 /** ����أ�������У� */
bool             CThreadPool::shutdown = false;                              /** �߳��˳���־ */
pthread_mutex_t  CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;    /** �߳�ͬ���� */
pthread_cond_t   CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;      /** �߳�ͬ������������ */

/** ���캯�����������̶������ģ��߳� */
CThreadPool::CThreadPool(int threadNum)
    : m_iThreadNum(threadNum)
{
    printf("i will create %d threads\n", m_iThreadNum);
    Create();
}

/** �����̳߳����߳� */
int CThreadPool::Create(void)
{
    pthread_id = static_cast<pthread_t*>(malloc(sizeof(pthread_t) * m_iThreadNum));
    for (int i = 0; i != m_iThreadNum;  ++i)
        pthread_create(&pthread_id[i], nullptr, ThreadFunc, nullptr);
    return 0;
}

/** �̻߳ص����� */
void* CThreadPool::ThreadFunc(void* threadData) {
    pthread_t tid = pthread_self(); // ��ȡ��ǰ�̵߳��߳�id
    while (true) {
        pthread_mutex_lock(&m_pthreadMutex);

        /** ���������û���κ�����֮�����̴߳���˯��״̬ */
        if (m_vecTaskList.size() == 0 && !shutdown)
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);

        if (shutdown) {
            pthread_mutex_unlock(&m_pthreadMutex);
            printf("tid %lu will exit\n", tid);
            pthread_exit(nullptr);
        }

        printf("tid %lu run\n", tid);
        /** ȡ��һ�����񣬲�ִ�� */
        vector<CTask*>::iterator iter = m_vecTaskList.begin();
        CTask* task;
        if (iter != m_vecTaskList.end()) {
            task = *iter;
            m_vecTaskList.erase(iter);
        }
        pthread_mutex_unlock(&m_pthreadMutex);

        task->Run();    /** ִ������ */
        printf("tid %lu idle\n", tid);
    }
    return 0;
}
/** ��������ӵ���������� */
int CThreadPool::AddTask(CTask* task) {
    pthread_mutex_lock(&m_pthreadMutex);
    m_vecTaskList.push_back(task);
    pthread_mutex_unlock(&m_pthreadMutex);
    pthread_cond_signal(&m_pthreadCond);
    return 0;
}
/** ʹ�̳߳��е��߳��˳� */
int CThreadPool::StopAll() {
    /** �����ظ����� */
    if (shutdown) return -1;

    printf("i will end all threads.\n");
    /** �������У�����pthread_cond_wait״̬���̣߳��̳߳�Ҫ������ */
    shutdown = true;
    pthread_cond_broadcast(&m_pthreadCond);

    /** ʹ�����ȴ��߳��˳�������ͳɽ�ʬ�߳��� */
    for (int i = 0; i != m_iThreadNum; ++i)
        pthread_join(pthread_id[i], nullptr);

    /** �ͷ��̳߳� */
    free(pthread_id);
    pthread_id = nullptr;

    /** ������������ �� ������ */
    pthread_cond_destroy(&m_pthreadCond);
    pthread_mutex_destroy(&m_pthreadMutex);

    return 0;
}
/** ��ȡ��ǰ�����б��е��������� */
int CThreadPool::GetTaskSize() {
    return m_vecTaskList.size();
}


}
