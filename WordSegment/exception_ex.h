#ifndef EXCEPTION_EX_H
#define EXCEPTION_EX_H
#include "header.h"
#include <exception>
#include <string>
#include <sstream>

using namespace std;

namespace word_segment {
#define Exception(what) ExceptionEx(what, __FILE__, __FUNCTION__, __LINE__)
class ExceptionEx : public exception
{
public:
	ExceptionEx(string what,
    	string throwFile,
    	string throwFunction,
    	int throwLine)
    {
        m_strWhat = what;
        m_throwFile = throwFile;
        m_throwFunction = throwFunction;
        m_throwLine = throwLine;
	}

	virtual ~ExceptionEx(void)
	{}

	virtual const char* what( )
	{
        std::stringstream ss;
        ss << "Error Message: " << m_strWhat << "\nFile: " << m_throwFile << "\nFunction: " << m_throwFunction << "\nLine: " << m_throwLine;
        return ss.str().c_str();
	}

	const char* GetThrowFile() const
	{
        return m_throwFile.c_str();
	}

	const char* GetThrowFunction() const
	{
        return m_throwFunction.c_str();
	}

	int GetThrowLine() const
	{
        return m_throwLine;
	}

	const string GetExceptionInfor()
	{
        return m_strWhat;
	}

private:
    string m_strWhat;
    string m_throwFile;
    string m_throwFunction;
    int m_throwLine;
};

}

#endif
