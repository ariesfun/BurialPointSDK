// 多线程任务处理，调度示例

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include "src/context/context.h"

std::mutex coutMutex; // 添加一个互斥锁来保护 std::cout

int main()
{
    buried::Context::GetGlobalContext().Start();

    buried::Context::GetGlobalContext().GetMainStrand().post([]()
    {
        std::lock_guard<std::mutex> lock(coutMutex); // 使用互斥锁：以确保同一时间只有一个线程能够访问 std::cout
        std::cout << "Operator 1 executed in strand1 on thread id " 
                  << std::this_thread::get_id() << std::endl;
    }
    );

    buried::Context::GetGlobalContext().GetReportStrand().post([]()
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Operator 2 executed in strand2 on thread id " 
                  << std::this_thread::get_id() << std::endl;


        buried::Context::GetGlobalContext().GetReportStrand().post([]()
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Operator 3 executed in strand2 on thread id " 
                      << std::this_thread::get_id() << std::endl;
        }
        );

        buried::Context::GetGlobalContext().GetMainStrand().post([]()
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Operator 4 executed in strand1 on thread id " 
                      << std::this_thread::get_id() << std::endl;
        }
        );

        buried::Context::GetGlobalContext().GetReportStrand().post([]()
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Operator 5 executed in strand2 on thread id " 
                      << std::this_thread::get_id() << std::endl;
        }
        );
    }
    );

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
