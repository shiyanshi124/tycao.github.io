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
