import sys
import time
import numpy as np
import queue

from matplotlib.backends.qt_compat import QtCore, QtWidgets, is_pyqt5
if is_pyqt5():
    from matplotlib.backends.backend_qt5agg import (
        FigureCanvas, NavigationToolbar2QT as NavigationToolbar)
else:
    from matplotlib.backends.backend_qt4agg import (
        FigureCanvas, NavigationToolbar2QT as NavigationToolbar)
from matplotlib.figure import Figure


class StaticWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self._main = QtWidgets.QWidget()
        self.setCentralWidget(self._main)
        layout = QtWidgets.QHBoxLayout(self._main)

        static_canvas = FigureCanvas(Figure(figsize=(10, 6), dpi=70))
        text=QtWidgets.QTextBrowser(self._main)

        layout.addWidget(static_canvas,stretch=2)
        layout.addWidget(text,stretch=1)
        vlayout=QtWidgets.QVBoxLayout(self._main)
        vlayout.addLayout(layout)
        self.setLayout(vlayout)
        self.addToolBar(NavigationToolbar(static_canvas, self))

        self._static_axis = static_canvas.figure.subplots(2,2)
        t = np.linspace(0, 10, 501)
        self._static_axis[0, 0].plot(t, np.tan(t+time.time()), ".")
        self._static_axis[0, 1].plot(t, np.tan(t+time.time()), ".")
        self._static_axis[1, 0].plot(t, np.tan(t+time.time()), ".")
        self._static_axis[1, 1].plot(t, np.tan(t+time.time()), ".")
        self._static_axis[0, 0].set_title('rssi1')
        self._static_axis[0, 1].set_title('rssi2')
        self._static_axis[1, 0].set_title('rssi3')
        self._static_axis[1, 1].set_title('rssi4')
        self.setWindowTitle('Static')


class DynamicWindow(QtWidgets.QMainWindow):
    def __init__(self, q):
        super().__init__()
        self.q = q
        self._main = QtWidgets.QWidget()
        self.setCentralWidget(self._main)
        layout = QtWidgets.QVBoxLayout(self._main)

        dynamic_canvas = FigureCanvas(Figure(figsize=(10, 6)))
        layout.addWidget(dynamic_canvas)
        self.addToolBar(QtCore.Qt.BottomToolBarArea,
                        NavigationToolbar(dynamic_canvas, self))
        self.setWindowTitle('rssi')
        self._dynamic_ax = dynamic_canvas.figure.subplots(2, 2)
        self._dynamic_ax[0, 0].set_title('rssi1')
        self._dynamic_ax[0, 1].set_title('rssi2')
        self._dynamic_ax[1, 0].set_title('rssi3')
        self._dynamic_ax[1, 1].set_title('rssi4')
        self._timer = dynamic_canvas.new_timer(
            100, [(self._update_canvas, (), {})])
        self._timer.start()

    def _update_canvas(self):
        global t
        try:
            t = self.q.get(block=False)
        except queue.Empty:
            return

        else:
            self._dynamic_ax[0, 0].clear()
            self._dynamic_ax[0, 1].clear()
            self._dynamic_ax[1, 0].clear()
            self._dynamic_ax[1, 1].clear()
            self._dynamic_ax[0, 0].set_ylim((0, 120))
            self._dynamic_ax[0, 1].set_ylim((0, 120))
            self._dynamic_ax[1, 0].set_ylim((0, 120))
            self._dynamic_ax[1, 1].set_ylim((0, 120))

            for i in range(2):
                for j in range(2):
                    self._dynamic_ax[i, j].plot(t[...,2*i+j])
                    #self._dynamic_ax[i, j].scatter(t[..., 2*i+j])
                    self._dynamic_ax[i, j].figure.canvas.draw()
