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

	if (currentYear > licenseYear)
	{
		//�ѹ���
		return false;
	}
	else if (currentMonth > licenseMonth)
	{
		//�ѹ���
		return false;
	}
	else if (currentDay > licenseDay)
	{
		//�ѹ���
		return false;
	}
	else
	{
		//δ����
		return true;
	}
}
