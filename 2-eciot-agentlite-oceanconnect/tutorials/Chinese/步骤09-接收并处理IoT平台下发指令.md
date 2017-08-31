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
HW_INT Device_ServiceCommandReceiveHandler(HW_UINT uiCookie, HW_MSG pstMsg)
{
    HW_CHAR *pcDevId;
    HW_CHAR *pcReqId;
    HW_CHAR *pcServiceId;
    HW_CHAR *pcMethod;
    HW_BYTES *pbstrContent;
    HW_JSONOBJ jsonObj;
    HW_JSON jsonRoot;
    HW_CHAR pcTemp[256] = {0};

    pcDevId = HW_MsgGetStr(pstMsg,EN_IOTA_DATATRANS_IE_DEVICEID);
    pcReqId = HW_MsgGetStr(pstMsg,EN_IOTA_DATATRANS_IE_REQUESTID);
    pcServiceId = HW_MsgGetStr(pstMsg,EN_IOTA_DATATRANS_IE_SERVICEID);    
    pcMethod = HW_MsgGetStr(pstMsg,EN_IOTA_DATATRANS_IE_METHOD);
    pbstrContent = HW_MsgGetBstr(pstMsg,EN_IOTA_DATATRANS_IE_CMDCONTENT);
    
    if ((HW_NULL == pcDevId)
        ||(HW_NULL == pcReqId)
        ||(HW_NULL == pcServiceId)
        ||(HW_NULL == pcMethod)
        )
    {
        HW_LOG_ERR("RcvCmd is invalid, pcDevId=%s, pcReqId=%s, pcServiceId=%s, pcMethod=%s.",
                    pcDevId, pcReqId, pcServiceId, pcMethod); 
        return HW_ERR;
    }

    HW_LOG_INF(" ---Device_ServiceCommandReceiveHandler--- pcDevId=%s, pcReqId=%s, pcServiceId=%s, pcMethod=%s, pbstrContent=%s.",
        pcDevId, pcReqId, pcServiceId, pcMethod, pbstrContent->pcByte);

    jsonObj = HW_JsonDecodeCreate(pbstrContent->pcByte, HW_TRUE);
    jsonRoot = HW_JsonGetJson(jsonObj);

    //if (0 == strncmp(IOTA_SERVICE_LEDCHAR, pcServiceId, strlen(IOTA_SERVICE_LEDCHAR)))
    if (0 == strncmp("SET_LED_CHAR_IDX", pcMethod, strlen(pcMethod)))
    {
        strcpy(pcTemp,HW_JsonGetStr(jsonRoot, "ledcharidx"));
        g_uiCurLedIdx = atoi(pcTemp);
        HW_LOG_INF(" ---g_uiCurLedIdx be seted %d ---.", g_uiCurLedIdx);
    }
    
    if (0 == strncmp("SET_LED_CHAR_ROLL", pcMethod, strlen(pcMethod)))
    {
        strcpy(pcTemp, HW_JsonGetStr(jsonRoot, "ledcharroll"));
        if (atoi(pcTemp) > HW_FALSE)
        {
            g_bRollStatus = HW_TRUE;
            HW_LOG_INF(" ---g_bRollStatus be seted success ---.");
        }
    }

    if (0 == strncmp(METHOD_REMOVE_GATEWAY,pcMethod,strlen(METHOD_REMOVE_GATEWAY)))
    {
        g_uiLoginFlg = HW_FALSE;
        IOTA_RmvGateWay();
        HW_LOG_INF(" ---IOTA_RmvGateWay success ---.");
    }

    HW_JsonObjDelete(&jsonObj);
   
    return HW_OK;
}
//在设备添加成功后立即注册设备命令接收广播 
HW_BroadCastReg(“IOTA_TOPIC_SERVICE_CMD_RECEIVE/XXXX_XXXX_XXXX_XXXX”, 
Device_AddResultHandler);
```

开发者需要在设备添加成功后注册该设备的命令接收广播，广播主题为
“ IOTA_TOPIC_SERVICE_CMD_RECEIVE/设备ID”，Agent Lite收到平台发往给设备的命令后会直接广播给该设备注册的广播处理函数。如果开发者不需要按设备进行分发，直接使用主题名即可即可，即” IOTA_TOPIC_SERVICE_CMD_RECEIVE”。
