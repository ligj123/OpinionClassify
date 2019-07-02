#ifndef CATEGORY_H
#define CATEGORY_H

namespace opinion_analysis{
//以下文本分类
//enum Category : unsigned int
//{
//    tcUnknown =           0x00000000,
//
//    tcGovLawEnforcement = 0x10000001,      //政府暴力执法
//    tcGovMalfeasance =    0x10000002,      //政府职权应用
//    tcGovPolicyIssue =    0x10000004,      //政府政策问题
//    tcGovEconomyIssue =   0x10000008,     //政府廉政经济问题
//    tcGovLeaderSpeech =   0x10000010,     //政府领导言论
//    tcGovAppointments =   0x10000020,     //政府官员任命
//    tcGovPrivateLife =    0x10000040,      //公务员私生活
//
//    tcSoeComplaints =     0x20000080,     //国企消费者投诉
//    tcSoeManagement =     0x20000100,     //国企经营问题
//    tcSoeEconomyIssue =   0x20000008,      //国企经济问题
//    tcSoeAppointments =   0x20000020,      //国企人事任命
//    tcSoePrivateLise =    0x20000040,     //国企私生活问题
//
//    tcPriComplaints =     0x30000080,     //私企消费者投诉
//    tcPriManagement =     0x30000100,     //私企国企经营问题
//    tcPriEconomyIssue =   0x30000008,      //私企国企经济问题
//    tcPriPrivateLise =    0x30000040,     //国企私生活问题
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
