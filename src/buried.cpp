#include <iostream>

#include <spdlog/spdlog.h>

int BuriedTest()
{
    std::cout << "Buried Test.";

    // 设置日志级别为DEBUG
    spdlog::set_level(spdlog::level::debug);

    // 输出不同级别的日志
    spdlog::debug("This is a debug message");
    spdlog::info("This is an info message");
    spdlog::warn("This is a warning message");
    spdlog::error("This is an error message");

    return 1;
}
