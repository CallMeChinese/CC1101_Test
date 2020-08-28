# CC1101使用方法

分为硬件和软件部分

## 硬件部分

代码请见<code>Hardware</code>文件夹下面，使用的是STVD开发工具，编译生成好，直接烧写进去就好了

## 软件部分

代码见<code>Software</code>文件夹下面，使用的是Python

### 环境需要

+ Python 3
  + pyserial
  + PyQt5
  + matplotlib

### 运行方法

1. 将4个RX连接到PC上，查看端口号
2. 根据查到的端口号修改main.py中的serialName变量、
3. 开始运行main.py
4. 给TX供电，使得TX开始发送信息

步骤3和4可以颠倒，但是可能会丢失已经发送但是没有传送到电脑上的数据
