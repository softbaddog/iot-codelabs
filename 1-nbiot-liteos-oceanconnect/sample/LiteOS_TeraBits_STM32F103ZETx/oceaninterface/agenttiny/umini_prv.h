/* ========================================================================== *
 *                                                                            *
 *         Copyright (C) 2014-2016 Huawei Technologies Co., Ltd.              *
 *                         All rights reserved.                               *
 *                                                                            *
 * THIS SOFTWARE IS PROVIDED BY Huawei Technologies Co., Ltd. ``AS IS'' AND   *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE *
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE    *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    *
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT *
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY  *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF     *
 * SUCH DAMAGE.                                                               *
 *                                                                            *
 * ========================================================================== */

#ifndef __UMINI_PRV_H__
#define __UMINI_PRV_H__
#include  "umini.h"
//#include "umini_coap_prv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMINI_BUF_MAX_CNT      5
#define UMINI_BUF_MAX_LEN    1500
#define UMINI_BUF_RETRY_TIMES  5

typedef enum enum_UMINI_STATE
{
    EN_UMINI_STA_IDLE          = 0,
    EN_UMINI_STA_BIND          = 1,
    EN_UMINI_STA_REGING        = 2,
    EN_UMINI_STA_ACTIVE        = 3,
    EN_UMINI_STA_BUTT
} EN_UMINI_STATE;

typedef struct stru_UMINI_BUFF
{
    UM_UINT8 u8Flag;
    UM_SIZE  uiBufLen;
    UM_UINT8 aucBuf[UMINI_BUF_MAX_LEN];
} ST_UMINI_BUFF;

typedef struct stru_UMINI_MGR
{   
	  
    UM_UINT8  u8LoginFlag;
	  UM_UINT8  u8BindFlag;
    UM_SOCK   hTransSock;
    UM_UINT8 *pucServerIp;
    UM_UINT16 usServerPort;
    ST_UMINI_BUFF stReportBuf[UMINI_BUF_MAX_CNT];
} ST_UMINI_MGR;

UM_UINT8 UMini_Login(UM_VOID);

#ifdef __cplusplus
}
#endif

#endif /* __UMINI_PRV_H__ */

