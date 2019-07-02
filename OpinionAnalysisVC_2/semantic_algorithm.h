#ifndef SEMANTIC_ALGORITHM_H
#define SEMANTIC_ALGORITHM_H

#include "ialgorithm.h"

namespace opinion_analysis {
class SemanticAlgorithm :
    public IAlgorithm
{
public:
    SemanticAlgorithm();
    SemanticAlgorithm(DictAnaly* pDictAnaly);
    ~SemanticAlgorithm();

    inline virtual void SetDictAnaly(DictAnaly* pDictAnaly){ m_pDictAnaly = pDictAnaly; }
    virtual bool JudgeTextCategory(const StatText& statText, Category& type);
    virtual bool JudgeTextOpinion(const StatText& statText, Category type, emOpinion& opinion);
    
protected:
    bool IsDenyWord(const SegWord* pSegWord);
    bool IsTopicWord(const SegWord* pSegWord, const vector<SegWord>* pVctSubject);
    void CalcWordOpinionManual(const SegWord* pSegWord, double& dblPositive, double& dblNegative);
    void CalcWordOpinionStat(const SegWord* pSegWord, double& dblPositive, double& dblNegative);
protected:
    DictAnaly* m_pDictAnaly;
};

}

#endif //SEMANTIC_ALGORITHM_H