# 无NB-IoT网络，实现数据从以太网口上报

上一节中我们介绍到南向设备数据收发，已经提到过两种传输方式：NB网络、AGENTTINY，这里的AGENTTINY就对应以太网传输。

对于数据从以太网上报：

1. 需要在user/ocean_sample.c文件首打开`USE_AGENTTINY`定义，并注释`USE_NB_BC95`。
```
//#define USE_NB_BC95 1
#define USE_AGENTTINY 1
```
2. 在ocean_sample.c文件首修改IP地址，配置本地静态IP、网关、子网掩码、本地mac地址，确保`init_testethernet`以太网测试任务正常通过。

另外，我们用到的SPI转以太网口模块需要相应的驱动程序，Demo工程内已经移植好了，放置在`HAREWARE\enc28j60`下，供参考了解。
