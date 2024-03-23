// 用于测试加解密模块的功能

#include "gtest/gtest.h"
#include "src/crypt/crypt.h"
#include <string>

TEST(CryptTest, BasicTest)
{
    std::string key = buried::AESCrypt::GetKey("salt", "password");
    EXPECT_TRUE(!key.empty());

    buried::Crypt* crypt = new buried::AESCrypt(key);
    std::string input = "hello world";
    std::string encrypted = crypt->Encrypt(input);      // 进行加密
    EXPECT_TRUE(!encrypted.empty());

    std::string decrypted = crypt->Decrypt(encrypted);  // 进行解密
    EXPECT_TRUE(!decrypted.empty());
    EXPECT_EQ(input, decrypted);                        // 检验原串和解密后的字符内容是否一致
    delete crypt;
}