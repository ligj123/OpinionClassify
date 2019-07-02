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
    virtual bool JudgeTextOpinion(const StatText& statText, Category type, emOpinion& opinion);

protected:
	const map<Category, double> CalcCategoryWeights(const ItemDict* pItem, const map<Category, double>& mapNPRate);
    const map<emOpinion, double> CalcOpinionWeights(const ItemDict* pItem, const Category type, const map<emOpinion, double>& mapNPRate);
protected:
    DictAnaly* m_pDictAnaly;
};

}
#endif //BAYESIAN_ALGORITHM_H
