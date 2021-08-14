#include<thread>
#include<queue>
#include<iostream>
#include<condition_variable>
#include<mutex>
#include<atomic>
#include<unistd.h>
#include<semaphore.h>

using namespace std;
typedef void (*func)(int) ;

class ThreadTask {
    public:
        int data;
        func f;
    public:
        ThreadTask(func f,int data){
            this->f = f;
            this->data = data;
        };
        ThreadTask(const ThreadTask& p){
            f = p.f;
            data = p.data;
        }
        ThreadTask(){};
        void run();
};
void ThreadTask::run() {
    f(data);
}


class ThreadPool{
    private:
        condition_variable  cond;
        ThreadTask          _task;
        mutex               m_task;
        mutex               queue_mtx;
        sem_t*              sem_task;
        int                 capacity;
        atomic_int          freeNumber;
        queue<ThreadTask>   TaskQueue;
        vector<thread*>     thread_;
        bool                breakPoint;
    public:
        ThreadPool(int capacity = 2);
        ~ThreadPool(){};
        void append(ThreadTask& Task);
        static void worker(void* p1);
        static void manager(ThreadPool* p);
        void cancel();
        
};
ThreadPool::ThreadPool(int capacity) {
    breakPoint = true;
    sem_init(sem_task,0,1);
    freeNumber = 0;
    for(int i = 0;i < capacity;i++){
        thread t(worker,this);
        thread_.push_back(&t);
        t.detach();
    }
    thread t(manager,this);
    t.detach();
    
}

void ThreadPool::append(ThreadTask& Task) {
    queue_mtx.lock();
    TaskQueue.push(Task);
    queue_mtx.unlock();
}

void ThreadPool::worker(void* p1){
    ThreadTask task;
    ThreadPool* p = static_cast<ThreadPool*>(p1);
    while(true){
        unique_lock<mutex> lk(p->m_task);
        // cout<<"线程"<<this_thread::get_id()<<"准备完毕\n";
        // cout << "待完成任务" << p->TaskQueue.size()  << endl;
        (p->freeNumber)++;
        (p->cond).wait(lk);
        // cout << this_thread::get_id() << "获得--------------" << p->_task.data << endl;
        task = p->_task;     

        sem_post(p->sem_task);

        lk.unlock();
        task.run();
        // cout << "line90\n";
    }
}

void ThreadPool::manager(ThreadPool* p){
    while(true){
        if(p->freeNumber > 0 && !p->TaskQueue.empty()){
            p->freeNumber--;
            sem_wait(p->sem_task);

            unique_lock<mutex> lk(p->queue_mtx);
            unique_lock<mutex> lk2(p->m_task);
            p->_task = p->TaskQueue.front();
            // cout << "本次任务"<<p->_task.data << "----------queue.size  " << p->TaskQueue.size() << endl;
            p->TaskQueue.pop();

            (p->cond).notify_one();

        }
        
        
    }
}

void ThreadPool::cancel(){
    breakPoint = true;
}