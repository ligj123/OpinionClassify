#include "log.h"
#include <boost/locale.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include "exception_ex.h"

namespace word_segment {

Log* Log::m_log = NULL;

Log::Log()
{
    m_logType = LogTypeInforMid;
	m_pLogStream = nullptr;
	m_byteCount = 0;
}

Log::~Log()
{
	if (m_pLogStream)
	{
        m_pLogStream->close();
        m_pLogStream = nullptr;
	}
}

void Log::WriteLog(string msg, emLogType logType, bool bWriteTime)
{
	boost::unique_lock<boost::mutex> lock(m_mutex);

	if (m_byteCount > 10 * 1024 * 1024) 
	{
        m_pLogStream->close();
        m_pLogStream = nullptr;
        m_byteCount = 0;
	}

	if (m_pLogStream == nullptr)
	{
        m_pLogStream = new ofstream(FileName(), ios_base::out | ios_base::binary);
	}

	if (bWriteTime)
        *m_pLogStream << to_simple_string(boost::posix_time::second_clock::local_time()) << "\t";

	*m_pLogStream << LogTypeToStr(logType) << "\t";
	*m_pLogStream << msg;

	m_byteCount += msg.size();

#ifdef LOG_SCREEN
    if (bWriteTime)
        cout << to_simple_string(boost::posix_time::second_clock::local_time()) << "\t";

	cout << LogTypeToStr(logType) << "\t";
	cout << msg;
#endif

	m_pLogStream->flush();
}

string Log::FileName()
{
	boost::posix_time::ptime pt(boost::posix_time::second_clock::local_time());
    return m_strRootPath + "SegLog" + to_iso_string(pt) + ".txt";
}

string Log::LogTypeToStr(emLogType logType)
{
	string type;
	switch(logType)
	{
	case LogTypeNull:
        type = "LogTypeNull";
        break;
	case LogTypeException:
        type = "LogTypeException";
        break;
	case LogTypeError:
        type = "LogTypeError";
        break;
	case LogTypeWarning:
        type = "LogTypeWarning";
        break;
	case LogTypeInforHigh:
        type = "LogTypeInforHigh";
        break;
	case LogTypeInforMid:
        type = "LogTypeInforMid";
        break;
	case LogTypeInforLow:
        type = "LogTypeInforLow";
        break;
	case LogTypeAllInfor:
        type = "LogTypeAllInfor";
        break;
    default:
        break;
	}

	return type;;
}

bool Log::InitLog(string strRootPath, emLogType logTypeFrom)
{
    boost::filesystem::path path(strRootPath);
    if (!boost::filesystem::is_directory(path))
    {
        if (!boost::filesystem::create_directories(path))
            return false;
    }

    if (strRootPath[strRootPath.size() - 1] == '\\')
        strRootPath[strRootPath.size() - 1] = '/';

    if (strRootPath[strRootPath.size() - 1] != '/')
        strRootPath += '/';

    m_log = new Log();
    m_log->m_logType = logTypeFrom;
    m_log->m_strRootPath = strRootPath;

    return true;
}

void Log::Write(wstring location, wstring msg, emLogType logType, bool bTime)
{
    Write(boost::locale::conv::from_utf(location, "utf-8"),
            boost::locale::conv::from_utf(msg,  "utf-8"),
            logType,
            bTime);
}

void Log::Write(string location, string msg, emLogType logType, bool bTime)
{
	if (msg.size() == 0)
    return;

	if (NULL == m_log)
        throw Exception("Not call InitLog first!");

	if (logType <= m_log->m_logType)
        m_log->WriteLog(location + "\t" + msg + CRLF, logType, bTime);
}

}
