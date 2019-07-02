#include "iword_seg.h"
#include "dict.h"
#include "word_segment.h"
#include "log.h"
#include "boost/filesystem.hpp"
#include <boost/locale.hpp>

namespace word_segment{

bool IWordSeg::InitWordSegment(std::string strLogPath, std::string strDictPath, bool isDebug)
{
    emLogType type = LogTypeWarning;
    if (isDebug)
        type = LogTypeAllInfor;

    if (!Log::InitLog(strLogPath, type))
        return false;

    if (strDictPath.size() == 0)
        return false;

    if (strDictPath[strDictPath.size() - 1] != '/')
        strDictPath += '/';

    if (!Dict::LoadCharDict(strDictPath + "CharDict.dct"))
        return false;
    if (!Dict::LoadWordDict(strDictPath + "WordDict.dct"))
        return false;
    return true;
} 

bool IWordSeg::SegmentWordMaxMatch(const std::string& strText, std::vector<std::string>& vctResult)
{
    std::vector<std::wstring> vctWR;
    if (!SegmentWordMaxMatch(boost::locale::conv::to_utf<wchar_t>(strText, "utf-8"), vctWR))
        return false;

    vctResult.clear();
    for(auto iter = vctWR.begin(); iter != vctWR.end(); iter++)
    {
        vctResult.push_back(boost::locale::conv::from_utf(*iter, "utf-8"));
    }

    return true;
}

bool IWordSeg::SegmentWordMaxMatch(const std::wstring& strText, std::vector<std::wstring>& vctResult)
{
    if (!Dict::IsLoadedDict())
        return false;

    WordSegment wordSeg;
    vector<SegWord> vct;

    if (!wordSeg.SegmentWordMaxMatch(strText, vct))
        return false;

    vctResult.clear();
    for (auto iter = vct.begin(); iter != vct.end(); iter++)
        vctResult.push_back(iter->m_strSour);

    return true;
}

bool IWordSeg::SegmentWordMinMatch(const std::string& strText, std::vector<std::string>&vctResult)
{
    std::vector<std::wstring> vctWR;
    if (!SegmentWordMinMatch(boost::locale::conv::to_utf<wchar_t>(strText, "utf-8"), vctWR))
        return false;

    vctResult.clear();
    for(auto iter = vctWR.begin(); iter != vctWR.end(); iter++)
    {
        vctResult.push_back(boost::locale::conv::from_utf(*iter, "utf-8"));
    }

    return true;
}

bool IWordSeg::SegmentWordMinMatch(const std::wstring& strText, std::vector<std::wstring>& vctResult)
{
    if (!Dict::IsLoadedDict())
        return false;

    WordSegment wordSeg;
    vector<SegWord> vct;

    if (!wordSeg.SegmentWordMinMatch(strText, vct))
        return false;

    vctResult.clear();
    for (auto iter = vct.begin(); iter != vct.end(); iter++)
        vctResult.push_back(iter->m_strSour);

    return true;
}
}
