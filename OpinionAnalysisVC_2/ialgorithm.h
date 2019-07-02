
#ifndef IALGORITHM_H
#define IALGORITHM_H

#include <string>
#include <vector>
#include <seg_word.h>
#include "category.h"
#include "dict_analy.h"

namespace opinion_analysis {
using namespace std;
class IAlgorithm
{
public:
    virtual void SetDictAnaly(DictAnaly* dictAnaly) = 0;
    virtual bool JudgeTextCategory(const StatText& statText, Category& type) = 0;
    virtual bool JudgeTextOpinion(const StatText& statText, Category type, emOpinion& opinion) = 0;
};

}
#endif //IALGORITHM_H
