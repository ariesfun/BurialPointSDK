#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

namespace buried
{
    class BuriedDBImpl;
    class BuriedDB {
    public:
        struct Data
        {
            int32_t id;
            int32_t priority;
            uint64_t timestamp;
            std::vector<char> content;
        };
        
    public:
        BuriedDB(std::string db_path);
        ~BuriedDB();

        void InsertData(const Data& data);
        void DeleteData(const Data& data);
        void DeleteDatas(const std::vector<Data>& datas);
        std::vector<Data> QueryData(int32_t limit);
    
    private:
        std::unique_ptr<BuriedDBImpl> impl_;
    };  
    
} // namespace buried

