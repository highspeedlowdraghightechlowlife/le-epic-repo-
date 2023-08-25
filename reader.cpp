#include <chrono>
#include <thread>
#include "writer.h"
#include "reader.h"

//ifstream reader::in;

reader::reader(writer& mywriter):
    thewriter(mywriter)
{
}


void reader::init(const string &name) 
{
    //string ch;
	in.open(name);
    cout << "initializing reader with name "<< name << endl;
    //cout << (in.peek() == EOF) << endl;
}

reader::~reader()
{
    in.close();
}

pthread_t reader::run()
{
    pthread_t ptid;
  //  int rc;
    int rc = pthread_create(&ptid, NULL, &reader::runner, this);
    if (rc != 0) 
    {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    return ptid;
}

void reader::pout()
{
    cout << "queue size: " << thewriter.queue.size() << " ";
    for (int i = thewriter.queue.size() - 1; i >= 0; i--)
    {
        cout << thewriter.queue[i] << " ";
    }
    cout << endl;
}

void reader::append(const string &line)
{
    thewriter.queue.push_back(line);
}

void* reader::runner(void* arg) 
{
    reader *inst = static_cast<reader *>(arg);
    long count = 0;
   // clock_t start = clock();
    while (true)
    {
        clock_t start = clock();
        pthread_mutex_lock(&inst->thewriter.lock);
        clock_t mutelock = clock();
            for (int i = 0; i < 1000; i++)
            {
                if (inst->in.peek() == EOF)
                {
                    inst->thewriter.eolReached = true;
                    pthread_t tID = pthread_self();
                    printf("eof in %d\n", pthread_self());
                    pthread_mutex_unlock(&inst->thewriter.lock);
                    break;
                }
                string stln;
                getline(inst->in, stln);
                inst->append(stln);
                //cout << " in:" << stln << " queue " << inst->thewriter.queue.size() << endl;
            }
            clock_t end = clock();
            clock_t duration = end - start;
            inst->tme_r += duration;
            //printf("reader %d %d\n", count++, pthread_self());
        pthread_mutex_unlock(&inst->thewriter.lock);
        //pthread_cond_signal(&inst->thewriter.cv);


        if (inst->thewriter.eolReached)
        {
             break;
        }
        this_thread::sleep_for(5ms);
    }
    //clock_t end = clock();
    //clock_t duration = end - start;
    //int seconds = (int)duration / (double)CLOCKS_PER_SEC;
    //cout << seconds;
    printf("finifed reader %d\n", pthread_self());
    return nullptr;
}