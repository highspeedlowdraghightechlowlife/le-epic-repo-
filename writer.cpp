#include <chrono>
#include <thread>
#include "writer.h"
#include "reader.h"
#include <time.h>

//std::ofstream writer::out;
//deque<string> writer::queue;

void writer::init(const std::string &name) 
{
	out.open(name);
}

writer::~writer()
{
    out.close();
}

writer::writer()
{
    eolReached = false;
    pthread_mutex_init(&lock, NULL);
}

pthread_t writer::run()
{
    //pthread_mutex_lock(&lock);
    pthread_t ptid;
    int rc;
    rc = pthread_create(&ptid, NULL, &writer::runner, this);
    if (rc != 0)
    {
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }
    return ptid;
}

void* writer::runner(void *arg)
{
    //using namespace std::chrono_literals;
    writer *inst = static_cast<writer *>(arg);
    long count = 0;
 

    while (true)
    {
        clock_t start = clock();
        pthread_mutex_lock(&inst->lock);
        clock_t mutelock = clock();

        //pthread_cond_wait(&inst->cv, &inst->lock);
            //string stln;
            if (inst->queue.empty() && inst->eolReached)
            {
                pthread_mutex_unlock(&inst->lock);
                break;
            }

            for (int i = 0; i < 1000; i++)
            {
                if (!inst->queue.empty())
                {
                    string &ln = inst->queue.front();
                    inst->out << ln << "\n";
                    inst->queue.pop_front();
                }
            }

            clock_t end = clock();
            clock_t duration = end - start;
            inst->tme += duration;
            //printf("writer %d %d\n", count++, pthread_self());

        pthread_mutex_unlock(&inst->lock);
        this_thread::sleep_for(5ms);

    }

    printf("finifed writer %d\n", pthread_self());
    return nullptr;
}

bool writer::consume(string &line)
{
    if (queue.size() == 0)
    {
        return false;
    }

    line = queue.front();
    queue.pop_front();
    return true;
}
//void writer::setfinished() {}
