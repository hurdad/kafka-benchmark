#include "producer-thread.h"

ProducerThread::ProducerThread(std::string topic, std::string kafka_url,
		size_t msg_bytes_size) :
		topic_(topic),
		msg_bytes_size_(msg_bytes_size){

	// Create the config
	cppkafka::Configuration config = { 
		{ "metadata.broker.list", kafka_url } ,
    	{ "compression.codec", "snappy" }
	};

	// Create the producer
	producer_ = new cppkafka::Producer(config);

}

ProducerThread::~ProducerThread() {
	delete producer_;
}

void* ProducerThread::run() {
	size_t flush_cnt = 0;
	size_t cnt = 0;
	auto start = std::chrono::steady_clock::now();

    // build payload vector / buffer
	std::vector<cppkafka::Buffer::DataType> vec(msg_bytes_size_);
	std::generate(vec.begin(), vec.end(), std::rand);
	cppkafka::Buffer payload(vec);

	while (!m_shutdown) {
		producer_->produce(
				cppkafka::MessageBuilder(topic_).partition(0).payload(payload));
		usleep(5000);

		// flush
		if (flush_cnt++ == 1000) {
			try {
				producer_->flush();
			} catch (std::exception& e) {
				QLOG(logERROR) << topic_ << e.what() << std::endl;
			}
			flush_cnt = 0;
		}

		// print rate stats
		if (cnt++ == 1000) {
			auto end = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast
					< std::chrono::microseconds > (end - start).count();
			QLOG(logINFO) << topic_ << " rate : "
					<< (float) cnt / (float) duration * 1000000 << std::endl;

			cnt = 0;
			start = std::chrono::steady_clock::now();
		}
	}

	return NULL;
}
