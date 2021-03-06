/**
@brief class for basic system operations
@author: zhu-ty
@date: Aug 8, 2018
*/

#ifndef __SHADOWK_COMMON__
#define __SHADOWK_COMMON__

#pragma once
// include stl
#include <memory>
#include <vector>
#ifndef _MANAGED
#include <thread>
#endif
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <cmath>
#include <cstring>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <random>
#include <regex>
#if defined(_WIN32) || defined(WIN32)
//#define _WINSOCKAPI_ 
#include <windows.h>
#include <direct.h>
#include <time.h>
//#include <Winsock2.h>
//#include <Winsock2.h>
#else
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <pthread.h>
#endif

//#ifndef max
//#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif
//
//#ifndef min
//#define min(a,b)            (((a) < (b)) ? (a) : (b))
//#endif

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#ifndef WIN32
#define BLACK_TEXT(x) "\033[30;1m" << x << "\033[0m"
#define RED_TEXT(x) "\033[31;1m" << x << "\033[0m"
#define GREEN_TEXT(x) "\033[32;1m" <<  x << "\033[0m"
#define YELLOW_TEXT(x) "\033[33;1m" << x << "\033[0m"
#define BLUE_TEXT(x) "\033[34;1m" << x << "\033[0m"
#define MAGENTA_TEXT(x) "\033[35;1m" << x << "\033[0m"
#define CYAN_TEXT(x) "\033[36;1m" << x << "\033[0m"
#define WHITE_TEXT(x) "\033[37;1m" << x << "\033[0m"
#endif

#ifdef WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif
#endif

#define SKCOMMON_DEBUG_STRING std::string("file: " +   \
((std::string(__FILE__).find_last_of("/") != std::string::npos || std::string(__FILE__).find_last_of("\\") != std::string::npos) ?   \
 (  \
(std::string(__FILE__).find_last_of("\\") != std::string::npos) ? \
 std::string(__FILE__).erase(0, std::string(__FILE__).find_last_of("\\") + 1) : \
 std::string(__FILE__).erase(0, std::string(__FILE__).find_last_of("/") + 1) \
 )  \
 : std::string(__FILE__))  \
+ " line: " + std::to_string(__LINE__) +" func: " + std::string(__func__) +"\n") 

#define SKCOMMON_MAX_INFO_LENTH 8192

#define SKCOMMON_USED



class SKCommon {
private:
	enum class ConsoleColor {
		red = 12,
		blue = 9,
		green = 10,
		yellow = 14,
		white = 15,
		pink = 13,
		cyan = 11
	};
public:
	/***********************************************************/
	/*                    mkdir function                       */
	/***********************************************************/
	static int mkdir(char* dir) {
		infoOutput("Makring dir :" + std::string(dir));
#ifdef WIN32
		_mkdir(dir);
#else
		printf("MKDIR RETURN:: %d\n",::mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH));
#endif
		return 0;
	}
	static int mkdir(std::string dir) {
		return mkdir((char *)dir.c_str());
	}
#ifndef _MANAGED
	static int sleep(size_t miliseconds) {
		std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
		return 0;
	}
#endif

	static int setConsoleColor(ConsoleColor color) {
#ifdef WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(color));
#endif
		return 0;
	}


	static std::string getTimeString(std::string format = "")
	{
		time_t timep;
		time(&timep);
		char tmp[64];
		std::string _format = (format == "") ? "__%Y_%m_%d_%H_%M_%S__" : format;
		strftime(tmp, sizeof(tmp), _format.c_str(), localtime(&timep));
		return tmp;
	}

	struct timezone
	{
		int  tz_minuteswest; // minutes W of Greenwich  
		int  tz_dsttime;     // type of dst correction
	};

	static int64_t getCurrentTimeMicroSecond()
	{
#if !defined(_WIN32) || !defined(WIN32)
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * (int64_t)1000000 + tv.tv_usec;
#else
		FILETIME ft;
		uint64_t tmpres = 0;
#ifdef _WIN32_WCE
		SYSTEMTIME st;
		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &ft);
#else
		GetSystemTimeAsFileTime(&ft);
