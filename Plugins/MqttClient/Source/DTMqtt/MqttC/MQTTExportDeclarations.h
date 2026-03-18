// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(EXPORTDECLARATIONS_H)
#define EXPORTDECLARATIONS_H

// #if defined(_WIN32) || defined(_WIN64)
// #   if defined(PAHO_MQTT_EXPORTS)
// #       define LIBMQTT_API __declspec(dllexport)
// #   elif defined(PAHO_MQTT_IMPORTS)
// #       define LIBMQTT_API __declspec(dllimport)
// #   else
// #       define LIBMQTT_API
// #   endif
// #else
// #    if defined(PAHO_MQTT_EXPORTS)
// #       define LIBMQTT_API  __attribute__ ((visibility ("default")))
// #    else
// #       define LIBMQTT_API extern
// #    endif
// #endif

#define LIBMQTT_API

#endif
