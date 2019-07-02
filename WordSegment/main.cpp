
#include "iword_seg.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/locale.hpp>

using namespace word_segment;
using namespace std;

int main(int argc, char* argv[])
{
    locale loc = locale::global(locale(locale(), "", LC_CTYPE));
    string strPath = "D:\\WordSegment\\";

    if (!IWordSeg::InitWordSegment(strPath + "log\\", strPath, true))
    {
        cout << "Failed to initialize WordSegment!" << endl;
        getchar();
        return -1;
    }

    ifstream ifs(strPath + "test.txt", ios::binary | ios::in);
    char buf[10244];
    string str;
    while(true)
    {
        ifs.read(buf, 10240);
        streamsize n = ifs.gcount();
        if (0 == n)
            break;

        buf[n] = 0;
        str += buf;
    }

    ifs.close();

    vector<wstring> vct;
    wstring wstr = boost::locale::conv::to_utf<wchar_t>(str, "utf-8");
    std::cout << "Size: " << wstr.size() << endl;
    
    for (int i = 0; i < 5; i++)
    {
        boost::posix_time::ptime pt1(boost::posix_time::microsec_clock::local_time());
        bool b = IWordSeg::SegmentWordMaxMatch(wstr, vct);
        boost::posix_time::ptime pt2(boost::posix_time::microsec_clock::local_time());

        boost::posix_time::time_duration td = pt2 -pt1;
        cout << boost::posix_time::to_simple_string(td) << endl;
    }

    std::ofstream ofs("D:\\result.txt");
    for (auto iter = vct.begin(); iter != vct.end(); iter++)
    {
        ofs << boost::locale::conv::from_utf(*iter, "gb2312") << "\n";
    }

    ofs.close();

//    cout << "\nReturn" << b << endl;

    getchar();

	return 0;
}