#endif
		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;
		/*converting file time to unix epoch*/
		tmpres /= 10;  /*convert into microseconds*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		auto tv_sec = (long)(tmpres / 1000000UL);
		auto tv_usec = (long)(tmpres % 1000000UL);
		return tv_sec * (int64_t)1000000 + tv_usec;
#endif
		
	}

	static inline std::string format(std::string msg, ...)
	{
		va_list list;
		std::string text;
		va_start(list, msg);
		text = SKCommon::_vsprint(msg, list);
		va_end(list);
		return text;
	}

	static int infoOutput(std::string info, ...)
	{
		std::string text;
		va_list list;
		if (info == "")
			return 0;
		va_start(list, info);
		text = SKCommon::_vsprint(info, list);
		va_end(list);
		SKCommon::_infoOutput(text);
		return 0;
	}

	static int errorOutput(std::string info, ...)
	{
		std::string text;
		va_list list;
		if (info == "")
			return 0;
		va_start(list, info);
		text = SKCommon::_vsprint(info, list);
		va_end(list);
		SKCommon::_errorOutput(text);
		return 0;
	}

	static int warningOutput(std::string info, ...)
	{
		std::string text;
		va_list list;
		if (info == "")
			return 0;
		va_start(list, info);
		text = SKCommon::_vsprint(info, list);
		va_end(list);
		SKCommon::_warningOutput(text);
		return 0;
	}


	static int debugOutput(std::string info, ...)
	{
		std::string text;
		va_list list;
		if (info == "")
			return 0;
		va_start(list, info);
		text = SKCommon::_vsprint(info, list);
		va_end(list);
		SKCommon::_debugOutput(text);
		return 0;
	}

	static int mkEmptyFile(std::string dir) 
	{
		FILE *a = fopen(dir.c_str(), "w");
		return fclose(a);
	}

	static inline bool existFile(const std::string& name) 
	{
		if (FILE *file = fopen(name.c_str(), "r")) {
			fclose(file);
			return true;
		}
		else {
			return false;
		}
	}

	static int copyFile(std::string file1, std::string file2)
	{
		if (file1 == file2)
			return -1;
		FILE *src = fopen(file1.c_str(), "rb");
		FILE *dst = fopen(file2.c_str(), "wb");
		if (src == nullptr)
			return -2;
		int i;
		for (i = getc(src); i != EOF; i = getc(src))
		{
			putc(i, dst);
		}
		fclose(dst);
		fclose(src);
	}

	static int removeFile(std::string file)
	{
		return remove(file.c_str());
	}

	static std::string getFileExtention(std::string fileName)
	{
		struct stat s;
		if (stat(fileName.c_str(), &s) == 0)
		{
			if (s.st_mode & S_IFDIR)
			{
				//it's a directory
				return "";
			}
			else if (s.st_mode & S_IFREG)
			{
				//it's a file
				if (fileName.find_last_of(".") == -1)
					return "";
				else
					return fileName.substr(fileName.find_last_of(".") + 1);
			}
			else
			{
				//something else
				return "";
			}
		}
		else
		{
			//error
			return "";
		}
	}
	
	static inline std::string toLower(std::string in)
	{
		std::transform(in.begin(), in.end(), in.begin(), singletolower); //Better than ::tolower() when work with UTF-8
		return in;
	}

	static std::vector<std::string> splitString(std::string input, std::string regex) {
		// passing -1 as the submatch index parameter performs splitting
		std::regex re(regex);
		std::sregex_token_iterator
			first{ input.begin(), input.end(), re, -1 },
			last;
		return{ first, last };
	}
	
private:
	static char singletolower(char in) {
		if (in <= 'Z' && in >= 'A')
			return in - ('Z' - 'z');
		return in;
	}

	static std::string _vsprint(std::string str, va_list list)
	{
		std::size_t const STRING_BUFFER(SKCOMMON_MAX_INFO_LENTH);
		char text[STRING_BUFFER];
		if (str == "")
			return std::string();
#		if(GLM_COMPILER & GLM_COMPILER_VC)
		vsprintf_s(text, STRING_BUFFER, str.c_str(), list);
#		else//
		vsprintf(text, str.c_str(), list);
#		endif//
		return std::string(text);
	}

	static int _errorOutput(std::string info) {
#ifdef WIN32
		SKCommon::setConsoleColor(ConsoleColor::red);
		std::cerr << "ERROR: " << info.c_str() << std::endl;
		SKCommon::setConsoleColor(ConsoleColor::white);
#else
		std::cerr << RED_TEXT("ERROR: ") << RED_TEXT(info.c_str())
			<< std::endl;
#endif
		return 0;
	}

	static int _warningOutput(std::string info) {
#ifdef WIN32
		SKCommon::setConsoleColor(ConsoleColor::yellow);
		std::cerr << "WARNING: " << info.c_str() << std::endl;
		SKCommon::setConsoleColor(ConsoleColor::white);
#else
		std::cerr << YELLOW_TEXT("WARNING: ") << YELLOW_TEXT(info.c_str())
			<< std::endl;
#endif
		return 0;
	}

	static int _infoOutput(std::string info) {
#ifdef WIN32
		SKCommon::setConsoleColor(ConsoleColor::green);
		std::cerr << "INFO: " << info.c_str() << std::endl;
		SKCommon::setConsoleColor(ConsoleColor::white);
#else
		std::cerr << GREEN_TEXT("INFO: ") << GREEN_TEXT(info.c_str())
			<< std::endl;
#endif
		return 0;
	}

	static int _debugOutput(std::string info) {
#ifdef WIN32
		SKCommon::setConsoleColor(ConsoleColor::pink);
		std::cerr << "DEBUG INFO: " << info.c_str() << std::endl;
		SKCommon::setConsoleColor(ConsoleColor::white);
#else
		std::cerr << MAGENTA_TEXT("DEBUG INFO: ") << MAGENTA_TEXT(info.c_str())
			<< std::endl;
#endif
		return 0;
	}
};

#endif //__SHADOWK_COMMON__