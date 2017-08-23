# 执行网关注册
```
HW_INT IOTA_Bind(HW_CHAR *pcVerifyCode,ST_IOTA_DEVICE_INFO *pstInfo);
HW_INT IOTA_Login();
HW_INT IOTA_Logout();
```
# 传感器注册

```
HW_INT IOTA_HubDeviceAdd(HW_UINT uiCookie, ST_IOTA_DEVICE_INFO *pstInfo);
HW_INT IOTA_HubDeviceRemove(HW_UINT uiCookie,  HW_CHAR *pucDeviceId);
HW_INT IOTA_DeviceStatusUpdate(HW_UINT uiCookie, HW_CHAR *pcDeviceId, HW_CHAR *pcStatus, HW_CHAR *pcStatusDetail);
```