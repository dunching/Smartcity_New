// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

#include "CoreMinimal.h"
#include "DTMqttHead.h"
#include "UObject/Object.h"
#include "DTMqttClient.generated.h"

UENUM()
enum class EDT_QualityOfService  : uint8
{
	QoS0 = 0 UMETA(DisplayName = "QoS0", ToolTip = "At most once"),
	QoS1 = 1 UMETA(DisplayName = "QoS1", ToolTip = "At least once"),
	QoS2 = 2 UMETA(DisplayName = "QoS2", ToolTip = "Exactly once"),
};


/**
 * Lightweight client for talking to an MQTT server using non-blocking
 */
UCLASS(BlueprintType, meta=(DisplayName="DT Mqtt Client"))
class DTMQTT_API UDTMqttClient : public UObject, public mqtt::callback
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FConnected, const FString&, Cause);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FConnectionLost, const FString&, Cause);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FMessageArrived, const FString&, Topic, const FString&, Message);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectedMulticast, const FString&, Cause);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectionLostMulticast, const FString&, Cause);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMessageArrivedMulticast, const FString&, Topic, const FString&, Message);

public:
	TSharedPtr<mqtt::async_client>			MqttClient;
	mqtt::connect_options					ConnectOptions;

public:
	FConnectedMulticast						OnConnected;
	FConnectionLostMulticast				OnConnectionLost;
	FMessageArrivedMulticast				OnMessageArrived;
	
	// mqtt 回调接口
public:
	virtual void connected(const mqtt::string& cause) override;
	virtual void connection_lost(const mqtt::string& cause) override;
	virtual void message_arrived(mqtt::const_message_ptr msg) override;

	// object 接口回调
public:
	virtual void FinishDestroy() override;

public:
	/**
	 * Create an MQTT client object
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	static void CreateMqttClient(UDTMqttClient *& MqttClient);
	
	/**
	 * Connects to an MQTT server using the provided connect options.
	 * Param Url : the address of the server to connect to, specified
	 *  				as a URI.
	 * Param ClientId : a client identifier that is unique on the server
	 *  			   being connected to
	 * Param UserName : The user name for connecting to the MQTT broker.
	 * Param Password : The password for connecting to the MQTT broker.
	 */
	UFUNCTION(BlueprintCallable, meta=(KeepAliveInterval=60,ConnectTimeout=5), Category="DT Mqtt")
	void Connect( const FString & ServerUrl, const FString & ClientId, const FString & UserName, const FString & Password, int KeepAliveInterval, int ConnectTimeout, bool & Success, FString & ErrorMsg );

	/**
	 * Subscribe to a topic, which may include wildcards.
	 * Param TopicFilter : the topic to subscribe to, which can include
	 *  				  wildcards.
	 * Param QoS : The quality of service for the subscription
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void Subscribe( const FString & TopicFilter, EDT_QualityOfService QoS, bool & Success, FString & ErrorMsg );

	/**
	 * Requests the server unsubscribe the client from a topic.
	 * Param TopicFilter : the topic to unsubscribe from. It must match a
	 *  				  topicFilter specified on an earlier subscribe.
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void UnSubscribe( const FString & TopicFilter, bool & Success, FString & ErrorMsg );

	/**
	 * Publishes a message to a topic on the server
	 * Param Topic : The topic to deliver the message to
	 * Param Message : sent message
	 * Param QoS : the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * Param Retained : whether or not this message should be retained by the
	 *  			   server.
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void Publish( const FString & Topic, const FString & Message, EDT_QualityOfService QoS, bool Retained, bool & Success, FString & ErrorMsg );

	/**
	 * Disconnects from the server.
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void Disconnect(bool & Success, FString & ErrorMsg);
	
	/**
	 * This method is called when the client is connected. 
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void BindConnectedDelegate(const FConnected Connected);

	/**
	 * This method is called when the connection to the server is lost.
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void BindConnectionLostDelegate(const FConnectionLost ConnectionLost);

	/**
	 * This method is called when a message arrives from the server.
	 */
	UFUNCTION(BlueprintCallable, Category="DT Mqtt")
	void BindMessageDelegate(const FMessageArrived MessageArrived);

};

