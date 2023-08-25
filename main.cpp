#include "reader.h"
#include "writer.h"
#include <cstdlib>
#include <numeric>

reader* readers;
writer* writers;

void cleanup()
{

}

int main(int argc, char** argv)
{
    atexit(cleanup);

    int nthreads = atoi(argv[1]);
    string infile  = string(argv[2]);
    string outfile = string(argv[3]);

    writer myWriter;
    myWriter.init(outfile);

    printf("\x1B[96mWRITING TO FILE \033[0m\t\t");
    printf("\x1B[36mTexting\033[0m\t\t");
    printf("\x1B[36mTexting\033[0m\t\t");

    reader myReader(myWriter);
    myReader.init(infile);

    clock_t start = clock();
    constexpr int THREAD_NUM = 100;
    pthread_t *th = new pthread_t[nthreads *2];
    for (int i = 0; i < nthreads; i++)
    {
        cout << "main() : creating thread, " << i << endl;
        th[2*i+0] = myReader.run();
        th[2*i+1] = myWriter.run();
    }

    for (int i = 0; i < nthreads *2; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
            return EXIT_FAILURE;
        }
    }

    cout << "threads joined" << endl;

    //int time_wr = accumulate(myWriter.tme.begin(), myWriter.tme.end(), 0) / myWriter.tme.size();
    //int time_rdr = accumulate(myReader.tme_r.begin(), myReader.tme_r.end(), 0) / myReader.tme_r.size();

     double ttl = (double)myWriter.tme / ((double)CLOCKS_PER_SEC*nthreads);
     double ttl_r = (double)myReader.tme_r / ((double)CLOCKS_PER_SEC * nthreads);
     cout << "writer time total: " << myWriter.tme << " writer average: " << ttl << endl; //<< " time per thread: " << endl;// << "reader av time: " << time_rdr;
     cout << "reader time total: " << myReader.tme_r << " reader average: " << ttl_r << endl; //<< " time per thread: " << endl;// << "reader av time: " << time_rdr;

    delete[] th;
    clock_t end = clock();
    clock_t duration = end - start;
    double seconds = (long)duration / (double)CLOCKS_PER_SEC;
    cout << "time taken for entire process " << seconds << endl;
    cout << "finished" << endl;
    return EXIT_SUCCESS;
}
