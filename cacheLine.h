#ifndef __CACHELINE_H__
#define __CACHELINE_H__

#include <cstdint>

class CacheLine {
private:
    uint32_t Address, Tag, Index;
    bool ValidBit, DirtyBit;
    int Counter;

public:
    CacheLine()
    {
        this->Address = -1;
        this->Tag = 0;
        this->Index = 0;
        this->ValidBit = false;
        this->DirtyBit = false;
    }

    uint32_t getTag() { return this->Tag; }
    uint32_t getAddress() { return this->Address; }
    bool getDirty() { return this->DirtyBit; }
    int getCounter() {return this->Counter; }
    bool getValid() {return this-> ValidBit; }

    void setTag(uint32_t Tag) { this->Tag = Tag; }
    void setAddress(uint32_t Address) { this->Address = Address; }
    void setValid(bool ValidBit) { this->ValidBit = ValidBit; }
    void setDirty(bool DirtyBit) { this->DirtyBit = DirtyBit; }
    void setCounter(int Counter) { this->Counter = Counter; }
};

#endif
