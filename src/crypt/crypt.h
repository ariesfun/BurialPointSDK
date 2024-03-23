#pragma once
#include <memory>
#include <string>

namespace buried
{
    // ����ʵ�ּӽ��ܵ�ģ��
    class Crypt {
    public:
        virtual ~Crypt() = default;

        virtual std::string Encrypt(const std::string& input) = 0; // ����

        virtual std::string Decrypt(const std::string& input) = 0; // ����

        virtual std::string Encrypt(const void* input, size_t input_size) = 0;

        virtual std::string Decrypt(const void* input, size_t input_size) = 0;

    };

    class AESImpl;
    class AESCrypt:public Crypt {
    public:
        // ��ʾ���캯������������ʽ����ת��
        explicit AESCrypt(const std::string& key);                                      // ������ʾAES�����������Կ

        ~AESCrypt();

        AESCrypt(const AESCrypt& other) = delete;                                       // ��ֹ��������

        AESCrypt& operator=(const AESCrypt& other) = delete;                            // ��ֹ��ֵ������

    public:
        static std::string GetKey(const std::string& salt, const std::string password); // ����ֵ������õ�Key

        std::string Encrypt(const std::string& input) override;

        std::string Decrypt(const std::string& input) override;

        std::string Encrypt(const void* input, size_t input_size) override;

        std::string Decrypt(const void* input, size_t input_size) override;

    private:
        std::unique_ptr<AESImpl> impl_; // pimpl��д��

    };

    
} // namespace buried
