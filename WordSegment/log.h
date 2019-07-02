#ifndef LOG_H
#define LOG_H

#include "header.h"
#include <string>
#include <fstream>
#include <boost/thread/mutex.hpp>

#define CRLF "\r\n"

using namespace std;

namespace word_segment{
enum emLogType : short
{
	LogTypeNull = 0,
	LogTypeException = 1,	//异常信息，必须写入日志
	LogTypeError = 2,    //错误信息，必须写入日志
	LogTypeWarning = 3,    //警告信息，最好写入日志
	LogTypeInforHigh = 4,	//较重要的文字信息
	LogTypeInforMid = 5,	//中等重要的文字信息
	LogTypeInforLow = 6,	//不重要的文字信息
	LogTypeAllInfor = 7,	//显示所有日志信息
};
 
class Log
{  
protected:
	Log();
	~Log();

protected:
	void WriteLog(string msg, emLogType logType, bool bTime);
	string FileName();
	string LogTypeToStr(emLogType logType);
	bool IsOpen(){return m_pLogStream != nullptr;}

protected:
	size_t m_byteCount;
	ofstream* m_pLogStream;
	boost::mutex m_mutex;

	string m_strRootPath;
	emLogType m_logType;	//写入日志的级别，即只有优先级高于或等于此的才会写入日志

    static Log* m_log;

public:
	static bool InitLog(string strRootPath, emLogType logTypeFrom);
	static void Write(wstring location, wstring msg, emLogType logType = LogTypeInforLow, bool bTime = true);
	static void Write(string location, string msg, emLogType logType = LogTypeInforLow, bool bTime = true);
};
}
#endif
