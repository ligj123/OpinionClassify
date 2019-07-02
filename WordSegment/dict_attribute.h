#ifndef DICT_ATTRIBUTE_H
#define DICT_ATTRIBUTE_H

namespace word_segment{
enum emCharAttribute : unsigned int
{
	CharUnknown = 0x00000000,	//所有未知字符，及GBK范围外的其他字符，例如他国字符，以及空余位

	CharHan = 0x80000000,	//所有汉字字符，包括汉字标点符号
	CharEnglish = 0x40000000,	//所有英文字符，包括英文标点符号
	CharToWord = 0x20000000,  //可组成词的所有字符，例如字母、数字、和下划线，汉字，点等
	CharPunction = 0x10000000,    //汉字英文中的标点符号

	CharEnToWord = CharEnglish | CharToWord,//可组成词的英文字符，例如字母、数字、和下划线，点等
	CharHanToWord = CharHan | CharToWord,//可组成词的汉文字符

	CharExSegMarker = 0x08000000,//显式切分标记
	CharImSegMarker = 0x04000000,//隐式切分标记

	CharNumber = 0x02000000,	//中英文数字
	CharHanNumber = 0x82000000,	//中文数字
	CharEnNumber = 0x42000000,	//英文数字
	CharSpace = 0x01000000,	//中英文空格,\t制表符

	CharOrdinalIndep = 0x00800000,	//可单独做数字编号的字符
	CharOrdinalDepend = 0x00400000,	//不可单独做做数字编号的字符
	CharOrdinalAuxi = 0x00200000, //不可单独做做数字编号的字符的辅助符号，例如.、（）()
	CharBullets = 0x00100000,	//可做项目符号的字符
	CharAllList = 0x00F00000,

	CharConnector = 0x00080000,//可用于连接同类信息的字符，例如3000~5000元的~
	CharSpliteSymbol = 0x00040000,//连续一行重复用于标记出两个信息项分隔的字符，例如-_－—~～
	CharLabelSymbolDouble = 0x00020000,//两个符号用于标记出前面可能是标签的字符，例如【】[]
	CharLabelSymbolSingle = 0x00010000,//单个符号用于标记出前面可能是标签的字符，例如:：

	CharEnSBCCase = 0x40008000,//英文全角
	CharEnDBCCase = 0x40000000,//英文半角
	CharEnUpperCase = 0x40004000,	//英文大写字母
	CharEnLowCase = 0x40002000,	//英文小写字母

	CharHanMorpheme = 0x80000800,//是否为语素汉字
	CharHanNoMorpheme = 0x80000400,//是否为非语素汉字
	CharHanTraditional = 0x80000200,//是否为繁体汉字
	CharHanPrefix = 0x80000100,//是否可为词的前缀
	CharHanSuffix = 0x80000080,//是否可为词的后缀
	CharHanName = 0x80000040,//中国人名字常用字
	CharHanForeignName = 0x80000020,//外国人译名常用字
	CharHanSurname = 0x80000010,//中国人姓氏用字
	CharHanOrgan = 0x80000008,//中国机构名称常用字
};

enum emWordAttribute : unsigned char
{
	WordNoun = 0x10,    //名词
	WordNounName = 0x11,	//人名
	WordNounAddr = 0x12,	//地名
	WordNounOrg =  0x13,	//组织机构名
	WordNounProper = 0x14,	//其他专名词
	WordNounEndDate = 0x15,   //今;现今;目前;当前;现在;至今
	WordNounAddrSuffixHigh = 0x16,//地名后缀，极少做其他用途
	WordNounAddrSuffixLow = 0x17,//地名后缀，可做其他用途
	WordNounSurnameHigh = 0x18,//中国常用姓氏，基本不做其他用途
	WordNounSurnameMid = 0x19,//中国常用姓氏，即常用于姓氏，也用常用于其他
	WordNounSurnameLow = 0x1A,//中国姓氏，常用其他用途，很少用于姓氏

	WordVerb = 0x20,    //动词
	WordVerbNoObject = 0x21,	//不带宾语动词
	WordVerbNounObject = 0x22,//带名宾动词
	WordVerbVerbObject = 0x23,//带动宾动词
	WordVerbAdjObject = 0x24,	//带形宾动词
	WordVerbSmallObject = 0x25,//带小句宾动词
	WordVerbTwoObject = 0x26,	//带双宾动词
	WordVerbAnnexe = 0x27,	//带兼语动词
	WordVerbAuxiliary = 0x28,	//助动词
	WordVerbTendency = 0x29,	//趋向动词
	WordVerbCopula = 0x2A,	//系动词

	WordAuxiliary = 0x30,	 //助词
	WordAuxiliaryStructure = 0x31,//结构助词
	WordAuxiliaryTense = 0x32,	//时态助词
	WordAuxiliaryMood = 0x33,	//语气助词

	WordTime = 0x40,    //时间词
	WordSpace = 0x48,    //处所词
	WordLocalizer = 0x50,	//方位词
	WordNumber = 0x58,	//数词
	WordQuantity = 0x60,	//量词
	WordDistinguish = 0x68,//区别词
	WordPronoun = 0x70,	//代词
	WordAdjective = 0x78,	 //形容词
	WordDescriptive = 0x80,//状态词
	WordAdverb = 0x88,	 //副词
	WordPreposition = 0x90,//介词
	WordConjunction = 0x98,//连词

	WordOther = 0xA0,     //其他

	WordUnknown = 0x0,     //其他未知
};

enum emWordType : unsigned short
{
	WordTypeNull = 0x0000,    //0

	WordTypeDate = 0x0001,    //日期类型
	WordTypePhone = 0x0003,    //固定电话
	WordTypeMobile = 0x0004,	//手机
	WordTypePostcode = 0x0005,	//邮编
	WordTypeEmail = 0x0006,    //电子信箱
	WordTypeIDCard = 0x00007,	//身份证
	WordTypeHttp = 0x0008,    //网页地址

	WordTypeDict = 0x0010,    //词典中存在的词及词组（包括词典中有的英文的词和词组）
	WordTypeEng = 0x0020,	//英文单词或字母与数字的组合
	WordTypeSingleHan = 0x0030,	//单个非词汉字
	WordTypeNumber = 0x0040,	//数字

	WordTypeNoWordChar = 0x0100,//单个不能组成词的字符
	WordTypeSpace = 0x0200,    //一或多个空格
	WordTypeLineBreak = 0x0300,	//换行符
	WordTypeAllNoWord = 0x0F00,

	WordTypeUnknown = 0xF000,	//当前未识别字符串，需要在下一步进行识别
};

enum emReliability : unsigned char	//可信度
{
	ReliabilityUnknown = 0x00,	//级别未知
	ReliabilityLow = 0x01,    //级别低
	ReliabilityMid = 0x02,    //级别中
	ReliabilityHigh = 0x4,    //级别高
	ReliabilityAffirm = 0x8,	//级别确认
};

}
#endif
