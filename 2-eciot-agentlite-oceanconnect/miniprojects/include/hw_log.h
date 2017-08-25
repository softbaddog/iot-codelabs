/**
 * @file hw_log.h
 */
#ifndef _HW_LOG_H_
#define _HW_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif
HW_API_FUNC HW_INT HW_LOG_ERR(HW_CHAR *pucFormat, ...);
HW_API_FUNC HW_INT HW_LOG_WARN(HW_CHAR *pucFormat, ...);
HW_API_FUNC HW_INT HW_LOG_INF(HW_CHAR *pucFormat, ...);
HW_API_FUNC HW_INT HW_LogSetLevel(HW_UINT uiLogLevels);

#ifdef __cplusplus
}
#endif

#endif
