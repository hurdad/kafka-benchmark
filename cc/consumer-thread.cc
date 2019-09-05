#include "consumer-thread.h"

ConsumerThread::ConsumerThread(std::string topic, std::string kafka_url,
		std::string group_id) :
		topic_(topic) {

	// Construct the configuration
	cppkafka::Configuration config = {
			{ "metadata.broker.list", kafka_url },
			{ "group.id", group_id },
			// Disable auto commit
			{ "enable.auto.commit", false }
	};

	// Create the consumer
	consumer_ = new cppkafka::Consumer(config);
}

ConsumerThread::~ConsumerThread() {
	delete consumer_;
}

void* ConsumerThread::run() {

	// Print the assigned partitions on assignment
	consumer_->set_assignment_callback([](const cppkafka::TopicPartitionList& partitions) {
		QLOG(logINFO) << "Got assigned: " << partitions << std::endl;
	});

	// Print the revoked partitions on revocation
	consumer_->set_revocation_callback([](const cppkafka::TopicPartitionList& partitions) {
		QLOG(logINFO) << "Got revoked: " << partitions << std::endl;
	});

	// Subscribe to the topic
	consumer_->subscribe( { topic_ });

	QLOG(logINFO) << "Consuming messages from topic " << topic_ << std::endl;

	// Now read lines and write them into kafka
	while (!m_shutdown) {
		// Try to consume a message
		cppkafka::Message msg = consumer_->poll();
		if (msg) {
			// If we managed to get a message
			if (msg.get_error()) {
				// Ignore EOF notifications from rdkafka
				if (!msg.is_eof()) {
					QLOG(logERROR) << "[+] Received error notification: "
							<< msg.get_error() << std::endl;
				}
			} else {
				// Print the key (if any)
				if (msg.get_key()) {
					QLOG(logINFO) << msg.get_key() << " -> ";
				}
				// Print the payload
				QLOG(logINFO) << msg.get_payload() << std::endl;
				// Now commit the message
				consumer_->commit(msg);
			}
		}
	}

	return NULL;
}
