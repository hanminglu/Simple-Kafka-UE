#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleKafkaFunctionLib.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FKafkaConsumerCallback_BP, FString, Msg);

UCLASS()
class SIMPLEKAFKA_API USimpleKafkaFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	* ��ʱ���������������ߣ�Լ��ֻ��Ҫ����һ��topic���ɡ�
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Consumer")
	static bool StartConsumer(const FString& URL, const FString& TopicStr, const FString& GroudID = "0");

	//��Ϸ��������֮ǰ��һ�£�������
	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Consumer")
	static void StopConsumer();

	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Consumer")
	static void RegisterConsumerCallback(FKafkaConsumerCallback_BP Callback);


	/*
	* ������Ҳ�����ɣ�ֻ��Ҫ����һ������������topic
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Producer")
	static bool StartProducer(const FString& URL, const FString& TopicStr, int32 Partion = 0);

	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Producer")
	static bool SendMessag(const FString& Msg, const FString& KeyStr = "");

	//��Ϸ��������֮ǰ��һ�£�������
	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Producer")
	static void StopProducer();


public:
	// ONLY for debug
	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Consumer")
	static void TestConsumer();

	// ONLY for debug
	UFUNCTION(BlueprintCallable, Category = "SimpleKafka|Producer")
	static void TestProduer();

private:
	static class SimpleKafkaConsumer* KafkaConsumerClient_;

	static class SimpleKafkaProducer* KafkaProducer;
};
