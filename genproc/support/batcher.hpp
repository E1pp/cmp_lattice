#pragma once

#include <weave/executors/executor.hpp>
#include <weave/futures/make/submit.hpp>
#include <weave/futures/combine/par/all.hpp>
#include <weave/futures/run/await.hpp>

#include <vector>

namespace cmp_lattice::support {

//! Sends tasks into executor in batches
template <typename Task, typename Executor>
    requires std::is_invocable_r_v<void, Task>
class Batcher
{
public:
    Batcher(
        Executor& exe,
        std::size_t batch_size)
        : executor_(exe)
        , batch_size_(batch_size)
    {
        batch_.reserve(batch_size_);
    }

    void Submit(Task task)
    {
        batch_.emplace_back(weave::futures::Submit(executor_, task));

        if (batch_.size() == batch_size_) {
            Flush();
        }
    }

    void Flush()
    {
        auto status = weave::futures::no_alloc::All(std::move(batch_)) | weave::futures::ThreadAwait();

        assert(status);

        assert(batch_.empty());
    }

    ~Batcher()
    {
        if (!batch_.empty()){
            Flush();
        }
    }
    
private:
    using FutureType = decltype(weave::futures::Submit(std::declval<Executor&>(), std::declval<Task>()));

    Executor& executor_;
    std::vector<FutureType> batch_;
    std::size_t batch_size_;
};


} // namespace cmp_lattice::support
