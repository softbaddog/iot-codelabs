/* ========================================================================== *
 *                                                                            *
 *        Copyright (C) 2014-2016 Huawei Technologies Co., Ltd.               *
 *                              All rights reserved.                          *
 * THIS SOFTWARE IS PROVIDE BY Huawei Technologies Co., Ltd. `` AS IS"" AND   *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE     *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    *
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT *
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY  *
 * OUT OF THE USE OF THIS SOFTWARE , EVEN IF ADVISED OF THE POSSIBILITY OF    * 
 * SUCH DAMAGE.                                                               *
 *                                                                            *
 * ========================================================================== */
 
#ifndef __UMINI_H__
#define __UMINI_H__
 
#include "coap.h"
#ifdef __cplusplus
extern "C" {
#endif

#define _UM_OS _UM_LITEOS
#ifndef _UM_OS
#error "Unsupported os, please define _UM_OS!"
#endif

//#define _UM_DEBUG

/* to check the hdr is full with the lib*/
#define _UM_VER_ 1

#define _UM_WINDOWS 	1
#define _UM_ANDROID 	2
#define _UM_IOS			3
#define _UM_LINUX		4
#define _UM_VXWORKS		5
#define _UM_WINPHONE 	6
#define _UM_LITEOS		7

#if ((_UM_OS == _UM_WINDOWS) || (_UM_OS == _UM_WINPHONE))
#define _UM_EXPORT
#define _UM_FILE_	__FILE__
#define _UM_FUNC_	__FUNCTION__
#define _UM_LINE_	__LINE__
#else
#define _UM_FILE_	__FILE__
#define _UM_FUNC_	__func__
#define _UM_LINE_	__LINE__
#ifdef __LP64__
	#define _UM_64BIT
#endif
#endif

#ifdef _UM_EXPORT
#define UM_API_FUNC __declspec(dllexport)
#elif defined(_UGP_IMPORT)
#define UM_API_FUNC __declspec(dllimport)
#else
#define UM_API_FUNC
#endif

#ifdef _UM_CONST
#define UM_CONST	const
#else
#define UM_CONST
#endif

#define UM_STATIC	static

#define TOKLENMAX	8
#define IPLENMAX	32
#define PORTLENMAX	8
#define DEVICESN	32

typedef void	UM_VOID;
typedef char 	UM_CHAR;
typedef int 	UM_INT;

typedef unsigned char 	UM_UINT8;
typedef unsigned short	UM_UINT16;
typedef unsigned int	UM_SIZE;
typedef unsigned int	UM_UINT32;

typedef int				UM_SOCK;
typedef void *			UM_COAPPKT;

#define UM_NULL			(0)

typedef UM_UINT8		UM_RET;
#define UM_OK			0
#define UM_ERR			1
#define UM_ERR_OVERFLOW	2
#define UM_ERR_VERSION	3
#define UM_ERR_DECODE	4
#define UM_CON			5
#define UM_NON			6

typedef UM_VOID (*PFN_DATAWRITE)(UM_UINT32 flashAddr, UM_UINT8 *pucData, UM_UINT32 uiLen);//写flash/eeprom等数据块
typedef UM_UINT8 *(*PFN_DATAREAD)(UM_UINT32 flashAddr, UM_UINT32 uiLen);//读flash/eeprom等数据块
typedef UM_VOID (*PFN_GETSN)(UM_UINT8 *pucData, UM_UINT8 *uiLen);//获取设备mac、sn等
typedef UM_VOID (*PFN_RECVCMD)(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen);//tiny接收到控制命令后传给设备应用的回调
typedef UM_VOID (*PFN_SENDDATA)(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen);
typedef UM_VOID (*PFN_ONRECV)(UM_UINT8 nType, UM_UINT8 *pucData, UM_SIZE nLen);

//CoAP
typedef UM_COAPPKT (*PFN_COAP_PKT_MALLOC)(UM_VOID);
typedef UM_VOID (*PFN_COAP_PKT_FREE)(UM_COAPPKT);
typedef UM_INT (*PFN_SET_COAP_HEADER)(UM_COAPPKT pkt, UM_UINT8 ver, UM_UINT8 t, UM_UINT8 tkl, UM_UINT8 code);
//&pkt->opts[0], &delta, &p, option_len
typedef UM_INT (*PFN_SET_COAP_OPTION)(UM_COAPPKT pkt, UM_UINT8 optsIndex, UM_UINT16 delta, UM_UINT8 *p, UM_UINT8 plen);
typedef UM_INT (*PFN_SET_COAP_PAYLOAD)(UM_COAPPKT pkt, UM_UINT8 *p, UM_UINT16 plen);
typedef UM_INT (*PFN_SET_COAP_TOKEN)(UM_COAPPKT pkt, UM_UINT8 *pToken, UM_UINT8 pTokenLen);
typedef UM_UINT32 (*PFN_GET_RANDOM)(UM_VOID);
typedef UM_INT (*PFN_GET_TOKEN)(UM_COAPPKT pkt, UM_UINT8 *pToken, UM_UINT8 *pTokenlen);
typedef UM_INT (*PFN_COAP_BUILD)(UM_UINT8 *buf, size_t *buflen, UM_COAPPKT *pkt);
//=========(2)=========数据结构
typedef struct _board_callback_t
{
	PFN_GETSN pfnGetSN;//获取设备mac、sn等
	PFN_DATAWRITE pfnDataWrite;//写flash/eeprom等数据块
	PFN_DATAREAD pfnDataRead;//读取flash/eeprom等数据块
	PFN_RECVCMD pfnRecvCmd;//Tiny接收到控制命令后传给设备应用的回调
	PFN_SENDDATA pfnSendData;//
	UM_SIZE flashAddr;
	UM_UINT16 uiLength;
	UM_INT iSocket;

}board_callback_t;

typedef struct _coap_callback_t
{
	PFN_COAP_PKT_MALLOC pfnCoapPktMalloc;
	PFN_COAP_PKT_FREE pfnCoapPktfree;
	PFN_SET_COAP_HEADER pfnSetCoapHeader;
	PFN_SET_COAP_OPTION pfnSetCoapOption;
	PFN_SET_COAP_PAYLOAD pfnSetCoapPayload;
	PFN_SET_COAP_TOKEN pfnSetCoapToken;
	PFN_GET_RANDOM pfnGetRandom;
	PFN_GET_TOKEN pfnGetToken;
	PFN_COAP_BUILD pfnCoapBuild;
}coap_callback_t;

UM_API_FUNC UM_RET UMini_Initial(board_callback_t boardfunc, coap_callback_t coapfunc);//初始化
UM_API_FUNC UM_RET UMini_Report(unsigned char *pucData, unsigned int uiLen);//上报数据，设备应用不感知bind login
UM_API_FUNC UM_RET NetRecv(uint8_t *buff, unsigned int uiLen);//网络接收数据后处理（中断调用或设备的网络的接收线程调用）

UM_API_FUNC UM_RET UMini_NetRecv(uint8_t t, uint8_t code, uint8_t delta, const uint8_t *pToken, uint8_t pTokenLen);

UM_API_FUNC UM_VOID TimerPro(UM_VOID);//10秒定时， 不断尝试bind, 设备应用起定时器调用。

#ifdef __cplusplus
}
#endif

#endif /* __UMINI_H__ */
