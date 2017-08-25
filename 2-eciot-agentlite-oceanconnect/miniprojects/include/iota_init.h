/**
 * @file IOTA_init.h
 * Indicates IOTA init Interface Functions
 */
#ifndef _IOTA_INIT_H_
#define _IOTA_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif


HW_API_FUNC HW_INT IOTA_Init(HW_CHAR *pcWorkPath, HW_CHAR *pcLogPath);
HW_API_FUNC HW_VOID IOTA_Destroy();
HW_API_FUNC HW_VOID HW_SysDestroy();

#ifdef __cplusplus
}
#endif

#endif

