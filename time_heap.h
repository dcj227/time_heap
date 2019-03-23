#ifndef __DC_TIME_HEAP_H__
#define __DC_TIME_HEAP_H__

#include <stdint.h>
#include <cstdlib>
#include <vector>

class DcTimer {
public:
    DcTimer()
        : in_use_(true)
        , expire_time_ms_(0) {
    }

    bool        in_use_;
    uint64_t    expire_time_ms_;
    uint32_t    index_;         // 在heap数组中的index, ResetTimer需要
};

class DcTimeHeap {
public:
    DcTimeHeap(uint32_t capacity);
    ~DcTimeHeap();

    int AddTimer(DcTimer* t, uint64_t expire_time_ms);
    void DelTimer(DcTimer* t);
    int ResetTimer(DcTimer* t, uint64_t expire_time_ms);

    std::vector<DcTimer*> PeriodicCheckExpired(uint64_t now_ms); 

private:
    int Insert(DcTimer* t);
    void DelTop();

    int Resize();

    uint32_t capacity_;     // 不算下标为0的元素
    uint32_t cur_size_;     // 不算下标为0的元素

    DcTimer** heap_;        // 下标从1开始使用
};


#endif  //  __DC_TIME_HEAP_H__

