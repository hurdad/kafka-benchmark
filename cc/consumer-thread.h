
#ifndef CONSUMER_THREAD_H_
#define CONSUMER_THREAD_H_


#include <quantstorm/thread/thread.h>
#include <quantstorm/util/log.h>

#include <cppkafka/cppkafka.h>
#include <string>
#include <chrono>

class ConsumerThread : public quantstorm::thread::thread {
 public:
  ConsumerThread(std::string topic, std::string kafka_url, std::string group_id);
  virtual ~ConsumerThread();
  void* run();

 private:
  cppkafka::Consumer *consumer_;
  std::string topic_;

};



#endif /* CONSUMER_THREAD_H_ */
