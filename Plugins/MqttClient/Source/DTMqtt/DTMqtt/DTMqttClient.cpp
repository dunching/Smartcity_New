// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "DTMqttClient.h"
#include "Async/TaskGraphInterfaces.h"
#include "Async/Async.h"

#define MQTT_TRY_BEGIN										\
if ( !MqttClient.IsValid() )								\
{															\
	Success = false;										\
	ErrorMsg = TEXT("No Connect");							\
	return;													\
}															\
try															\
{							

#define MQTT_TRY_END										\
	Success = true;											\
	ErrorMsg = TEXT("Ok");									\
}															\
catch (mqtt::exception e)									\
{															\
	Success = false;										\
	ErrorMsg = UTF8_TO_TCHAR(e.what());						\
	return;													\
}															\
catch (...)													\
{															\
	Success = false;										\
	ErrorMsg = TEXT("Unknown Error");						\
	return;													\
}

void UDTMqttClient::connected(const mqtt::string& cause)
{
	const FString Cause = UTF8_TO_TCHAR(cause.c_str());
	AsyncTask(ENamedThreads::GameThread, [this, Cause]()
	{
		OnConnected.Broadcast(Cause);
	});
}

void UDTMqttClient::connection_lost(const mqtt::string& cause)
{
	const FString Cause = UTF8_TO_TCHAR(cause.c_str());
	AsyncTask(ENamedThreads::GameThread, [this, Cause]()
	{
		OnConnectionLost.Broadcast(Cause);
	});
}

void UDTMqttClient::message_arrived(mqtt::const_message_ptr msg)
{
	if ( msg )
	{
		const FString Topic = UTF8_TO_TCHAR(msg->get_topic().c_str());
		const FString Message = UTF8_TO_TCHAR(msg->get_payload_str().c_str());	
		AsyncTask(ENamedThreads::GameThread, [this, Topic, Message]()
		{
			OnMessageArrived.Broadcast(Topic, Message);
		});
	}
}

void UDTMqttClient::FinishDestroy()
{
	UObject::FinishDestroy();
	
	try
	{
		// 有效数据，并且有链接, 关闭链接
		if ( MqttClient.IsValid())
		{
			if (MqttClient->is_connected())
			{
				MqttClient->stop_consuming();
				MqttClient->disconnect()->wait();
			}
			MqttClient.Reset();
		}
	}
	catch (...)
	{
	}
}

void UDTMqttClient::CreateMqttClient(UDTMqttClient *& MqttClient)
{
	MqttClient = NewObject<UDTMqttClient>();
}


void UDTMqttClient::Connect( const FString& ServerUrl, const FString& ClientId, const FString& UserName, const FString& Password, int KeepAliveInterval, int ConnectTimeout, bool & Success, FString & ErrorMsg )
{
	// 有效数据，并且有链接, 关闭链接
	if ( MqttClient.IsValid())
	{
		MQTT_TRY_BEGIN
		
		if (MqttClient->is_connected())
		{
			MqttClient->stop_consuming();
			MqttClient->disconnect()->wait();
		}
		MqttClient.Reset();

		MQTT_TRY_END
	}

	// 格式化数据
	const std::string szServerUrl = TCHAR_TO_UTF8(*ServerUrl);
	const std::string szClientId = TCHAR_TO_UTF8(*ClientId);
	const std::string szUserName = TCHAR_TO_UTF8(*UserName);
	const std::string szPassword = TCHAR_TO_UTF8(*Password);
	
	try
	{							
		// 重新创建链接
		MqttClient = MakeShared<mqtt::async_client>(szServerUrl, szClientId);
	}			
	catch (mqtt::exception e)									
	{															
		Success = false;										
		ErrorMsg = UTF8_TO_TCHAR(e.what());						
		return;													
	}															
	catch (...)													
	{															
		Success = false;										
		ErrorMsg = TEXT("Unknown Error");						
		return;												
	}
	
	MQTT_TRY_BEGIN
	
	// 设置链接属性
	ConnectOptions.set_keep_alive_interval(KeepAliveInterval);
	ConnectOptions.set_clean_session(true);
	ConnectOptions.set_user_name(szUserName);
	ConnectOptions.set_password(szPassword);
	ConnectOptions.set_connect_timeout(ConnectTimeout);
	MqttClient->set_callback(*this);
	
	// 链接服务器
	MqttClient->connect(ConnectOptions)->wait();
	MqttClient->start_consuming();

	MQTT_TRY_END
}

void UDTMqttClient::Subscribe(const FString& TopicFilter, EDT_QualityOfService QoS, bool & Success, FString & ErrorMsg)
{
	MQTT_TRY_BEGIN
	
	const std::string szTopicFilter = TCHAR_TO_UTF8(*TopicFilter);
	MqttClient->subscribe(szTopicFilter, (int)QoS);
	
	MQTT_TRY_END
}

void UDTMqttClient::UnSubscribe(const FString& TopicFilter, bool & Success, FString & ErrorMsg)
{
	MQTT_TRY_BEGIN
	
	const std::string szTopicFilter = TCHAR_TO_UTF8(*TopicFilter);
	MqttClient->unsubscribe(szTopicFilter);

	MQTT_TRY_END
}

void UDTMqttClient::Publish(const FString& Topic, const FString& Message, EDT_QualityOfService QoS, bool Retained, bool & Success, FString & ErrorMsg)
{
	MQTT_TRY_BEGIN
	
	const std::string szTopic = TCHAR_TO_UTF8(*Topic);
	const std::string szMessage = TCHAR_TO_UTF8(*Message);
	MqttClient->publish(szTopic, szMessage.c_str(), szMessage.length(), (int)QoS, Retained );

	MQTT_TRY_END
}

void UDTMqttClient::Disconnect(bool & Success, FString & ErrorMsg)
{
	MQTT_TRY_BEGIN
	
	MqttClient->stop_consuming();
	MqttClient->disconnect()->wait();
	MqttClient.Reset();

	MQTT_TRY_END
}

void UDTMqttClient::BindConnectedDelegate(const FConnected Connected)
{
	OnConnected.Add(Connected);
}

void UDTMqttClient::BindConnectionLostDelegate(const FConnectionLost ConnectionLost)
{
	OnConnectionLost.Add(ConnectionLost);
}

void UDTMqttClient::BindMessageDelegate(const FMessageArrived MessageArrived)
{
	OnMessageArrived.Add(MessageArrived);
}
