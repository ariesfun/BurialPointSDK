#include <filesystem>

#include "gtest/gtest.h"
#include "src/database/database.h"

TEST(DbTest, Test1) {
  std::filesystem::path db_path("hello_world.db");

  if (std::filesystem::exists(db_path)) {
    std::filesystem::remove(db_path);
  }

  buried::BuriedDB db(db_path.string());
  {
    // 第四个参数类型是 std::vector<char>
    buried::BuriedDB::Data data{-1, 1, 2, std::vector<char>{'h', 'e', 'l', 'l', 'o', '1'}};
    db.InsertData(data);
  }
  auto datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 1);
  {
    buried::BuriedDB::Data data{-1, 2, 3, std::vector<char>{'h', 'e', 'l', 'l', 'o', '2'}};
    db.InsertData(data);
  }
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 2);
  {
    buried::BuriedDB::Data data{-1, 3, 4, std::vector<char>{'h', 'e', 'l', 'l', 'o', '3'}};
    db.InsertData(data);
  }
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 3);
  {
    buried::BuriedDB::Data data{-1, 4, 5, std::vector<char>{'h', 'e', 'l', 'l', 'o', '4'}};
    db.InsertData(data);
  }
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 4);
  EXPECT_EQ(datas[0].priority, 4);
  EXPECT_EQ(datas[1].priority, 3);
  EXPECT_EQ(datas[2].priority, 2);
  EXPECT_EQ(datas[3].priority, 1);

  db.DeleteData(datas[0]);
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 3);

  for (int i = 0; i < 100; ++i) {
    uint64_t value = static_cast<uint64_t>(i); // int turn to uint64_t, for fix some warnings
    buried::BuriedDB::Data data{-1, i, value, std::vector<char>{'h', 'e', 'l', 'l', 'o'}};
    db.InsertData(data);
  }

  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 10);

  // remove db_path
  std::filesystem::remove(db_path);
}