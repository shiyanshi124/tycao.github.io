ʹ��C++11ʵ���̳߳�
=========
### ���ȣ������˽����̳߳ص�ԭ��
!["thrPool1"](https://github.com/tycao/Cpp_mini_projects/blob/master/thread_pool/threadpool_1.png "thrPool1")<br />
����ͼ�����ǿ��Կ�����**�̳߳ؾ���һ��_����_���������̶��������̣߳���Щ�߳�һֱ��ͣ�ش�����أ��ֳ�������У���ȡ�����񣬲�ִ�С�**
��ִ������Ĺ����У������߳�֮��Ĳ���ʹ����ģ���ȷ���̰߳�ȫ��

### ���ϴ���
#### thread.h
```cpp
#pragma once

#include <stdio.h>  // for printf()
#include <iostream>
#include <thread>
#include <chrono>   // for std::chrono::seconds()
#include <future>   // for future<>
#include <string>
#include <vector>
#include <queue>
#include <memory>   // for make_shared<>, shared_ptr<>
#include <functional>   // for std::function<>

class ThreadPool {
public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template<typename F, typename... Args>
    auto enqueue(F &&f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;

    bool stop;
};

inline ThreadPool::ThreadPool(size_t threads)
    : stop(false)
{
    /** ���̳߳��д���������Ϊthreads�����߳� */
    for (size_t i = 0; i < threads; ++i)
    {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);

                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });

                    /** ��������֮ǰ�����������е�˯���߳� */
                    if (this->stop && this->tasks.empty())
                        return;

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    /** �������У����ɻ�ģ�˯���߳� */
    condition.notify_all();

    /** ���������̣߳��������main���߳� */
    for (std::thread &worker: workers)
        worker.join();
}

/**
 * ���̳߳ص�����������������
*/
template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
                ->std::future<typename std::result_of<F(Args...)>::type>
{

    using return_type = typename std::result_of<F((Args...))>::type;

    auto task =
    std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]{(*task)();});
    }

    condition.notify_one();
    return res;

}
```

#### main.cpp
```cpp
#include "thread.h"

int main(int argc, const char * argv[])
{
    /** ���̳߳��д���4���߳� */
    ThreadPool pool(4);

    std::vector<std::future<std::string>> results;

    for (int i = 0; i < 8; i++)
    {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << (std::string("hello") + std::to_string(i)) << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(16 - i * 2));
                std::cout << (std::string("world") + std::to_string(i)) << std::endl;
                return std::string("---thread") + std::to_string(i) + std::string( "finished---");
            })
        );
    }

    for (auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    return 0;
}
```

### ��������֮��Ľ�ͼ��
!["C++ʵ���̳߳�3"](https://github.com/tycao/tycao.github.io/blob/master/thread_pool/result03.png "C++ʵ���̳߳�3")<br />

