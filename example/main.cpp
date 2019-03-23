#include <unistd.h>
#include <sys/time.h>
#include <iostream>

#include <time_heap.h>

class TestItem : public DcTimer {
public:
    int i_;
};

int main() {
    DcTimeHeap time_heap(6);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    uint64_t now_ms = tv.tv_sec*1000 + tv.tv_usec/1000;

    for (int i = 0; i < 30; i+=3) {
        {
        TestItem* tmp = new TestItem();
        tmp->i_ = i;
        
        time_heap.AddTimer(tmp, now_ms + i/3 * 1000);

        if (i == 9) {
            time_heap.DelTimer(tmp);    
        }

        }
{
        TestItem* tmp1 = new TestItem();
        tmp1->i_ = i+1;
        
        time_heap.AddTimer(tmp1, now_ms + i/3 * 1000);
}
{
        TestItem* tmp1 = new TestItem();
        tmp1->i_ = i+2;
        
        time_heap.AddTimer(tmp1, now_ms + i/3 * 1000);
}
    }

    for (;;) {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        uint64_t now_ms1 = tv.tv_sec*1000 + tv.tv_usec/1000;
        //std::cout << now_ms << std::endl; 

        if (now_ms1 - now_ms > 20 * 1000) {
            break;
        }

        std::vector<DcTimer*> ret = time_heap.PeriodicCheckExpired(now_ms1);
        for (uint32_t i = 0; i < ret.size(); i++) {

            std::cout << ((TestItem*)ret[i])->i_ << std::endl; 
            delete ((TestItem*)ret[i]);
        }

        usleep(100 * 1000);
    }


    return 0;
}
