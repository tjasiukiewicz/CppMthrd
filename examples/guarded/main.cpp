#include <memory>
#include <mutex>
#include <vector>

#include <iostream>

template<typename T, typename M = std::mutex>
class guarded {
    class unlocker;
public:
    using handler_t = std::unique_ptr<T, unlocker>;

    template<typename... TT>
    guarded(TT&&... data);

    handler_t lock() {
        std::unique_lock<M> lck(mtx);
        return handler_t(&obj, unlocker(std::move(lck)));
    }
    handler_t try_lock() {
        std::unique_lock<M> lck(mtx, std::try_to_lock);
        if(lck.owns_lock()) {
            return handle_t(&obj, unlocker(std::move(lock)));
        }
        return handle_t(nullptr, unlocker(std::move(lock)));
    }
private:
    class unlocker {
    public:
        using pointer_t = T *;
        unlocker(std::unique_lock<M> lock): lck{std::move(lock)} {}
        void operator()(T * ptr) {
            if(lck.owns_lock()) {
                lck.unlock();
            }
            (void) ptr;
        }
    private:
        std::unique_lock<M> lck;
    };
    T obj;
    M mtx;

};

template<typename T, typename M>
template<typename... TT>
guarded<T, M>::guarded(TT&&... data): obj(std::forward<TT>(data)...) {}

int main() {
    std::vector<int> v = {1203,12, 43, 4123};
    guarded<decltype(v)> gv(v);
    {
        auto myVec = *(gv.lock());
        std::cout << myVec[1] << '\n';
    }
}
