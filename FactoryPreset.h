#pragma once
#ifndef FACTORYPRESET_H
#define FACTORYPRESET_H
#include <string>

using namespace std;

class FactoryPreset
{
public:
    static FactoryPreset& GetInstance();

protected:
    static FactoryPreset Instance;
public:
    FactoryPreset();
    ~FactoryPreset();

public:
    const std::wstring app_exe_path;          //可执行程序的绝对路径
    const std::wstring app_root_dir;            //应用程序根目录
    const std::wstring cfg_file_path;           //配置文件存放路径
    std::wstring vcom_driver_path;              //分别对应32位或64位的驱动
public:
    void SaveConfig() const;      
    void LoadConfig();
public:
    std::wstring host = L"127.0.0.1";
    int port = 8888;
    std::wstring com = L"COM1";
    int baudrate = 9600;
    int databits = 8;
    std::wstring parity = L"None";
    std::wstring stopbits = L"1";
protected:

};

#endif // FACTORYPRESET_H
