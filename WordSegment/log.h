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
	LogTypeException = 1,	//�쳣��Ϣ������д����־
	LogTypeError = 2,    //������Ϣ������д����־
	LogTypeWarning = 3,    //������Ϣ�����д����־
	LogTypeInforHigh = 4,	//����Ҫ��������Ϣ
	LogTypeInforMid = 5,	//�е���Ҫ��������Ϣ
	LogTypeInforLow = 6,	//����Ҫ��������Ϣ
	LogTypeAllInfor = 7,	//��ʾ������־��Ϣ
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
	emLogType m_logType;	//д����־�ļ��𣬼�ֻ�����ȼ����ڻ���ڴ˵ĲŻ�д����־

    static Log* m_log;

public:
	static bool InitLog(string strRootPath, emLogType logTypeFrom);
	static void Write(wstring location, wstring msg, emLogType logType = LogTypeInforLow, bool bTime = true);
	static void Write(string location, string msg, emLogType logType = LogTypeInforLow, bool bTime = true);
};
}
#endif
