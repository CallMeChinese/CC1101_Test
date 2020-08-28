from enum import Enum, unique
import numpy as np

@unique
class State(Enum):
    SHUTDOWN = 0
    INITIALIZING = 1
    WORKING = 2

class processor():
    def __init__(self, q):
        self.buffer_size = 250
        self.buffer = np.zeros(shape=(self.buffer_size, 4))
        self.item_num = 0
        self.status = State.INITIALIZING
        self.max_diff = []
        self.diff_buffer = np.zeros(shape=(self.buffer_size - 1, 4))
        self.index = np.zeros(shape=(self.buffer_size, 1))
        self.q = q
        self.show_buffer = np.zeros(shape=(self.buffer_size, 4))

    def write_buffer(self):
        self.q.put(self.show_buffer[:self.item_num], block=False)

    def process(self, packet):
        if self.status == State.INITIALIZING:
            self.buffer[self.item_num] = packet[1:]
            self.show_buffer[self.item_num] = packet[1:]
            self.index[self.item_num] = packet[0]

            if self.item_num > 0:
                self.diff_buffer[self.item_num - 1] = np.abs(self.buffer[self.item_num] - self.buffer[self.item_num - 1])

            self.item_num = self.item_num + 1

            if self.item_num == self.buffer_size:
                self.status = State.WORKING
                self.max_diff = np.max(self.diff_buffer, axis=0)
            return 3

        elif self.status == State.WORKING:
            diff = np.abs(packet[1:] - self.buffer[self.buffer_size - 1])
            self.show_buffer = np.roll(self.show_buffer, -1, axis=0)
            self.show_buffer[self.buffer_size - 1] = packet[1:]
            flag = [0, 0, 0, 0]

            for i in range(4):
                if diff[i] <= 1.05 * self.max_diff[i]:
                    flag[i] = 1

            valid_path_num = flag.count(1)

            if valid_path_num < 2:
                print("Attacker detected.")
                return 2
            else:
                self.buffer = np.roll(self.buffer, -1, axis=0)
                self.diff_buffer = np.roll(self.diff_buffer, -1, axis=0)
                self.index = np.roll(self.index, -1, axis=0)
                self.buffer[self.buffer_size - 1] = packet[1:]
                self.index[self.buffer_size - 1] = packet[0]
                self.diff_buffer[self.buffer_size - 2] = diff
                self.max_diff = np.max(self.diff_buffer, axis=0)

                if valid_path_num < 4:
                    print("Valid user, NLoS env.")
                    return 1
                else:
                    print("Valid user, LoS env")
                    return 0
