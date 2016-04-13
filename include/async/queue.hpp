#pragma once

#include <string>
#include <map>
#include <thread>
#include <future>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include <boost/optional.hpp>

namespace color_coded
{
  namespace async
  {
    template <typename Task, typename Result>
    class queue
    {
      public:
        using func_t = std::function<Result (Task const &)>;

        queue() = delete;
        queue(func_t &&f)
          : func_{ std::move(f) }
          , thread_{ std::bind(&queue::work, this) }
        { }

        void push(Task &&t)
        {
          {
            /* Allows multiple tasks to override each other; only considers
             * the most recently pushed task. */
            std::lock_guard<std::mutex> const lock{ task_mutex_ };
            task_ = std::move(t);
            wake_up_.store(true);
          }
          task_cv_.notify_one();
        }

        boost::optional<Result> pull(std::string const &name)
        {
          std::lock_guard<std::mutex> const lock{ results_mutex_ };
          if(has_results_[name].exchange(false))
          { return { std::move(results_[name]) }; }
          else
          { return {}; }
        }

        void join()
        {
          should_work_.store(false);
          {
            /* Even if the shared variable is atomic, it must be modified under
             * the mutex in order to correctly publish the modification to the
             * waiting thread. */
            std::lock_guard<std::mutex> const lock{ task_mutex_ };
            wake_up_.store(true);
          }
          task_cv_.notify_one();
          thread_.join();
        }

      private:
        void work()
        {
          Task curr;
          Result result;
          while(should_work_.load())
          {
            {
              std::unique_lock<std::mutex> lock{ task_mutex_ };
              task_cv_.wait(lock, [&]{ return wake_up_.load(); });

              /* We may be woken up to die. */
              if(!should_work_.load())
              { return; }

              curr = std::move(task_);
              wake_up_.store(false);
            }

            result = func_(curr);

            {
              std::lock_guard<std::mutex> const lock{ results_mutex_ };
              has_results_[result.name].store(true);
              results_[result.name] = std::move(result);
            }
          }
        }

        func_t const func_;
        std::thread thread_;
        std::atomic_bool should_work_{ true };

        std::atomic_bool wake_up_{};
        Task task_;
        std::mutex task_mutex_;
        std::condition_variable task_cv_;

        std::map<std::string, std::atomic_bool> has_results_{};
        std::map<std::string, Result> results_;
        std::mutex results_mutex_;
    };
  }
}
