
#pragma once

#include <string>
#include <iostream>
#include "librdkafka/rdkafkacpp.h"

/*
�÷���
SimpleKafkaProducer* KafkaprClient_ = new SimpleKafkaProducer("localhost:9092", "test", 0);
KafkaprClient_->Init();
KafkaprClient_->pushMessage("ddddddd","Anykey"); 

1. KafkaProducer���캯������ʼ��һ���������߿ͻ��ˡ���Ҫָ��������������1����������Ҫ������Kafka��ַ����zookeeper��ip��port����2�������ߺ�����������Ϣ��Ҫ���͵�Topic���⣻��3����Ϣ��Ҫ�����ķ���

2. Init �л��������ã�RdKafka::Conf �����ýӿ��࣬�������������ߡ������ߡ�broker�ĸ�������ֵ��

*/
class SIMPLEKAFKA_API SimpleKafkaProducer {
public:
    explicit SimpleKafkaProducer(const std::string& brokers, const std::string& topic, int partition);       //epplicit����ֹ��ʽת�������粻��ͨ��string�Ĺ��캯��ת����һ��broker
    ~SimpleKafkaProducer();

    bool Init();
    bool SendMessage(const std::string& msg, const std::string& key);
    void Stop();

protected:
    std::string     m_brokers;  //IP+�˿� 127.0.0��8080
    std::string     m_topicStr;  //topic 
    int             m_partition;  //����

    RdKafka::Conf* m_config;           //RdKafka::Conf --- ���ýӿ��࣬�������ö� �����ߡ������ߡ�broker�ĸ�������ֵ
    RdKafka::Conf* m_topicConfig;

    RdKafka::Producer* m_producer;
    RdKafka::Topic* m_topic;

    RdKafka::DeliveryReportCb* m_dr_cb;            //RdKafka::DeliveryReportCb �����ڵ��� RdKafka::Producer::produce() �󷵻ط��ͽ����RdKafka::DeliveryReportCb��һ���࣬��Ҫ����������еĻص������������ؽ���ķ�ʽ
    RdKafka::EventCb* m_event_cb;         //RdKafka::EventCb ���ڴ�librdkafka��Ӧ�ó��򴫵�errors,statistics,logs ����Ϣ��ͨ�ýӿ�
    RdKafka::PartitionerCb* m_partitioner_cb;   //Rdkafka::PartitionerCb �����趨�Զ��������

};




/*
Kafka������produce()֮�󷵻ط��ͽ��ʱ���� DeliveryReportCb::dr_cb(), ���������䵽message��
*/
class SIMPLEKAFKA_API ProducerDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message& message) {  //���ػ���RdKafka::DeliveryReportCb�е��麯��dr_cb()
        if (message.err() != 0) {       //���ͳ���
            std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
        }
        else {                             //���ͳɹ�
            std::cerr << "Message delivered to topic: " << message.topic_name()
                << " [" << message.partition()
                << "] at offset " << message.offset() << std::endl;
        }
    }
};


/*
EventCb �����ڴ�librdkafka��Ӧ�ó��򷵻�errors,statistics, logs����Ϣ��ͨ�ýӿڣ�
*/
class SIMPLEKAFKA_API ProducerEventCb : public RdKafka::EventCb {
public:
    void event_cb(RdKafka::Event& event) {
        switch (event.type()) {
        case RdKafka::Event::EVENT_ERROR:
            std::cout << "RdKafka::EVENT::EVENT_ERROR: " << RdKafka::err2str(event.err()) << std::endl;
            break;
        case RdKafka::Event::EVENT_STATS:
            std::cout << "RdKafka::EVENT::EVENT_STATS: " << event.str() << std::endl;
            break;
        case RdKafka::Event::EVENT_LOG:
            std::cout << "RdKafka::EVENT::EVENT_LOG: " << event.fac() << std::endl;
            break;
        case RdKafka::Event::EVENT_THROTTLE:
            std::cout << "RdKafka::EVENT::EVENT_THROTTLE: " << event.broker_name() << std::endl;
            break;
        }

    }
};




/*
�û�ʵ�ֵ�����������partitioner_cb() ���������Ҳ��Ҫ�ṩ��Kafkaȥ���õģ����в��� partition_cnt ������Producerָ��������Kafka����Topic����ʱ����Ϣȥ��ѯ��
��Kafka�ϵ�Topic����Ҳ������Producer�����߿ͻ��˴����ģ�Ŀǰ��֪�ķ���ֻ��ʹ�� kafka-topics.sh �ű���һ�ַ�����

���ڡ��������⡱��������
-- ���broker�����ò��� auto.create.topics.enable ����Ϊtrue��Ĭ��ֵΪtrue����
�� ô����������һ����δ���������ⷢ����Ϣʱ�����Զ�����һ��������Ϊ num.partitions��Ĭ��ֵΪ1����������ΪΪ default.replication.factor��Ĭ��ֵΪ1�������⡣
-- ����֮�⣬��һ�������߿�ʼ��δ֪�����ж�ȡ��Ϣʱ�����ߵ�����һ���ͻ�����δ֪���ⷢ��Ԫ��������ʱ�����ᰴ�����ò��� num.partitions �� default.replication.factor��ֵ����һ����Ӧ���⡣
*/
class SIMPLEKAFKA_API HashPartitionerCb : public RdKafka::PartitionerCb {       //�Զ��������߷����������þ��Ƿ���һ������id��  ��key����Hashֵ���õ������͵ķ����ţ���ʵ���Ĭ�ϵķ��������㷽ʽ��һ���ģ�
public:
    int32_t partitioner_cb(const RdKafka::Topic* topic, const std::string* key,
        int32_t partition_cnt, void* msg_opaque)
    {
        char msg[128] = { 0 };
        sprintf_s(msg, "HashPartitionCb:[%s][%s][%d]", topic->name().c_str(), key->c_str(), partition_cnt);
        std::cout << msg << std::endl;

        //ǰ��Ĳ���ֻ��Ϊ���ڷ������ص��д�ӡ��һ�д�ӡ�������������Ĳ�����������generate_hash������һ�������͵ķ���ID
        return generate_hash(key->c_str(), key->size()) % partition_cnt;
    }
private:
    static inline unsigned int generate_hash(const char* str, size_t len) {
        unsigned int hash = 5381;
        for (size_t i = 0; i < len; i++) {
            hash = ((hash << 5) + hash) + str[i];
        }
        return hash;    //����ֵ������ 0 �� partition_cnt ֮�䡣��������򷢻� PARTITION_UA��-1��
    }
};





