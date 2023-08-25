#pragma once
#include <pthread.h>
#include <signal.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <time.h>
        
using namespace std;

class writer
{
   public:
    writer();
    ~writer();

    void init(const string &name);
    static void *runner(void*);
    pthread_t run();
    bool consume(string &line);


    pthread_mutex_t         lock;
    pthread_cond_t          cv;
    long                    tme;

    bool                    eolReached;
    deque<string>           queue;
  private:
    ofstream                out;
};
