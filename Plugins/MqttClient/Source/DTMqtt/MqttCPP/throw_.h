// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

using namespace std;

class throw_
{
public:
	static void badcast();
	static void exception(int rc);
	static void exception(int rc, const char * msg);
	static void exception(int rc, int reasonCode, const char* msg);
};

