#ifndef __CACHE_H__
#define __CACHE_H__
#include "cacheLine.h"
#include <string>
#include <cstdint>

using namespace std;

class Cache
{
private:
    CacheLine ** block;
    Cache *L2_Cache;
    uint32_t Blocksize, Cache_Size, Assoc, Sets, Tag, Offset, Index;
    int Index_Width, Tag_Width, Offset_Width;
    uint32_t readCount, writeCount, readMissCount, writeMissCount, readHitCount, writeHitCount, writeBackCount, memoryTraffic;

    bool Hit, Evicted, WB;
    int Associavity;
    uint32_t EvictedAddress;

public:
    Cache(uint32_t , uint32_t , uint32_t);
    uint32_t calTag(uint32_t Address) {return ((Address)>>(Index_Width + Offset_Width));}
    uint32_t calIndex(uint32_t Address) {return (((Address)<<(Tag_Width))>>(Offset_Width+Tag_Width));}
    uint32_t calOffset(uint32_t Address) {return (((Address)<<(Tag_Width+Index_Width))>>(Tag_Width+Index_Width));}

    void accessBlock(uint32_t, char);
    CacheLine *blockFind(uint32_t);
    CacheLine *blockReplace(uint32_t);
    CacheLine *blockEvicted(uint32_t);
    CacheLine *blockInvalid(uint32_t);
    CacheLine *blockReplacement(uint32_t);
    void updateRanking(CacheLine *, uint32_t);
    void printCacheContents();

    bool getIsHit() {return this->Hit;}
    bool getEvicted() {return this->Evicted;}
    int getAssociavity() {return this->Associavity;}
    uint32_t getEvictedAddress() {return this->EvictedAddress;}
    bool getWriteBack() {return this->WB;}

    void setIsHit(bool Hit) {this->Hit = Hit;}
    void setEvicted(bool Evicted) {this->Evicted = Evicted;}
    void setAssociavity(int Associavity) {this->Associavity = Associavity;}
    void setEvictedAddress(uint32_t EvictedAddress) {this->EvictedAddress = EvictedAddress;}
    void setWriteBack(bool WB) {this->WB = WB;}

    uint32_t getReads() {return this->readCount;}
    uint32_t getWrites() {return this->writeCount;}
    uint32_t getRdMiss() {return this->readMissCount;}
    uint32_t getWtMiss() {return this->writeMissCount;}
    uint32_t getRdHits() {return this->readHitCount;}
    uint32_t getWtHits() {return this->writeHitCount;}
    uint32_t getWtBacks() {return this->writeBackCount;}
    uint32_t getMemoryTraffic() {return this->memoryTraffic;}

    void set_L2Cache(Cache *L2_Cache) {this->L2_Cache = L2_Cache;}
    Cache *get_L2Cache(){ return this->L2_Cache;}
};
#endif
