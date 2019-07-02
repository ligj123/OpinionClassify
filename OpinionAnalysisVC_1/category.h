#ifndef CATEGORY_H
#define CATEGORY_H

namespace opinion_analysis{
//以下文本分类
/*enum Category : unsigned int
{
    tcUnknown =           0x00000000,

    tcEnvironment = 1,      //环境
    tcComputer =    2,      //计算机
    tcTraffic =    3,      //交通
    tcEducation =   4,     //教育
    tcEconamy =   5,     //经济
    tcMilitary =   6,     //军事
    tcSports =    7,      //体育

    tcMedicine =     8,     //医药
    tcArt =     9,     //艺术
    tcPolitics =   10,      //政治
};

const Category arCategory[] = { tcEnvironment, tcComputer, tcTraffic, tcEducation, tcEconamy, tcMilitary, tcSports,
								tcMedicine, tcArt, tcPolitics};
*/

enum Category : unsigned int
{
	tcUnknown = 0x00000000,

	tcMilitary = 1,	//军事
	tcHealth = 2,	//健康
	tcEconamy = 3,	//财经
	tcHouse = 4,	//房产
	tcCar = 5,		//汽车
	tcEducation = 6,//教育
	tcSports = 7,	//体育
};

const Category arCategory[] = { tcMilitary, tcHealth, tcEconamy, tcHouse, tcCar, tcEducation, tcSports };
}

#endif  //CATEGORY_H
