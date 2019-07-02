#ifndef CATEGORY_H
#define CATEGORY_H

namespace opinion_analysis{
//�����ı�����
//enum Category : unsigned int
//{
//    tcUnknown =           0x00000000,
//
//    tcGovLawEnforcement = 0x10000001,      //��������ִ��
//    tcGovMalfeasance =    0x10000002,      //����ְȨӦ��
//    tcGovPolicyIssue =    0x10000004,      //������������
//    tcGovEconomyIssue =   0x10000008,     //����������������
//    tcGovLeaderSpeech =   0x10000010,     //�����쵼����
//    tcGovAppointments =   0x10000020,     //������Ա����
//    tcGovPrivateLife =    0x10000040,      //����Ա˽����
//
//    tcSoeComplaints =     0x20000080,     //����������Ͷ��
//    tcSoeManagement =     0x20000100,     //����Ӫ����
//    tcSoeEconomyIssue =   0x20000008,      //���󾭼�����
//    tcSoeAppointments =   0x20000020,      //������������
//    tcSoePrivateLise =    0x20000040,     //����˽��������
//
//    tcPriComplaints =     0x30000080,     //˽��������Ͷ��
//    tcPriManagement =     0x30000100,     //˽�����Ӫ����
//    tcPriEconomyIssue =   0x30000008,      //˽����󾭼�����
//    tcPriPrivateLise =    0x30000040,     //����˽��������
//};
//
//const Category arCategory[] = { tcGovLawEnforcement, tcGovMalfeasance, tcGovPolicyIssue, tcGovEconomyIssue,
//                                        tcGovLeaderSpeech, tcGovAppointments, tcGovPrivateLife,
//
//                                        tcSoeComplaints, tcSoeManagement, tcSoeEconomyIssue, tcSoeAppointments, tcSoePrivateLise,
//
//                                        tcPriComplaints, tcPriManagement, tcPriEconomyIssue, tcPriPrivateLise};
//

//enum Category : unsigned int
//{
//    tcUnknown =         0x00000000,
//    tcEnvironment =     0x10000001,
//    tcComputer =        0x10000002,
//    tcTraffic =         0x10000003,
//    tcEducation =       0x10000004,
//    tcEconomics =       0x10000005,
//    tcMulitary =        0x10000006,
//    tcSports =          0x10000007,
//    tcMedicine =        0x10000008,
//    tcArts =            0x10000009,
//    tcPolitics =        0x1000000A,
//};
//
//const Category arCategory[] = {
//    tcEnvironment,
//    tcComputer,
//    tcTraffic,
//    tcEducation,
//    tcEconomics,
//    tcMulitary,
//    tcSports,
//    tcMedicine,
//    tcArts,
//    tcPolitics
//};

enum Category : unsigned int
{
    tcUnknown =         0x00000000,
    tcOpinion =     0x10000001,
};
    
const Category arCategory[] = {
    tcOpinion,
};
const char* cArCategory[];
}

#endif  //CATEGORY_H
