// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "throw_.h"
#include "exception.h"
#include "types.h"

void throw_::badcast()
{
	throw bad_cast();
}

void throw_::exception(int rc)
{
	throw mqtt::exception(rc);
}

void throw_::exception(int rc, const char* msg)
{
	throw mqtt::exception(rc, msg);
}

void throw_::exception(int rc, int reasonCode, const char* msg)
{
	throw mqtt::exception(rc, (mqtt::ReasonCode)reasonCode, msg);
}