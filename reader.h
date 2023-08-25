#pragma once
#include "writer.h"
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class reader
{
   public:
    reader(writer& thewriter);
    ~reader();

    void init(const string& name);
    void append(const string &line);
    static void* runner(void*);
    pthread_t run();
    void pout();
    long tme_r;

   private:
    ifstream in;
    writer &thewriter;
    string nme;
};
