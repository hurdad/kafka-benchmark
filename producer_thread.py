#!/usr/bin/python
## Copyright [2019] <Alexander Hurd>"

import threading
import time
import logging
import numpy
from kafka import KafkaProducer

logger = logging.getLogger(__name__)


class ProducerThread(threading.Thread):

    def __init__(self, topic, kafka_url, msg_bytes_size, name='ProducerThread'):
        self._stopevent = threading.Event()
        threading.Thread.__init__(self, name=name)

        self._topic = topic
        self._kafka_url = kafka_url
        self._msg_bytes_size = msg_bytes_size

        self._producer = KafkaProducer(bootstrap_servers=kafka_url)

    def run(self):
        start = time.time()
        cnt = 0
        while not self._stopevent.isSet():

            self._producer.send(self._topic, numpy.random.bytes(self._msg_bytes_size))
            cnt += 1
            if cnt == 5:
                elapsed_time = time.time() - start
                print("%s : %0.2f rate/s : %0.2f bytes/s" % (self._topic, cnt / elapsed_time, self._msg_bytes_size*cnt/elapsed_time))
                cnt = 0
                start = time.time()

            time.sleep(1)

    def join(self, timeout=None):
        self._stopevent.set()
        threading.Thread.join(self, timeout)
