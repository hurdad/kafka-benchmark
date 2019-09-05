#include "producer-thread.h"

ProducerThread::ProducerThread(std::string topic, std::string kafka_url,
		size_t msg_bytes_size) :
		topic_(topic) {

	// Create the config
	cppkafka::Configuration config = { { "metadata.broker.list", kafka_url } };

	// Create the producer
	producer_ = new cppkafka::Producer(config);

}

ProducerThread::~ProducerThread() {
	delete producer_;
}

void* ProducerThread::run() {

	std::string message = "hey there!";
	size_t flush_cnt = 0;
	size_t cnt = 0;
	auto start = std::chrono::steady_clock::now();


	while (!m_shutdown) {
		producer_->produce(
				cppkafka::MessageBuilder(topic_).partition(0).payload(
						message));
		usleep(5000);

		if (flush_cnt++ == 1000){
			producer_->flush();
			flush_cnt = 0;
		}

		if(cnt++ == 1000){
			auto end = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			QLOG(logINFO) << topic_ << " rate : " << (float)cnt / (float)duration * 1000000 << std::endl;

			cnt = 0;
			start = std::chrono::steady_clock::now();

		}
	}

	return NULL;
}
