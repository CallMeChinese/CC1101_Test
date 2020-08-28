# -*- coding:utf-8 -*-

import serial, struct
import process, sys
import guitest
from multiprocessing import Process, Queue
from matplotlib.backends.qt_compat import QtWidgets

def show_module(q):
    qapp = QtWidgets.QApplication(sys.argv)
    app1 = guitest.DynamicWindow(q)
    app1.show()
    qapp.exec_()

if __name__ == "__main__":
    # 这里Windows使用"COM"，Linux使用"/dev/ttyUSB"
    #serialName = ['COM10', 'COM11', 'COM12', 'COM13']
    #serialName = ['COM10', 'COM11']
    serialName = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3']
    serialList = []
    for name in serialName:
        # 同理，Windows使用win32，Linux使用posix
        #ser = serial.serialwin32.Serial(name)
        ser = serial.serialposix.Serial(name)
        serialList.append(ser)
    serialCnt = len(serialList)
    # List of List of tuple, which is (packageIndex, rssi)
    recvPacket = [[]]*serialCnt

    q = Queue()
    processer = process.processor(q)
    show = Process(target=show_module, args=(q,))
    show.start()
    count = 0

    myFile = open("test.log", 'w')
    try:
        while True:
            for i in range(serialCnt):
                # FIXME: It will block process here, multiprocess should be used
                data = serialList[i].read(5)
                recvPacket[i].append(struct.unpack(">Ib", data))

            """
                synchronize data process

                e.g

                device        data stream ------->
                                space means lost
                recvPacket[0]: ---- --|-- --------
                recvPacket[1]:    -- -| ----- -------
                recvPacket[2]:        |---- ---- -----
                recvPacket[3]:   -- --|-- ---------
                                ^        ^
                                |        |
                            useless   useful

                因此每次寻找队列中首个信息包的最大编号，编号小于这个的信息都是无效的
            """
            maxIndex = max(
                recvPacket[0][0][0],
                recvPacket[1][0][0],
                recvPacket[2][0][0],
                recvPacket[3][0][0])
            for i in range(serialCnt):
                j = 0
                for j in range(len(recvPacket[i])):
                    if recvPacket[i][j][0] < maxIndex:
                        continue
                    else:
                        break
                recvPacket[i] = recvPacket[i][j:]

            if (len(recvPacket[0]) == 0 or
                len(recvPacket[1]) == 0 or
                len(recvPacket[2]) == 0 or
                len(recvPacket[3]) == 0):
                # 如果队列为空，就没有获得信息的必要
                continue
            else:
                # 如果消息都不为空
                if (recvPacket[0][0][0] != recvPacket[1][0][0] or
                    recvPacket[1][0][0] != recvPacket[2][0][0] or
                    recvPacket[2][0][0] != recvPacket[3][0][0]):
                    # 如果一开始的编号不同，那么就还要再剪裁，例如上面那个例子，需要剪裁两次
                    # TODO: Something should be done here, because it may be always wrong if it occurs
                    #print("Lost packet!")
                    continue
                else:
                    count += 1
                    data = [
                        recvPacket[0][0][0],
                        recvPacket[0][0][1],
                        recvPacket[1][0][1],
                        recvPacket[2][0][1],
                        recvPacket[3][0][1]
                    ]
                    #print(data)
                    for i in range(serialCnt):
                        # XXX: Why not pop(0)? Maybe it raise an error when list is empty
                        recvPacket[i] = recvPacket[i][1:]
                    result = processer.process(data)
                    myFile.write("%d  %s\n"%(result, str(data)))
                    myFile.flush()
                    if count%20 == 0:
                        processer.write_buffer()
    except:
        myFile.close()
        show.terminate()

