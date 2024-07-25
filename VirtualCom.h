#ifndef VIRTUALCOM_H
#define VIRTUALCOM_H

#include <string>
#include <stdint.h>

class VirtualCom
{
public:
    /**
    * �������⴮�ڶ�
    * ÿһ�����⴮���ڴ���ʱ���Զ��������
    **/
    struct CommPairType {
        uint8_t index;
        std::wstring comA;
        std::wstring comB;
    };

    //��������ʽִ��һ�����������ִ�н��
    uint8_t execReadBack(const std::wstring& cmd, char* read_buff, size_t buff_len);

    //��װ���⴮������
    uint8_t installDriver();

    //�������⴮���б�
    void loadCommPairs(CommPairType* commPairs, size_t* numOfPairs);

    //����һ���µ����⴮��
    void createComPair(const std::wstring& comportA, const std::wstring& comportB);

    //ɾ��ָ����һ�����⴮��
    void removeComPair(uint8_t pairIndex);

protected:

private:
    //���ÿ�������
    VirtualCom();
    ~VirtualCom();
    VirtualCom(const VirtualCom&);
    void operator=(VirtualCom&);
};

#endif // VIRTUALCOM_H
