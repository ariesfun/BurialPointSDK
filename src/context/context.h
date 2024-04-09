#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "src/module/boost/asio/io_context.hpp"
#include "src/module/boost/asio/io_context_strand.hpp"

namespace buried
{
    // 让线程统一在Context内部管理
    class Context {
    public:
        static Context& GetGlobalContext()  // 单例模式
        {
            static Context global_context;
            return global_context;
        }

        ~Context();

        // 使用strand模型：顺序的任务队列（保证多个任务在同一个线程上按序执行，而且能避免数据竞争）
        // 创建strand对象，并使用 boost::asio::post() 将任务提交到strand中，来进行任务的调度
        using Strand = boost::asio::io_context::strand; 
        // 运行时环境
        using IOContext = boost::asio::io_context;

        // 在埋点项目中，mainStrand用于处理大多数主要逻辑，reportStrand用于执行网络相关的任务
        Strand& GetMainStrand() { return main_strand_; }
        Strand& GetReportStrand() { return report_strand_; }

        IOContext& GetMainContext() { return main_context_; }

        // Start后，可以拿到不同的strand，不同的strand可以理解为不同的线程，通过不同的strand执行任务就相当于在某个线程执行任务
        void Start();

    private:
        Context() : main_strand_(main_context_), report_strand_(report_context_) {}

        Context(const Context&) = delete;

        Context& operator=(const Context&) = delete;

    private:
        boost::asio::io_context main_context_;
        boost::asio::io_context report_context_;

        boost::asio::io_context::strand main_strand_;
        boost::asio::io_context::strand report_strand_;

        std::unique_ptr<std::thread> main_thread_;
        std::unique_ptr<std::thread> report_thread_;

        std::atomic<bool> is_start_{false};
        std::atomic<bool> is_stop_{false};

    };

} // namespace buried

