#include "dict_attr.h"

namespace word_segment {

wstring DictAttr::CharAttributeToWStr(emCharAttribute charAttr)
{
	wstring str;
	switch(charAttr)
	{
	case CharUnknown:
        str = L"CharUnknown";
        break;
	case CharHan:
        str = L"CharHan";
        break;
	case CharEnglish:
        str = L"CharEnglish";
        break;
	case CharToWord:
        str = L"CharToWord";
        break;
	case CharPunction:
        str = L"CharPunction";
        break;
	case CharEnToWord:
        str = L"CharEnToWord";
        break;
	case CharHanToWord:
        str = L"CharHanToWord";
        break;
	case CharExSegMarker:
        str = L"CharExSegMarker";
        break;
	case CharImSegMarker:
        str = L"CharImSegMarker";
        break;
	case CharNumber:
        str = L"CharNumber";
        break;
	case CharHanNumber:
        str = L"CharHanNumber";
        break;
	case CharEnNumber:
        str = L"CharEnNumber";
        break;
	case CharSpace:
        str = L"CharSpace";
        break;
	case CharOrdinalIndep:
        str = L"CharOrdinalIndep";
        break;
	case CharOrdinalDepend:
        str = L"CharOrdinalDepend";
        break;
	case CharOrdinalAuxi:
        str = L"CharOrdinalAuxi";
        break;
	case CharBullets:
        str = L"CharBullets";
        break;
	case CharAllList:
        str = L"CharAllList";
        break;
	case CharConnector:
        str = L"CharConnector";
        break;
	case CharSpliteSymbol:
        str = L"CharSpliteSymbol";
        break;
	case CharLabelSymbolDouble:
        str = L"CharLabelSymbolDouble";
        break;
	case CharLabelSymbolSingle:
        str = L"CharLabelSymbolSingle";
        break;
	case CharEnSBCCase:
        str = L"CharEnSBCCase";
        break;
	//case CharEnDBCCase:
	//	str = L"CharEnDBCCase";
	//	break;
	case CharEnUpperCase:
        str = L"CharEnUpperCase";
        break;
	case CharEnLowCase:
        str = L"CharEnLowCase";
        break;
	case CharHanMorpheme:
        str = L"CharHanMorpheme";
        break;
	case CharHanNoMorpheme:
        str = L"CharHanNoMorpheme";
        break;
	case CharHanTraditional:
        str = L"CharHanTraditional";
        break;
	case CharHanPrefix:
        str = L"CharHanPrefix";
        break;
	case CharHanSuffix:
        str = L"CharHanSuffix";
        break;
	case CharHanName:
        str = L"CharHanName";
        break;
	case CharHanForeignName:
        str = L"CharHanForeignName";
        break;
	case CharHanSurname:
        str = L"CharHanSurname";
        break;
	case CharHanOrgan:
        str = L"CharHanOrgan";
        break;
	default:
        str = L"Unknown emCharAttribute type!";
        break;
	}

	return str;
}

wstring DictAttr::WordAttributeToWStr(emWordAttribute wordAttr)
{
	wstring str;
	switch(wordAttr)
	{
	case WordNoun:
        str = L"WordNoun";
        break;
	case WordNounName:
        str = L"WordNounName";
        break;
	case WordNounAddr:
        str = L"WordNounAddr";
        break;
	case WordNounOrg:
        str = L"WordNounOrg";
        break;
	case WordNounProper:
        str = L"WordNounProper";
        break;
	case WordNounEndDate:
        str = L"WordNounEndDate";
        break;
	case WordNounAddrSuffixHigh:
        str = L"WordNounAddrSuffixHigh";
        break;
	case WordNounAddrSuffixLow:
        str = L"WordNounAddrSuffixLow";
        break;
	case WordNounSurnameHigh:
        str = L"WordNounSurnameHigh";
        break;
	case WordNounSurnameMid:
        str = L"WordNounSurnameMid";
        break;
	case WordNounSurnameLow:
        str = L"WordNounSurnameLow";
        break;
	case WordVerb:
        str = L"WordVerb";
        break;
	case WordVerbNoObject:
        str = L"WordVerbNoObject";
        break;
	case WordVerbNounObject:
        str = L"WordVerbNounObject";
        break;
	case WordVerbVerbObject:
        str = L"WordVerbVerbObject";
        break;
	case WordVerbAdjObject:
        str = L"WordVerbAdjObject";
        break;
	case WordVerbSmallObject:
        str = L"WordVerbSmallObject";
        break;
	case WordVerbTwoObject:
        str = L"WordVerbTwoObject";
        break;
	case WordVerbAnnexe:
        str = L"WordVerbAnnexe";
        break;
	case WordVerbAuxiliary:
        str = L"WordVerbAuxiliary";
        break;
	case WordVerbTendency:
        str = L"WordVerbTendency";
        break;
	case WordVerbCopula:
        str = L"WordVerbCopula";
        break;
	case WordAuxiliary:
        str = L"WordAuxiliary";
        break;
	case WordAuxiliaryStructure:
        str = L"WordAuxiliaryStructure";
        break;
	case WordAuxiliaryTense:
        str = L"WordAuxiliaryTense";
        break;
	case WordAuxiliaryMood:
        str = L"WordAuxiliaryMood";
        break;
	case WordTime:
        str = L"WordTime";
        break;
	case WordSpace:
        str = L"WordSpace";
        break;
	case WordLocalizer:
        str = L"WordLocalizer";
        break;
	case WordNumber:
        str = L"WordNumber";
        break;
	case WordQuantity:
        str = L"WordQuantity";
        break;
	case WordDistinguish:
        str = L"WordDistinguish";
        break;
	case WordPronoun:
        str = L"WordPronoun";
        break;
	case WordAdjective:
        str = L"WordAdjective";
        break;
	case WordDescriptive:
        str = L"WordDescriptive";
        break;
	case WordAdverb:
        str = L"WordAdverb";
        break;
	case WordPreposition:
        str = L"WordPreposition";
        break;
	case WordConjunction:
        str = L"WordConjunction";
        break;
	case WordUnknown:
        str = L"WordUnknown";
        break;
	default:
        str = L"Error Unknown Word";
        break;
	}

	return str;
}

wstring DictAttr::WordTypeToWStr(emWordType wordType)
{
	wstring str;
	switch(wordType)
	{
	case WordTypeNull:
        str = L"WordTypeNull";
        break;
	case WordTypeDate:
        str = L"WordTypeDate";
        break;
	case WordTypePhone:
        str = L"WordTypePhone";
        break;
	case WordTypeMobile:
        str = L"WordTypeMobile";
        break;
	case WordTypePostcode:
        str = L"WordTypePostcode";
        break;
	case WordTypeEmail:
        str = L"WordTypeEmail";
        break;
	case WordTypeIDCard:
        str = L"WordTypeIDCard";
        break;
	case WordTypeHttp:
        str = L"WordTypeHttp";
        break;
	case WordTypeDict:
        str = L"WordTypeDict";
        break;
	case WordTypeEng:
        str = L"WordTypeEng";
        break;
	case WordTypeSingleHan:
        str = L"WordTypeSingleHan";
        break;
	case WordTypeNumber:
        str = L"WordTypeNumber";
        break;
	case WordTypeNoWordChar:
        str = L"WordTypeNoWordChar";
        break;
	case WordTypeSpace:
        str = L"WordTypeSpace";
        break;
	case WordTypeLineBreak:
        str = L"WordTypeLineBreak";
        break;
	case WordTypeAllNoWord:
        str = L"WordTypeAllNoWord";
        break;
	case WordTypeUnknown:
        str = L"WordTypeUnknown";
        break;
	default:
        str = L"Error word type";
        break;
	}

	return str;
}

wstring DictAttr::ReliabilityToWStr(emReliability relia)
{
	wstring str;
	switch(relia)
	{
	case ReliabilityUnknown:
        str = L"ReliabilityUnknown";
        break;
	case ReliabilityLow:
        str = L"ReliabilityLow";
        break;
	case ReliabilityMid:
        str = L"ReliabilityMid";
        break;
	case ReliabilityHigh:
        str = L"ReliabilityHigh";
        break;
	case ReliabilityAffirm:
        str = L"ReliabilityAffirm";
        break;
	default:
        str = L"Error emReliability";
        break;
	}

	return str;
}

}
