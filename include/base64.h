//Copyright 2022 Yang Y., All rights reserved.
#ifndef _BASE64_H_
#define _BASE64_H_
#include<iostream>
#include<vector>
#include<stdio.h>
#include <string>
#include<string.h>
using namespace std;
std::string b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string toBinary(int a)
{
	string str = "00000000";
	for (int i = 0; i < str.length(); i++)
	{
		if (a > 0)
		{
			str[7 - i] = a % 2 + 48;
			a /= 2;
		}
	}
	return str;
}

int fromBinary(string a)
{
	int b, c;
	b = 0;
	c = 1;
	for (int i = a.length() - 1; i >= 0; i--)
	{
		if (a[i] == '1')
		{
			b += c;
		}
		c *= 2;
	}
	return b;
}

vector<int> fromBase64toInt(string str)
{
	vector<int> a;
	string str1, str2, str3, str4, stra, strb, strc;
	str1 = toBinary(b64.find_first_of(str[0]));
	str2 = toBinary(b64.find_first_of(str[1]));
	str3 = toBinary(b64.find_first_of(str[2]));
	str4 = toBinary(b64.find_first_of(str[3]));
	stra = "00000000";
	strb = stra;
	strc = stra;
	for (int i = 0; i < 6; i++)
	{
		stra[i] = str1[i + 2];
		strc[i + 2] = str4[i + 2];
	}
	for (int i = 0; i < 4; i++)
	{
		strb[i] = str2[i + 4];
		strb[i + 4] = str3[i + 2];
	}
	for (int i = 0; i < 2; i++)
	{
		strc[i] = str3[i + 6];
		stra[i + 6] = str2[i + 2];
	}
	a.push_back(fromBinary(stra));
	a.push_back(fromBinary(strb));
	a.push_back(fromBinary(strc));
	return a;
}
#endif
