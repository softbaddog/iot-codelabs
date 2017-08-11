#ifndef __HOLMER_H__
#define __HOLMER_H__

#include "hw_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define CONFIG_PATH "D:/SDK/01 Holmer/test"
#define CONFIG_PATH "."
#define GATEWAY_BIND_INFO_FILE "gwbindinfo.json"
#define GATEWAY_REG_INFO_FILE  "gwreginfo.json"
#define DEVICE_INFO_FILE      "deviceinfo.json"
#define SENSOR_ID             "nodeId"
#define SENSOR_DEVICE_TYPE    "deviceType"
#define SENSOR_ManufacturerName          "manufacturerName"
#define SENSOR_MANUFACTUREID  "manufacturerId"
#define SENSOR_MODEL          "model"
#define SENSOR_PROTOCOLTYPE   "protocolType"
#define IOTA_CFG_DEVICEID      "DeviceId"
#define IOTA_CFG_DEVICESECRET  "DeviceSecret"
#define IOTA_CFG_SENSORID      "SensorId"
#define IOTA_CFG_APPID         "AppId"
#define IOTA_CFG_IOCM_ADDR     "IoCMAddr"
#define IOTA_CFG_IOCM_PORT     "IoCMPort"
#define IOTA_CFG_IODM_ADDR     "IoDMAddr"
#define IOTA_CFG_IODM_PORT     "IoDMPort"
#define IOTA_CFG_MQTT_ADDR     "MQTTAddr"
#define IOTA_CFG_MQTT_PORT     "MQTTPort"
#define IOTA_SERVICE_BATTERY        "Battery"
#define IOTA_SERVICE_LEDCHAR		"LedChar"
#define IOTA_SERVICE_CHARROLL		"CharRoll"
#define IOTA_AR_ESN_INI_FILE    "esnfilepath"
#define IOTA_PLATFORM_ADDR      "platformaddr"
#define IOTA_PLATFORM_PORT      "platformport"
#define IOTA_MANUFACTURE_ID     "manufactureid"
#define IOTA_DEVICE_TYPE        "devicetype"
#define IOTA_MODEL              "model"
#define IOTA_PROTOCOL_TYPE      "protocoltype"
#define IOTA_LOGLEVEL           "loglevel"
#define METHOD_REMOVE_GATEWAY   "REMOVE_GATEWAY"
#define BUFF_MAX_LEN              200
#define MAX_REQUEST_ID_POSTFIX    9999
/** Indicates the type of gateway parameters. */
typedef struct stru_ST_GATEWAY_INFO
{
    HW_CHAR *pcDeviceID;    /**< Indicates the platform of the DeviceId. */  
    HW_CHAR *pcName;        /**< Indicates the Device name. */
    HW_CHAR *pcSecret;      /**< Indicates the platform of the Secret. */   
    HW_CHAR *pcAppID;       /**< Indicates the developer application ID. */
    HW_CHAR *pcIOCMAddr;    /**< Indicates the IOCM address. */
    HW_UINT pcIOCMPort;    /**< Indicates the IOCM port. */
    HW_CHAR *pcIODMAddr;    /**< Indicates the IODM address. */
    HW_UINT pcMqttPort;    /**< Indicates the MQTT protocol port. */
    HW_UINT pcHttpPort;    /**< Indicates the HTTP protocol port. */
    HW_BOOL uiLoginFlg;    
    HW_UINT uiLogLevel;
    HW_BOOL uiRmvedFlg;

}ST_GATEWAY_INFO;



typedef struct stru_UUID_STR
{
    HW_UINT uiMsgIdHigh;
    HW_UINT uiMsgIdLow;
    HW_UINT uiMsgSeqHigh;
    HW_UINT uiMsgSeqLow;
}UUID_STR;
HW_INT Gateway_UnbindRecvtHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_RegResultHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_ConnectedHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_DisconnectHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_AddResultHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_RemovResultHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_ServiceDataReportResultHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_ServiceCommandReceiveHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_INT Device_DevUpDateHandler(HW_UINT uiCookie, HW_MSG pstMsg);
HW_VOID DEVICE_ReadConf();
HW_UINT DEVICE_BindGateWay();
HW_VOID IOTA_RmvGateWay();
HW_INT Gateway_ReportBattery( HW_CHAR **pcJsonStr);
HW_INT Device_ReportIdx( HW_CHAR **pcJsonStr);
HW_INT Device_ServiceDataReport(HW_CHAR *pcSensorDeviceID,HW_CHAR *pcServiceID, HW_CHAR *pcJsonStr);
HW_VOID HW_GetUUID(HW_CHAR *pcUUID);
HW_VOID HW_GetRequestId(HW_CHAR *pcRequestId);
HW_UINT HW_GeneralCookie();
HW_VOID DEVICE_InitGateWayInfo();
HW_VOID AddSensors();


#ifdef __cplusplus
}
#endif

#endif /* __GATEWAY_H__ */
