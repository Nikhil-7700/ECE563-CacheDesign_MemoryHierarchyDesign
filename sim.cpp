#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <fstream>
#include "cache.h"
#include "cacheLine.h"
#include "sim.h"

using namespace std;


void print_config(uint32_t *, uint32_t*, uint32_t*, uint32_t*, uint32_t*, uint32_t*, uint32_t*, string);
void print_result(Cache *);

int main(int argc, char *argv[])
{
    if (argc != 9) {
        printf("Error: Expected 8 command-line arguments but was provided %d.\n", (argc - 1));
        exit(EXIT_FAILURE);
    }
    cache_params_t params;
    //FILE fp;
    //uint32_t Blocksize, L1_Size, L1_Assoc, L2_Size, L2_Assoc, PREF_N, PREF_M, Address;
    char op;
    //char *t_file
    string trace_file;
    uint32_t Address;
    //cin >> Blocksize >> L1_Size >> L1_Assoc >> L2_Size >> L2_Assoc >> PREF_N >> PREF_M >> trace_file;

    params.BLOCKSIZE = (uint32_t) atoi(argv[1]);
    params.L1_SIZE   = (uint32_t) atoi(argv[2]);
    params.L1_ASSOC  = (uint32_t) atoi(argv[3]);
    params.L2_SIZE   = (uint32_t) atoi(argv[4]);
    params.L2_ASSOC  = (uint32_t) atoi(argv[5]);
    params.PREF_N    = (uint32_t) atoi(argv[6]);
    params.PREF_M    = (uint32_t) atoi(argv[7]);
    trace_file       = argv[8];    

    uint32_t Blocksize = params.BLOCKSIZE;
    uint32_t L1_Size = params.L1_SIZE;
    uint32_t L1_Assoc = params.L1_ASSOC;
    uint32_t L2_Size = params.L2_SIZE;
    uint32_t L2_Assoc = params.L2_ASSOC;
    uint32_t PREF_N = params.PREF_N;
    uint32_t PREF_M = params.PREF_M;

    Cache *L1, *L2;
    L1 = new Cache(Blocksize, L1_Size, L1_Assoc);
    if(L2_Size!=0)
    {
        L2 = new Cache(Blocksize, L2_Size, L2_Assoc);
        L1->set_L2Cache(L2);
    }

    uint32_t prefUnit[PREF_N];
    bool prefUnitValidBit[PREF_N];
    int prefUnitCounter[PREF_N];
    if (PREF_N!=0){
	for (int i=0 ;i<PREF_N; i++){
	    prefUnitCounter[i] = PREF_N - i - 1;
        }
    }

    ifstream fin;
    fin.open(trace_file);
    if (!fin.is_open()) {
        cerr << "Error: Unable to open trace file '" << trace_file << "'" << endl;
        return 1; // Exit with an error code
    }
    while(fin >> op >> hex >> Address)
    {
        L1->accessBlock(Address, op);
        if(L2_Size != 0)
        {
            if(L1->getWriteBack() == true)
                L2->accessBlock(L1->getEvictedAddress(), 'w');
            if(L1->getIsHit() == false)
                L2->accessBlock(Address, 'r');
        }
    }

    fin.close();

    print_config(&Blocksize, &L1_Size, &L1_Assoc, &L2_Size, &L2_Assoc, &PREF_N, &PREF_M, trace_file);
    cout << "===== L1 contents =====" << endl;
    L1->printCacheContents();
    if (L2_Size != 0) {
        cout << endl;
        cout << "===== L2 contents =====" << endl;
        L2->printCacheContents();
    }
    cout << endl;
    print_result(L1);

    return 0;
}

void print_config(uint32_t *Blocksize, uint32_t *L1_Size, uint32_t *L1_Assoc, uint32_t *L2_Size, uint32_t *L2_Assoc, uint32_t *PREF_N, uint32_t *PREF_M, string fname)
{
    cout << "===== Simulator configuration =====" << endl;
    cout << "BLOCKSIZE:  " << *Blocksize << endl;
    cout << "L1_SIZE:    " << *L1_Size << endl;
    cout << "L1_ASSOC:   " << *L1_Assoc << endl;
    cout << "L2_SIZE:    " << *L2_Size << endl;
    cout << "L2_ASSOC:   " << *L2_Assoc << endl;
    cout << "PREF_N:     " << *PREF_N << endl;
    cout << "PREF_M:     " << *PREF_M << endl;
    cout << "trace_file: " << fname << endl << endl;
}

void print_result(Cache *L1)
{
    cout << dec << "===== Measurements =====" << endl;
    cout << dec << "a. L1 reads:                   " << L1->getReads() << endl;
    cout << dec << "b. L1 read misses:             " << L1->getRdMiss() << endl;
    cout << dec << "c. L1 writes:                  " << L1->getWrites() << endl;
    cout << dec << "d. L1 write misses:            " << L1->getWtMiss() << endl;
    cout << dec << "e. L1 miss rate:               " << fixed << setprecision(4) << (float)(((float)(L1->getRdMiss() + L1->getWtMiss()))/(L1->getReads() + L1->getWrites())) << endl;
    cout << dec << "f. L1 writebacks:              " << L1->getWtBacks() << endl;
    cout << dec << "g. L1 prefetches:              " << 0 << endl;

    Cache *L2 = L1->get_L2Cache();

    if(L2 == NULL)
    {
        cout << "h. L2 reads (demand):          " << 0 << endl;
        cout << "i. L2 read misses (demand):    " << 0 << endl;
        cout << "j. L2 reads (prefetch):        " << 0 << endl;
        cout << "k. L2 read misses (prefetch):  " << 0 << endl;
        cout << "l. L2 writes:                  " << 0 << endl;
        cout << "m. L2 write misses:            " << 0 << endl;
        cout << "n. L2 miss rate:               " << 0.0000 << endl;
        cout << "o. L2 writebacks:              " << 0 << endl;
        cout << "p. L2 prefetches:              " << 0 << endl;
        cout << "q. memory traffic:             " << L1->getMemoryTraffic() << endl;
    }
    else
    {
        cout << "h. L2 reads (demand):          " << L2->getReads() << endl;
        cout << "i. L2 read misses (demand):    " << L2->getRdMiss() << endl;
        cout << "j. L2 reads (prefetch):        " << 0 << endl;
        cout << "k. L2 read misses (prefetch):  " << 0 << endl;
        cout << "l. L2 writes:                  " << L2->getWrites() << endl;
        cout << "m. L2 write misses:            " << L2->getWtMiss() << endl;
        cout << "n. L2 miss rate:               " << fixed << setprecision(4) << (float)(((float)(L2->getRdMiss()))/(L2->getReads())) << endl;
        //cout << "n. L2 miss rate:               " << fixed << setprecision(4) << (float)(((float)(L2->getRdMiss() + L2->getWtMiss()))/(L2->getReads() + L2->getWrites())) << endl;
        cout << "o. L2 writebacks:              " << L2->getWtBacks() << endl;
        cout << "p. L2 prefetches:              " << 0 << endl;
        cout << "q. memory traffic:             " << L2->getMemoryTraffic() << endl;
    }
}

