// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "subscribe_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

#if __cplusplus < 201703L
	constexpr bool subscribe_options::SUBSCRIBE_NO_LOCAL;
	constexpr bool subscribe_options::SUBSCRIBE_LOCAL;

	constexpr bool subscribe_options::NO_RETAIN_AS_PUBLISHED;
	constexpr bool subscribe_options::RETAIN_AS_PUBLISHED;
#endif

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

