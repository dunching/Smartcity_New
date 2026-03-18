// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "topic.h"
#include "async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

delivery_token_ptr topic::publish(const void* payload, size_t n)
{
	return cli_.publish(name_, payload, n, qos_, retained_);
}

delivery_token_ptr topic::publish(const void* payload, size_t n,
								  int qos, bool retained)
{
	return cli_.publish(name_, payload, n, qos, retained);
}

delivery_token_ptr topic::publish(binary_ref payload)
{
	return cli_.publish(name_, std::move(payload), qos_, retained_);
}

delivery_token_ptr topic::publish(binary_ref payload, int qos, bool retained)
{
	return cli_.publish(name_, std::move(payload), qos, retained);
}

token_ptr topic::subscribe(const subscribe_options& opts)
{
	return cli_.subscribe(name_, qos_, opts);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



