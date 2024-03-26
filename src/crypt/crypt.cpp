#include "crypt.h"
#include <string>

#include "module/mbedtls-3.5.0/include/mbedtls/cipher.h"
#include "module/mbedtls-3.5.0/include/mbedtls/error.h"
#include "module/mbedtls-3.5.0/include/mbedtls/md.h"
#include "module/mbedtls-3.5.0/include/mbedtls/pkcs5.h"

namespace buried
{
    class AESImpl {
    public:
        explicit AESImpl(const std::string& key)
        {
            Init(key.data(), key.size());
        }

        AESImpl(const AESImpl& other) = delete;
        AESImpl& operator=(const AESImpl& other) = delete;
        ~AESImpl()
        {
            UnInit();
        }

        // 用于初始化加密算法模块，接受一个密钥key和密钥的长度key_size作为参数
        void Init(const char* key, size_t key_size);

        // 用于释放加密算法模块的资源
        void UnInit();

        // 用于对输入数据进行加密
        std::string Encrypt(const void* input, size_t input_size);

        // 
        std::string Decrypt(const void* input, size_t input_size);

    private:
        mbedtls_cipher_context_t encrypt_ctx_;
        mbedtls_cipher_context_t decrypt_ctx_;

        uint32_t encrypt_block_size_ = 0;
        uint32_t decrypt_block_size_ = 0;

        unsigned char iv_[16] = {0};
    };

    void AESImpl::Init(const char* key, size_t key_size)
    {
        mbedtls_cipher_init(&encrypt_ctx_);                                                                                // 初始化加密上下文
        mbedtls_cipher_setup(&encrypt_ctx_, mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_256_CBC));                    // 配置加密上下文，指定加密算法类型为AES-256-CBC
        mbedtls_cipher_set_padding_mode(&encrypt_ctx_, MBEDTLS_PADDING_PKCS7);                                             // 设置加密上下文的填充模式为PKCS7
        mbedtls_cipher_setkey(&encrypt_ctx_, reinterpret_cast<const unsigned char*>(key), key_size * 8, MBEDTLS_ENCRYPT);  // 设置加密上下文的密钥，将key转换为无符号字符指针类型，并指定密钥的位数为key_size*8
        encrypt_block_size_ = mbedtls_cipher_get_block_size(&encrypt_ctx_);                                                // 获取加密上下文的快打下，并将其保存到encrypt_block_size_中

