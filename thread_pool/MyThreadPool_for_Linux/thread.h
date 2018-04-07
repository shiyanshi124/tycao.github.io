#ifndef __THREAD_H   
#define __THREAD_H   
  
#include <vector>   
#include <string>   
#include <cstdio>
#include <cstdlib>
#include <pthread.h>   
  
using namespace std;  
  
/** 
 * ִ��������࣬�����������ݲ�ִ�� 
 */  
class CTask  
{  
protected:  
    string m_strTaskName;  /** ��������� */  
    void* m_ptrData;       /** Ҫִ�е�����ľ������� */  
public:  
    CTask(){}  
    CTask(string taskName)  
    {  
        m_strTaskName = taskName;  
        m_ptrData = NULL;  
    }  
    virtual int Run()= 0;  
    void SetData(void* data);    /** ������������ */  
  
public:  
    virtual ~CTask(){}  
};  
  
/** 
 * �̳߳ع������ʵ�� 
 */  
class CThreadPool  
{  
private:  
    static  vector<CTask*> m_vecTaskList;     /** �����б� */  
    static  bool shutdown;                    /** �߳��˳���־ */           
    int     m_iThreadNum;                     /** �̳߳����������߳��� */  
    pthread_t   *pthread_id;  
      
    static pthread_mutex_t m_pthreadMutex;    /** �߳�ͬ���� */  
    static pthread_cond_t m_pthreadCond;      /** �߳�ͬ������������ */  
  
protected:  
    static void* ThreadFunc(void * threadData); /** ���̵߳��̻߳ص����� */  
    static int MoveToIdle(pthread_t tid);       /** �߳�ִ�н����󣬰��Լ����뵽�����߳��� */  
    static int MoveToBusy(pthread_t tid);       /** ���뵽æµ�߳���ȥ */  
      
    int Create();          /** �����̳߳��е��߳� */  
  
public:  
    CThreadPool(int threadNum = 10);  
    int AddTask(CTask *task);      /** ��������ӵ���������� */  
    int StopAll();                 /** ʹ�̳߳��е��߳��˳� */  
    int getTaskSize();             /** ��ȡ��ǰ��������е������� */  
};  
  
#endif  