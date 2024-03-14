#pragma once
#include <stdint.h> // ��ȷָ���������͵Ĵ�С

#define BURIED_EXPORT __declspec(dllexport) // ����ָʾ�������������������������̬���ӿ⣨DLL����

// ʹ�� extern "C" ����������ȷ������������ C �ĵ���Լ�������� C++ �ĵ���Լ����������Щ����
// �Ӷ�ʹ�����ǿ����ڲ�ͬ�ı������֮����н���
extern "C"
{
    BURIED_EXPORT int BuriedTest(); 

    typedef struct Buried Buried;

    // ������Ⱪ¶��C�ӿ�,�������������Խ��н���
    struct BuriedConfig
    {
        const char* host;
        const char* topic;
        const char* user_id;
        const char* app_version;
        const char* app_name;
        const char* custom_data;
    };

    BURIED_EXPORT Buried* Buried_Create(const char* work_dir);

    BURIED_EXPORT void Buried_Destory(Buried* buried);

    BURIED_EXPORT int32_t Buried_Start(Buried* buried, BuriedConfig* config);

    BURIED_EXPORT int32_t Buried_Report(Buried* buried, const char* title, const char* report_data, uint32_t priority);

}
