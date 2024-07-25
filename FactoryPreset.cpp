#include "pch.h"
#include "FactoryPreset.h"
#include <iostream>
#include <filesystem>
#include "winicpp.hpp"

FactoryPreset FactoryPreset::Instance;

FactoryPreset& FactoryPreset::GetInstance()
{
    return Instance;
}

FactoryPreset::FactoryPreset() :
    app_exe_path(std::filesystem::current_path()),       //获取可执行程序路径
    app_root_dir(std::filesystem::path(std::filesystem::current_path()).parent_path()),
    cfg_file_path(app_root_dir + L"config.ini")                        //配置文件路径
{
    this->LoadConfig();

    SYSTEM_INFO si = { 0 };
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
        si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
        vcom_driver_path = app_root_dir + L"com0com_x64\\setupc.exe";
    }
    else {
        vcom_driver_path = app_root_dir + L"com0com_x86\\setupc.exe";
    }

    //---------------------------------------------
    if (!std::filesystem::exists(cfg_file_path))
    {
        this->SaveConfig();
    }
}

void FactoryPreset::SaveConfig() const
{
    std::wfstream outStream(cfg_file_path);

    if (outStream) {
        outStream << L"Host=" << this->host << std::endl;
        outStream << L"Port=" << this->port << std::endl;
        outStream << L"Com=" << this->com << std::endl;
        outStream << L"BaudRate=" << this->baudrate << std::endl;
        outStream << L"Databits=" << this->databits << std::endl;
        outStream << L"Parity=" << this->parity << std::endl;
        outStream.close();
    }
}

void FactoryPreset::LoadConfig()
{
    if (std::filesystem::exists(this->cfg_file_path)) {
        winicpp::IniManager manager(cfg_file_path);

        this->host = manager[L""].getValue(L"host");
        this->port = manager[L""].toInt(L"port");
        this->com = manager[L""].getValue(L"com");
        this->baudrate = manager[L""].toInt(L"baudrate");
        this->databits = manager[L""].toInt(L"databits");
        this->parity = manager[L""].getValue(L"parity");
        this->stopbits = manager[L""].getValue(L"stopbits");
    }
}

FactoryPreset::~FactoryPreset()
{

}
