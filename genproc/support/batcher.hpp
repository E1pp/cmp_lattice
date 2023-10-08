#pragma once

#include <weave/executors/executor.hpp>
#include <weave/futures/make/submit.hpp>
#include <weave/futures/combine/par/all.hpp>
#include <weave/futures/run/await.hpp>

#include <vector>

namespace cmp_lattice::support {

//! Sends tasks into executor in batches
template <typename Task, typename Executor>
    requires std::is_invocable_r_v<void, Task> && std::is_base_of_v<weave::executors::IExecutor, Executor>
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

    void Flush()
    {
        auto status = weave::futures::no_alloc::All(std::move(batch_)) | weave::futures::ThreadAwait();

        assert(status);

        assert(batch_.empty());
    }
};

namespace detail {

template <typename T>
struct BatcherTraits : public std::false_type
{};

template <typename Task, typename Executor>
    requires std::is_invocable_r_v<void, Task> && std::is_base_of_v<weave::executors::IExecutor, Executor>
struct BatcherTraits<Batcher<Task, Executor>> : public std::true_type
{
    using TaskType = Task;
    using ExecutorType = Executor;
};

} // namespace detail

} // namespace cmp_lattice::support

namespace cmp_lattice::traits {

template <typename T>
concept BatcherInstance = support::detail::BatcherTraits<T>::value;

template <BatcherInstance T>
using BatcherExecutor = typename support::detail::BatcherTraits<T>::ExecutorType;

template <BatcherInstance T>
using BatcherTask = typename support::detail::BatcherTraits<T>::TaskType;

} // namespace cmp_lattice::traits

