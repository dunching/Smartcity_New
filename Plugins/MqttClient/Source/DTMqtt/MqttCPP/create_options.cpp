// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "create_options.h"
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

const MQTTAsync_createOptions create_options::DFLT_C_STRUCT =
			MQTTAsync_createOptions_initializer;


create_options::create_options(int mqttVersion) : create_options()
{
	opts_.MQTTVersion = mqttVersion;
}

create_options::create_options(int mqttVersion, int maxBufferedMessages) : create_options()
{
	opts_.MQTTVersion = mqttVersion;

	if (maxBufferedMessages != 0) {
		opts_.sendWhileDisconnected = to_int(true);
		opts_.maxBufferedMessages = maxBufferedMessages;
	}
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt

