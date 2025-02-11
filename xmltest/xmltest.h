#pragma once

#include <tinyxml2.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if defined( _MSC_VER ) || defined (WIN32)
	#include <crtdbg.h>
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#else
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

#ifdef BUILDING_XMLTEST
#define XMLTEST_EXTERN
#else
#define XMLTEST_EXTERN extern
#endif

namespace xmltest {

class TestResults {
public:
    TestResults() = default;
    void result(bool passed) { passed ? ++m_Pass : ++m_Fail; }
    int pass() { return m_Pass; }
    int fail() { return m_Fail; }
private:
    int m_Pass{0};
    int m_Fail{0};
};

XMLTEST_EXTERN TestResults allResults;

bool XMLTest (const char* testString, const char* expected, const char* found, bool echo=true, bool extraNL=false );
bool XMLTest(const char* testString, tinyxml2::XMLError expected, tinyxml2::XMLError found, bool echo = true, bool extraNL = false);
bool XMLTest(const char* testString, bool expected, bool found, bool echo = true, bool extraNL = false);

template< class T > bool XMLTest( const char* testString, T expected, T found, bool  echo=true );

void NullLineEndings( char* P);

int example_1();
int example_2();
int example_3();
bool example_4();

}  // namespace xmltest
