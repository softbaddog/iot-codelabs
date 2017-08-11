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

 /**
 * @file IOTA_login.h
 * Indicates IOTA Login Interface Functions
 */

#ifndef _IOTA_LOGIN_H_
#define _IOTA_LOGIN_H_

#ifdef __cplusplus
extern "C" {
#endif



#define IOTA_TOPIC_CONNECTED_NTY     "IOTA_TOPIC_CONNECTED_NTY"
#define IOTA_TOPIC_DISCONNECT_NTY  "IOTA_TOPIC_DISCONNECT_NTY"


/** Indicates notification parameters */
typedef enum enum_EN_IOTA_LGN_IE_TYPE
{
    EN_IOTA_LGN_IE_REASON = 0, 
} EN_IOTA_LGN_IE_TYPE;
          
typedef enum enum_IOTA_LOGIN_REASON_TYPE
{
    EN_IOTA_LGN_REASON_NULL = 0,//EN_ULGN_REASON_NULL,
    EN_IOTA_LGN_REASON_CONNCET_ERR = 1,//连接失败 = EN_ULGN_REASON_CONNCET_ERR,
    EN_IOTA_LGN_REASON_SERVER_BUSY = 2,//服务器忙 = EN_ULGN_REASON_SERVER_BUSY,
    EN_IOTA_LGN_REASON_AUTH_FAILED = 3,//鉴权失败 = EN_ULGN_REASON_AUTH_FAILED,
    EN_IOTA_LGN_REASON_NET_UNAVAILABLE = 5,//网络不可用 = EN_ULGN_REASON_NET_UNAVAILABLE,
    EN_IOTA_LGN_REASON_DEVICE_NOEXIST  = 12,//设备不存在 = EN_ULGN_REASON_UNREG_USER,
    EN_IOTA_LGN_REASON_DEVICE_RMVED    = 13,//设备已删除 = EN_ULGN_REASON_RMVED_USER,
    EN_IOTA_LGN_REASON_UNKNOWN = 255
}EN_IOTA_LGN_REASON_TYPE;


HW_API_FUNC HW_INT IOTA_Login();
HW_API_FUNC HW_INT IOTA_Logout();

#ifdef __cplusplus
}
#endif

#endif

