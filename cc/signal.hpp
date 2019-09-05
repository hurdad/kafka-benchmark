#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include <signal.h>
#include <quantstorm/util/log.h>

static int s_shutdown = 0; // static shutdown variable
static void signal_callback_handler(int signum) {
  QLOG(logINFO) << "Caught shutdown signal: " << signum << std::endl;

  // Handle repeat signals here
  if (s_shutdown) {
    QLOG(logINFO) << "Please wait..." << std::endl;
    return;
  }

  // Stop main loop
  s_shutdown = 1;
}




#endif /* SIGNAL_HPP_ */