        mbedtls_cipher_init(&decrypt_ctx_);
        mbedtls_cipher_setup(&decrypt_ctx_, mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_256_CBC));
        mbedtls_cipher_set_padding_mode(&decrypt_ctx_, MBEDTLS_PADDING_PKCS7);
        mbedtls_cipher_setkey(&decrypt_ctx_, reinterpret_cast<const unsigned char*>(key), key_size * 8, MBEDTLS_DECRYPT);
        decrypt_block_size_ = mbedtls_cipher_get_block_size(&decrypt_ctx_);

    }

    void AESImpl::UnInit()
    {
        // 调用 mbedtls_cipher_free 释放加密上下文
        mbedtls_cipher_free(&encrypt_ctx_);
        mbedtls_cipher_free(&decrypt_ctx_);
    }

    std::string AESImpl::Encrypt(const void* input, size_t input_size)
    {
        // 设置加密上下文encrypt_ctx_的初始化向量（IV）为全零的16字节
        mbedtls_cipher_set_iv(&encrypt_ctx_, iv_, sizeof(iv_));
        // 重置加密上下文的状态
        mbedtls_cipher_reset(&encrypt_ctx_);
        // 创建一个输出字符串output, 大小为输入数据的大小加上加密块的大小
        std::string output(input_size + encrypt_block_size_, 0);
        size_t olen = 0;
        // 调用mbedtls_cipher_update，对输入数据进行加密处理，并将加密结果存储到output中
        int ret = mbedtls_cipher_update(
            &encrypt_ctx_, reinterpret_cast<const unsigned char*>(input), input_size,
            reinterpret_cast<unsigned char*>(output.data()), &olen);

        if(ret != 0)
        {
            return "";
        }

        size_t olen2;
        // 调用mbedtls_cipher_finish完成加密操作，并将最后一部分加密结果追加到output中
        ret = mbedtls_cipher_finish(
            &encrypt_ctx_, reinterpret_cast<unsigned char*>(output.data()) + olen, &olen2);
        if(ret != 0)
        {
            return "";
        }
        // 调整output的大小为实际加密结果的大小，并返回加密结果
        output.resize(olen + olen2);
        return output;

    }

    std::string AESImpl::Decrypt(const void* input, size_t input_size)
    {
        // 调用 mbedtls_cipher_set_iv 设定解密上下文的初始向量为iv_ 
        mbedtls_cipher_set_iv(&decrypt_ctx_, iv_, sizeof(iv_));
        // 重置解密上下文
        mbedtls_cipher_reset(&decrypt_ctx_);
        // 创建一个初始大小为 input_size + decrypt_block_size_ 的输出字符串output
        std::string output(input_size + decrypt_block_size_, 0);
        size_t olen = 0;
        // 调用 mbedtls_cipher_update 对数据进行解密操作，并将解密后的数据存储到output中
        int ret = mbedtls_cipher_update(
            &decrypt_ctx_, reinterpret_cast<const unsigned char*>(input), input_size,
            reinterpret_cast<unsigned char*>(output.data()), &olen);

        // 检查解密字符串是否成功，如果失败则返回空字符串
        if(ret != 0)
        {
            return "";
        }

        size_t olen2;
        // 调用 mbedtls_cipher_finish 完成解密操作，并将解密后的数据存储到output中
        ret = mbedtls_cipher_finish(
            &decrypt_ctx_, reinterpret_cast<unsigned char*>(output.data()) + olen, &olen2);
        if(ret != 0)
        {
            return "";
        }
        // 重新调整output的大小，将其截断为正确的长度，最后返回解密后的数据
        output.resize(olen + olen2);
        return output;
    }

    AESCrypt::AESCrypt(const std::string& key) : impl_(std::make_unique<AESImpl>(key))
    {
        
    }

    AESCrypt::~AESCrypt() {}

    std::string AESCrypt::GetKey(const std::string& salt, const std::string password)
    {
        // 结合盐值和用户提供的password相结合，使用PBKDF2算法和HMAC-SHA256哈希算法来生成密钥
        int32_t keylen = 32;
        uint32_t iterations = 1000;
        unsigned char key[32] = {0}; // 生成一个32字节（256位）的密钥
        mbedtls_md_context_t md_ctx;
        mbedtls_md_init(&md_ctx);
        const mbedtls_md_info_t* md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
        mbedtls_md_setup(&md_ctx, md_info, 1);
        mbedtls_md_starts(&md_ctx);

        // 生成一个加密安全的密钥
        int ret = mbedtls_pkcs5_pbkdf2_hmac(
            &md_ctx, reinterpret_cast<const unsigned char*>(password.data()),
            password.size(), reinterpret_cast<const unsigned char*>(salt.data()),
            salt.size(), iterations, keylen, key
        );

        mbedtls_md_free(&md_ctx);
        if(ret != 0)
        {
            return "";
        }
        return std::string((char*)key, keylen); // 将生成的密钥转换为std::string类型并返回

    }

    std::string AESCrypt::Encrypt(const std::string& input)
    {
        return impl_->Encrypt(input.data(), input.size());
    }

    std::string AESCrypt::Decrypt(const std::string& input)
    {
        return impl_->Decrypt(input.data(), input.size());
    }

    std::string AESCrypt::Encrypt(const void* input, size_t input_size)
    {
        return impl_->Encrypt(input, input_size);
    }

    std::string AESCrypt::Decrypt(const void* input, size_t input_size)
    {
        return impl_->Decrypt(input, input_size);
    }

} // namespace buried
