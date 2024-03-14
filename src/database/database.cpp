#include "database/database.h"

#include "module/sqlite/sqlite_orm.h"

using namespace sqlite_orm;
namespace buried
{
    inline auto InitStorage(const std::string& path)
    {
        return make_storage(
            path, make_table("buried_data",
                            make_column("id", &BuriedDB::Data::id, primary_key().autoincrement()),
                            make_column("priority", &BuriedDB::Data::priority),
                            make_column("timestamp", &BuriedDB::Data::timestamp),
                            make_column("content", &BuriedDB::Data::content)
                            )
        );
    }


    class BuriedDBImpl {
    public:
        using DBStorage = decltype(InitStorage(""));

    public:
        BuriedDBImpl(std::string db_path) : db_path_(db_path_)
        {
            storage_ = std::make_unique<DBStorage>(InitStorage(db_path_));
            storage_->sync_schema();
        }

        ~BuriedDBImpl() {}

        void InsertData(const BuriedDB::Data& data)
        {
            auto guard = storage_->transaction_guard();
            storage_->insert(data);
            guard.commit();
        }

        void DeleteData(const BuriedDB::Data& data)
        {
            auto guard = storage_->transaction_guard();
            storage_->remove_all<BuriedDB::Data>(
                where(c(&BuriedDB::Data::id) == data.id)
            );
            guard.commit();
        }

        void DeleteDatas(const std::vector<BuriedDB::Data>& datas)
        {
            auto guard = storage_->transaction_guard();
            for (const auto& data : datas)
            {
                storage_->remove_all<BuriedDB::Data>(
                    where(c(&BuriedDB::Data::id) == data.id)
                );
            }
            guard.commit();
        }

        std::vector<BuriedDB::Data> QueryData(int32_t limit_size)
        {
            auto limited = storage_->get_all<BuriedDB::Data>(
                order_by(&BuriedDB::Data::priority).desc(), limit(limit_size)
            );
            return limited;
        }

    private:
        std::string db_path_;                   // 数据库的存放路径

        std::unique_ptr<DBStorage> storage_;    
    };


    // BuriedDB 类成员函数的实现
    BuriedDB::BuriedDB(std::string db_path) : impl_{std::make_unique<BuriedDBImpl>(std::move(db_path))}
    {
    }

    BuriedDB::~BuriedDB() {}

    void BuriedDB::InsertData(const Data& data)
    {
        impl_->InsertData(data);
    }

    void BuriedDB::DeleteData(const Data& data)
    {
        impl_->DeleteData(data);
    }

    void BuriedDB::DeleteDatas(const std::vector<Data>& datas)
    {
        impl_->DeleteDatas(datas);
    }

    std::vector<BuriedDB::Data> BuriedDB::QueryData(int32_t limit)
    {
        return impl_->QueryData(limit);
    }

} // namespace buried


