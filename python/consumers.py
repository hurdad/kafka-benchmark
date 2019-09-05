#!/usr/bin/python
## Copyright [2019] <Alexander Hurd>"


import time
import signal
import logging

from python import consumer_thread

m_shutdown = 0  # global shutdown variable
logger = logging.getLogger(__name__)


# signal shutdown when receive SIGHUP/SIGINT
def receive_signal(signalNumber, frame):
    logger.info('Signal Received : {}'.format(signalNumber))
    global m_shutdown
    m_shutdown = 1
    return


def str_to_bool(s):
    if s == 'True':
        return True
    elif s == 'False':
        return False
    else:
        raise ValueError


def main():
    # configure logging
    logging.basicConfig(format="%(asctime)s [%(name)s:%(lineno)d][%(funcName)s][%(levelname)s] %(message)s")

    # enable debugging
    logger.setLevel(logging.DEBUG)
    logging.getLogger('ProducerThread').setLevel(logging.DEBUG)

    # register the signals to be caught
    signal.signal(signal.SIGHUP, receive_signal)
    signal.signal(signal.SIGINT, receive_signal)

    # benchmark params
    kafka_url = "localhost:9092"
    num_consumers = 4

    # array to hold threads
    threads = []

    for x in range(0, num_consumers):
        thr = consumer_thread.ConsumerThread(str(x), kafka_url, '3')
        thr.start()
        threads.append(thr)

    #  main loop
    while not m_shutdown:
        time.sleep(0.5)

    for x in threads:
        x.join()


if __name__ == '__main__':
    main()
