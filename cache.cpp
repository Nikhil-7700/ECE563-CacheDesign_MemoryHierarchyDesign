#include "cache.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include<algorithm>

using namespace std;

Cache :: Cache(uint32_t Blocksize, uint32_t Cache_Size, uint32_t Assoc)
{
    this->Blocksize = Blocksize;
    this->Cache_Size = Cache_Size;
    this->Assoc = Assoc;

    this->Sets = (Cache_Size)/(Assoc * Blocksize)+1;
    this->readCount = 0;
    this->writeCount = 0;
    this->readMissCount = 0;
    this->writeMissCount = 0;
    this->readHitCount = 0;
    this->writeHitCount = 0;
    this->writeBackCount = 0;
    this->memoryTraffic = 0;

    this->Offset_Width = (int)log2(Blocksize);
    if (Sets-1!=1) {
        this->Index_Width = (int)log2(Sets);
    }
    else {
        this->Index_Width = 0;
    }

    this->Tag_Width = 32 - Offset_Width - Index_Width;
        
    block = new CacheLine*[Sets];
    for(uint32_t i=0; i<Sets; i++) {
        block[i] = new CacheLine[Assoc];
        for (uint32_t j=0; j<Assoc; j++)
            block[i][j].setCounter(Assoc-(j+1));
    }
    

    L2_Cache = NULL;
}

void Cache :: accessBlock(uint32_t Address, char Op)
{
    setAssociavity(-1);
    setIsHit(false);
    setEvicted(false);
    setWriteBack(false);

    if(Op == 'r')
        readCount++;
    else
        writeCount++;

    CacheLine *Cell = blockFind(Address);
    if(Cell == NULL)					// Address Miss in Cache
    {
        setIsHit(false);
        if(Op == 'r')
            readMissCount++;
        else
            writeMissCount++;

        memoryTraffic++;
        Cell = blockReplace(Address);			// Function to replace CacheLine in case of Miss
    }
    else							// Address Hit in Cache
    {
        setIsHit(true);					// Function to set if Address was Hit or Miss
        if(Op == 'r')
            readHitCount++;
        else
            writeHitCount++;

        updateRanking(Cell, Address);			// Function to Update the Counter for CacheLines
    }

    if(Op == 'w')
        Cell->setDirty(true);
}

CacheLine * Cache :: blockFind(uint32_t Address)
{
    uint32_t Tag = calTag(Address);
    uint32_t Index;
    if (Sets-1!=1) {
        Index = calIndex(Address);
    }
    else {
        Index = (uint32_t)0;
    }
    
    for(int i=0; i<(int)Assoc; i++)
    {
        if((block[Index][i].getTag() == Tag) && (block[Index][i].getValid()))
        {
            setAssociavity(i);
            return &(block[Index][i]);
        }
    }
    return NULL;
}

CacheLine *Cache :: blockReplace(uint32_t Address)
{
    uint32_t Tag = calTag(Address);
    uint32_t Index;
    if (Sets-1==1) {
        Index = uint32_t(0);
    }
    else {
        Index = calIndex(Address);
    }
    CacheLine *evictedBlock = blockEvicted(Index);

    if(evictedBlock->getDirty())
    {
        setWriteBack(true);
        writeBackCount++;
        memoryTraffic++;
    }

    evictedBlock->setTag(Tag);
    evictedBlock->setAddress(Address);
    evictedBlock->setValid(true);
    evictedBlock->setDirty(false);
    updateRanking(evictedBlock, Address);

    return evictedBlock;
}

CacheLine *Cache :: blockEvicted(uint32_t Index)
{
    CacheLine *blockToReplace = blockInvalid(Index);
    if(blockToReplace == NULL)
    {
        blockToReplace = blockReplacement(Index);
        setEvicted(true);
        setEvictedAddress(blockToReplace->getAddress());
    }
    return blockToReplace;
}

CacheLine *Cache :: blockInvalid(uint32_t Index)
{
    for(int i=0; i<(int)Assoc; i++)
    {
        if(block[Index][i].getValid() == false)
        {
            return &(block[Index][i]);
        }
    }
    return NULL;
}

CacheLine *Cache :: blockReplacement(uint32_t Index)
{
    uint32_t ReplAssoc;

    for(uint32_t i=0; i<Assoc; i++)
    {
        if(block[Index][i].getCounter() ==(int)Assoc-1)
        {
            ReplAssoc = i;
        }
    }
    return &(block[Index][ReplAssoc]);
}

void Cache :: updateRanking(CacheLine* Cell, uint32_t Address)
{
    uint32_t Tag = calTag(Address);
    uint32_t Index;
    if (Sets-1!=1) {
        Index = calIndex(Address);
    }
    else {
        Index = (uint32_t)0;
    }
    for (int i=0; i<(int)Assoc; i++){
        if (block[Index][i].getCounter()<Cell->getCounter())
            block[Index][i].setCounter(block[Index][i].getCounter() + 1);
    }
    Cell->setCounter(0);
}

void Cache::printCacheContents() {
    for (uint32_t setIndex = 0; setIndex < Sets-1; setIndex++) {
        cout << "set " << dec << setw(6) << setIndex << ": ";

        // Create a vector to store CacheLine references
        vector<CacheLine*> cacheLines;
        for (uint32_t lineIndex = 0; lineIndex < Assoc; lineIndex++) {
            CacheLine &line = block[setIndex][lineIndex];
            if (line.getValid()) {
                cacheLines.push_back(&line);  // Store the CacheLine reference
            }
        }

        // Sort the CacheLine references based on the Counter value
        sort(cacheLines.begin(), cacheLines.end(), [](CacheLine* a, CacheLine* b) {
            return a->getCounter() < b->getCounter();
        });

        // Print the sorted CacheLines
        for (const auto& line : cacheLines) {
            cout << " " << hex << setw(7) << calTag(line->getAddress());
            if (line->getDirty()) {
                cout << " D";
            } else {
                cout << "  ";
            }
        }

        cout << endl;
    }
}
