#pragma once
#include <memory>
#include <string>

namespace buried
{
    // 用于实现加解密的模块
    class Crypt {
    public:
        virtual ~Crypt() = default;

        virtual std::string Encrypt(const std::string& input) = 0; // 加密

        virtual std::string Decrypt(const std::string& input) = 0; // 解密

        virtual std::string Encrypt(const void* input, size_t input_size) = 0;

        virtual std::string Decrypt(const void* input, size_t input_size) = 0;

    };

    class AESImpl;
    class AESCrypt:public Crypt {
    public:
        // 显示构造函数，不允许隐式类型转化
        explicit AESCrypt(const std::string& key);                                      // 参数表示AES加密所需的密钥

        ~AESCrypt();

        AESCrypt(const AESCrypt& other) = delete;                                       // 禁止拷贝构造

        AESCrypt& operator=(const AESCrypt& other) = delete;                            // 禁止赋值操作符

    public:
        static std::string GetKey(const std::string& salt, const std::string password); // 由盐值和密码得到Key

        std::string Encrypt(const std::string& input) override;

        std::string Decrypt(const std::string& input) override;

        std::string Encrypt(const void* input, size_t input_size) override;

        std::string Decrypt(const void* input, size_t input_size) override;

    private:
        std::unique_ptr<AESImpl> impl_; // pimpl的写法

    };

    
} // namespace buried
