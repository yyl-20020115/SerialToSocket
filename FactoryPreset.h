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
    const std::wstring app_exe_path;          //��ִ�г���ľ���·��
    const std::wstring app_root_dir;            //Ӧ�ó����Ŀ¼
    const std::wstring cfg_file_path;           //�����ļ����·��
    std::wstring vcom_driver_path;              //�ֱ��Ӧ32λ��64λ������
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
