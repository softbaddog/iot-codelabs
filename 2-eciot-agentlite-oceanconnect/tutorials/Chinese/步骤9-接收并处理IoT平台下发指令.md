# 接收并处理IoT平台下发指令

注册设备命令接收广播来接收处理平台下发的控制命令。

**接口描述**

`IOTA_TOPIC_SERVICE_COMMAND_RECEIVE/{deviceId}`

**参数说明**

`EN_IOTA_DATATRANS_IE_TYPE`

**返回结果**

无

```c
// 开发者注册该函数进行命令接收的处理 
HW_INT Switch_CommandRecvtHandler(HW_UINT uiCookie, HW_MSG pstMsg) 
{ 
HW_CHAR *pcMethod, *pcServiceId, *pcCmdContent, *pcDeviceId; 
 
pcDeviceId = HW_MsgGetStr(pstMsg, EN_IOTA_DATATRANS_IE_DEVICEID); 
pcServiceId = HW_MsgGetStr(pstMsg, EN_IOTA_DATATRANS_IE_SERVICEID); 
pcMethod = HW_MsgGetStr(pstMsg, EN_IOTA_DATATRANS_IE_METHOD); 
pcCmdContent = HW_MsgGetStr(pstMsg, EN_IOTA_DATATRANS_IE_CMDCONTENT); 
 
if (strcmp(pcServiceId, “switch”)) 
{ 
//根据Proflie定义的命令参数，使用Json组件解析pcCmdContent 
//Send command to Switch 
} 
 
return 0; 
}
//在设备添加成功后立即注册设备命令接收广播 
HW_BroadCastReg(“IOTA_TOPIC_SERVICE_CMD_RECEIVE/XXXX_XXXX_XXXX_XXXX”, 
Device_AddResultHandler);
```

开发者需要在设备添加成功后注册该设备的命令接收广播，广播主题为
“ IOTA_TOPIC_SERVICE_CMD_RECEIVE/设备ID”，Agent Lite收到平台发往给设备的命令后会直接广播给该设备注册的广播处理函数。如果开发者不需要按设备进行分发，直接使用主题名即可即可，即” IOTA_TOPIC_SERVICE_CMD_RECEIVE”。
