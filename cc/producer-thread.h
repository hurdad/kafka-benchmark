
#ifndef PRODUCER_THREAD_H_
#define PRODUCER_THREAD_H_

#include <quantstorm/thread/thread.h>
#include <quantstorm/util/log.h>

#include <cppkafka/cppkafka.h>
#include <string>
#include <chrono>

class ProducerThread : public quantstorm::thread::thread {
 public:
  ProducerThread(std::string topic, std::string kafka_url, size_t msg_bytes_size);
  virtual ~ProducerThread();
  void* run();

 private:
  cppkafka::Producer *producer_;
  std::string topic_;
  size_t msg_bytes_size_;

};

#endif /* PRODUCER_THREAD_H_ */
