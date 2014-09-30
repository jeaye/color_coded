#pragma once

#include <thread>
#include <future>
#include <atomic>
#include <chrono>
#include <vector>

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
        ~queue()
        {
          should_work_.store(false);
          thread_.join(); /* TODO: seem to've locked here once */
        }

        void push(Task &&t)
        {
          std::lock_guard<std::mutex> const lock{ task_mutex_ };
          task_ = std::move(t);
          has_work_.store(true);
        }
        std::pair<Result, bool> pull()
        {
          std::lock_guard<std::mutex> const lock{ result_mutex_ };
          return { result_, has_result_.load() };
        }

      private:
        void work()
        {
          Task curr;
          Result result;
          while(should_work_.load())
          {
            if(!has_work_.load())
            {
              std::this_thread::sleep_for(std::chrono::milliseconds{ 500 });
              continue;
            }

            {
              std::lock_guard<std::mutex> const lock{ task_mutex_ };
              curr = std::move(task_);
              has_work_.store(false);
            }

            result = func_(curr);

            {
              std::lock_guard<std::mutex> const lock{ result_mutex_ };
              result_ = std::move(result);
              has_result_.store(true);
            }
          }
        }

        func_t const func_;
        std::thread thread_;
        std::atomic_bool should_work_{ true };

        std::atomic_bool has_work_{};
        Task task_;
        std::mutex task_mutex_;

        std::atomic_bool has_result_{};
        Result result_;
        std::mutex result_mutex_;
    };
  }
}
