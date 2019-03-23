#include "time_heap.h"

#include <string.h>

int CompareTimer(DcTimer* t1, DcTimer* t2) {
    if (t1->expire_time_ms_ > t2->expire_time_ms_) {
        return 1;
    } else if (t1->expire_time_ms_ == t2->expire_time_ms_) {
        return 0;
    } else {
        return -1;
    }
}

DcTimeHeap::DcTimeHeap(uint32_t capacity) 
    : capacity_(capacity)
    , cur_size_(0)
    , heap_(new DcTimer*[capacity_+1]) {
}

DcTimeHeap::~DcTimeHeap() {
    if (heap_) {
        delete [] heap_;
        heap_ = NULL;
    }
}

int DcTimeHeap::AddTimer(DcTimer* t, uint64_t expire_time_ms) {
    t->in_use_ = true;
    t->expire_time_ms_ = expire_time_ms;
    
    return Insert(t);
}

void DcTimeHeap::DelTimer(DcTimer* t) {
    t->in_use_ = false; 
}

int DcTimeHeap::ResetTimer(DcTimer* t, uint64_t expire_time_ms) {
    if (!t->in_use_) {
        return -1;
    }

    DcTimer* tmp = new DcTimer();
    *tmp = *t;
    tmp->in_use_ = false;

    heap_[tmp->index_] = tmp;

    return Insert(t);  
}

std::vector<DcTimer*> DcTimeHeap::PeriodicCheckExpired(uint64_t now_ms) {
    std::vector<DcTimer*> expireds;

    while (cur_size_ > 0 && heap_[1]->expire_time_ms_ <= now_ms) {
        if (heap_[1]->in_use_) {
            expireds.push_back(heap_[1]);
        }

        DelTop();
    }

    return expireds;
}

int DcTimeHeap::Insert(DcTimer* t) {
    if (cur_size_ >= capacity_) {
        int ret = Resize();
        if (ret != 0) {
            return ret;
        }
    } 

    uint32_t i = cur_size_ + 1;
    uint32_t gi = i / 2;            // 父节点的位置
    while (i > 1) {
        if (CompareTimer(t, heap_[gi]) < 0) {
            heap_[i] = heap_[gi];
            i = gi;
            gi = i / 2;
        } else {
            break;
        }
    }

    t->index_ = i;
    heap_[i] = t;

    cur_size_++;

    return 0; 
}

void DcTimeHeap::DelTop() {
    if (cur_size_ == 0) {
        return;
    }
    if (cur_size_ == 1) {
        cur_size_--;
        return;
    }

    uint32_t i = 1, ci = 2;
    while (ci < cur_size_) {
        if (CompareTimer(heap_[ci], heap_[ci+1]) > 0) {
            ci++;
        } 

        if (CompareTimer(heap_[cur_size_], heap_[ci]) > 0) {
            heap_[i] = heap_[ci];
            i = ci;
            ci = i * 2;
        } else {
            break;
        }
    }
    heap_[i] = heap_[cur_size_];
    
    cur_size_--;
}

int DcTimeHeap::Resize() {
    uint32_t new_capacity = capacity_ * 2;

    DcTimer** tmp_new = new DcTimer*[new_capacity + 1];
    if (!tmp_new) {
        return -1;
    }

    memcpy(tmp_new, heap_, (capacity_ + 1) * sizeof(DcTimer*));

    delete [] heap_;

    capacity_ = new_capacity;
    heap_ = tmp_new;

    return 0;
}
