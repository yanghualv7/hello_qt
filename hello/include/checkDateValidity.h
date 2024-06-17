#pragma once

#ifdef CHECKDATEVALIDITY_EXPORTS
#define CHECKDATEVALIDITY_API __declspec(dllexport)
#else
#define CHECKDATEVALIDITY_API __declspec(dllimport)
#endif

extern "C" CHECKDATEVALIDITY_API bool checkDateValidity(const std::string & currentDateStr, const std::string & licenseDateStr);