// ���ڲ��Լӽ���ģ��Ĺ���

#include "gtest/gtest.h"
#include "src/crypt/crypt.h"
#include <string>

TEST(CryptTest, BasicTest)
{
    std::string key = buried::AESCrypt::GetKey("salt", "password");
    EXPECT_TRUE(!key.empty());

    buried::Crypt* crypt = new buried::AESCrypt(key);
    std::string input = "hello world";
    std::string encrypted = crypt->Encrypt(input);      // ���м���
    EXPECT_TRUE(!encrypted.empty());

    std::string decrypted = crypt->Decrypt(encrypted);  // ���н���
    EXPECT_TRUE(!decrypted.empty());
    EXPECT_EQ(input, decrypted);                        // ����ԭ���ͽ��ܺ���ַ������Ƿ�һ��
    delete crypt;
}