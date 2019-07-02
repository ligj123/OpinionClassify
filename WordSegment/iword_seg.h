#ifndef IWORD_SEG_H
#define IWORD_SEG_H
#include <string>
#include <vector>

namespace word_segment{

class IWordSeg
{
    public:
        static bool InitWordSegment(std::string strLogPath, std::string strDictPath, bool isDebug);
        static bool SegmentWordMaxMatch(const std::string& strText, std::vector<std::string>& vctResult);
        static bool SegmentWordMaxMatch(const std::wstring& strText, std::vector<std::wstring>& vctResult);
        static bool SegmentWordMinMatch(const std::string& strText, std::vector<std::string>&vctResult);
        static bool SegmentWordMinMatch(const std::wstring& strText, std::vector<std::wstring>& vctResult);
};

}
#endif // IWORD_SEG_H
 