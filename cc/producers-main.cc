#include <iostream>

#include "signal.hpp"
#include "producer-thread.h"

int main(int argc, char* argv[]) {

	// Register signal and signal handler (Ctrl+c / kill $pid)
	signal(SIGINT, signal_callback_handler);
	signal(SIGTERM, signal_callback_handler);

	//benchmark params
	std::string kafka_url = "localhost:9092";
	size_t num_producers = 4;
	size_t msg_bytes_size = 500;

	// start threads
	std::vector<ProducerThread*> threads;
	for (size_t i = 0; i < num_producers; i++) {
		ProducerThread* thr = new ProducerThread(std::to_string(i), kafka_url,
				msg_bytes_size);
		if (thr != NULL && thr->start() != 0) {
			throw std::runtime_error("Thread failed to start!");
		}
		threads.push_back(thr);
	}

	//  main loop
	while (!s_shutdown) {
		sleep(1);
	}

	// clean up threads
	for (size_t i = 0; i < num_producers; i++) {
		ProducerThread* thr = threads[i];
		thr->signal_shutdown();
		thr->join();
		delete thr;
	}

	std::cout << "Exiting.." << std::endl;
	return EXIT_SUCCESS;

}
