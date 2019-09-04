#!/usr/bin/python
## Copyright [2019] <Alexander Hurd>"

import threading
import logging
import time
from kafka import KafkaConsumer

logger = logging.getLogger(__name__)


class ConsumerThread(threading.Thread):

    def __init__(self, topic, kafka_url, group_id, name='ConsumerThread'):
        self._stopevent = threading.Event()
        threading.Thread.__init__(self, name=name)

        self._topic = topic
        self._kafka_url = kafka_url

        self._consumer = KafkaConsumer(topic, group_id=group_id, bootstrap_servers=kafka_url)

    def run(self):
        start = time.time()
        cnt = 0
        for msg in self._consumer:
            if self._stopevent.isSet():
                return

            cnt += 1
            if cnt == 5:
                elapsed_time = time.time() - start
                print("%s : %0.2f rate/s" % (self._topic, cnt / elapsed_time))
                cnt = 0
                start = time.time()

    def join(self, timeout=None):
        self._stopevent.set()
        threading.Thread.join(self, timeout)
