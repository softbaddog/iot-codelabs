基于边缘计算网关开放边缘基础服务API实现轻量级应用接入OceanConnect平台方案介绍

场景说明：

- 边缘计算网关支持虚拟化容器技术，开发者可以在容器（Docker）内实现轻量级边缘计算应用；
- 通过调用网关内封装的边缘基础服务API，实现对RS232/485， DI/DO，CANBUS，蓝牙等多种数据进行本地数据预分析处理，优化。然后调用内置的AgentLite组件，将数据上传到IoT平台或者业务系统；
- 该场景充分利用了边缘计算网关基础服务API和平台AgentLite组件，让开发者快速实现采集数据边缘计算业务开发。

![OceanConnct+EC-IoT](http://developer.huawei.com/ict/cn/hdc/assert/img/iot_2.png)
