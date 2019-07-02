#ifndef CATEGORY_H
#define CATEGORY_H

namespace opinion_analysis{
//�����ı�����
/*enum Category : unsigned int
{
    tcUnknown =           0x00000000,

    tcEnvironment = 1,      //����
    tcComputer =    2,      //�����
    tcTraffic =    3,      //��ͨ
    tcEducation =   4,     //����
    tcEconamy =   5,     //����
    tcMilitary =   6,     //����
    tcSports =    7,      //����

    tcMedicine =     8,     //ҽҩ
    tcArt =     9,     //����
    tcPolitics =   10,      //����
};

const Category arCategory[] = { tcEnvironment, tcComputer, tcTraffic, tcEducation, tcEconamy, tcMilitary, tcSports,
								tcMedicine, tcArt, tcPolitics};
*/

enum Category : unsigned int
{
	tcUnknown = 0x00000000,

	tcMilitary = 1,	//����
	tcHealth = 2,	//����
	tcEconamy = 3,	//�ƾ�
	tcHouse = 4,	//����
	tcCar = 5,		//����
	tcEducation = 6,//����
	tcSports = 7,	//����
};

const Category arCategory[] = { tcMilitary, tcHealth, tcEconamy, tcHouse, tcCar, tcEducation, tcSports };
}

#endif  //CATEGORY_H
