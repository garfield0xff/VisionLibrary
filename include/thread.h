#include <thread>
#include <atomic>

using namespace std;

typedef void(*proc_t)();

namespace vl {

class ThreadPool {
    public:
        ThreadPool(size_t nthreads) : _nthreads(nthreads) {};
        template<class CLASS>
        bool createThread(void(CLASS::*proc)(), CLASS *pthis) {
            threads.emplace_back(proc, pthis);
            return true;
        }

        void joinAll() {
            for(auto &t : threads) {
                if(t.joinable()) {
                    t.join();
                }
            }
        }
        
    protected: 
        size_t _nthreads;
        vector<thread> threads;
        thread t;
};
};


