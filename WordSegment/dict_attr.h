#ifndef DICT_ATTR_H
#define DICT_ATTR_H
#include "header.h"
#include "dict_attribute.h"
#include <string>
#include <vector>
#include "log.h"

namespace word_segment {

class DictAttr
{
public:
    static wstring CharAttributeToWStr(emCharAttribute charAttr);
    static wstring WordAttributeToWStr(emWordAttribute wordAttr);
    static wstring WordTypeToWStr(emWordType wordType);
    static wstring ReliabilityToWStr(emReliability relia);
};
}

#endif
