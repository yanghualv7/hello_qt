#include "pch.h"
#include <iostream>
#include <string>
#include "checkDateValidity.h"
bool checkDateValidity(const std::string& currentDateStr, const std::string& licenseDateStr)
{
	int currentYear = std::stoi(currentDateStr.substr(0, 4));
	int currentMonth = std::stoi(currentDateStr.substr(4, 2));
	int currentDay = std::stoi(currentDateStr.substr(6, 2));

	int licenseYear = std::stoi(licenseDateStr.substr(0, 4));
	int licenseMonth = std::stoi(licenseDateStr.substr(4, 2));
	int licenseDay = std::stoi(licenseDateStr.substr(6, 2));

	// 比较年
	if (currentYear < licenseYear)
		return true;
	else if (currentYear > licenseYear)
		return false;

	// 比较月
	if (currentMonth < licenseMonth)
		return true;
	else if (currentMonth > licenseMonth)
		return false;

	// 比较日
	if (currentDay <= licenseDay)
		return true;
	else
		return false;
}
