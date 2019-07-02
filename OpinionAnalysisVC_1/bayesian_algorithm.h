#ifndef BAYESIAN_ALGORITHM_H
#define BAYESIAN_ALGORITHM_H

#include "opinion_header.h"
#include "ialgorithm.h"

namespace opinion_analysis {

class BayesianAlgorithm : public IAlgorithm
{
public:
    BayesianAlgorithm(void);
    BayesianAlgorithm(DictAnaly* dictAnaly);
    ~BayesianAlgorithm(void);

    inline virtual void SetDictAnaly(DictAnaly* pDictAnaly){ m_pDictAnaly = pDictAnaly; }
    virtual bool JudgeTextCategory(const StatText& statText, Category& type);
    
protected:
    const map<Category, double> CalcCategoryWeights(const ItemDict* pItem);
    void CalcOpinionWeights(const ItemDict* pItem, const Category type, double& dblNegative, double& dblPositive);
protected:
    DictAnaly* m_pDictAnaly;
};

}
#endif //BAYESIAN_ALGORITHM_H
