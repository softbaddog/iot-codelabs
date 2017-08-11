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

#ifndef _IOTA_BIND_H_
#define _IOTA_BIND_H_

#include "iota_device_type.h"

#ifdef __cplusplus
extern "C" {
#endif


#define IOTA_TOPIC_BIND_RSP           "IOTA_TOPIC_BIND_RSP"
#define IOTA_TOPIC_CMD_UNBIND_RECEIVE "IOTA_TOPIC_CMD_UNBIND_RECEIVE"

typedef enum enum_EN_IOTA_BIND_RESULT_TYPE
{
    EN_IOTA_BIND_RESULT_SUCCESS            = 0,
    EN_IOTA_BIND_RESULT_DEV_NOT_REGISTER   = 1,
    EN_IOTA_BIND_RESULT_VERIFYCODE_EXPIRED = 2,
    EN_IOTA_BIND_RESULT_FAILED = 255
}EN_IOTA_BIND_RESULT_TYPE;

typedef enum enum_IOTA_BIND_IE_TYPE
{
    EN_IOTA_BIND_IE_RESULT       = 0,//EN_IOTA_REG_RESULT_TYPE
    EN_IOTA_BIND_IE_DEVICEID     = 1,//String 平台分配的逻辑设备ID
    EN_IOTA_BIND_IE_DEVICESECRET = 2,//String 设备接入的鉴权秘钥
    EN_IOTA_BIND_IE_APPID        = 3,//String 开发者应用ID
    EN_IOTA_BIND_IE_IOCM_ADDR    = 4,//IoCM服务器地址
    EN_IOTA_BIND_IE_IOCM_PORT    = 5,//IoCM服务器端口
    EN_IOTA_BIND_IE_MQTT_ADDR    = 6,//MQTT服务器地址
    EN_IOTA_BIND_IE_MQTT_PORT    = 7,//MQTT服务器端口
    EN_IOTA_BIND_IE_IODM_ADDR    = 8,//IoDM服务器地址
    EN_IOTA_BIND_IE_IODM_PORT    = 9,//IoDM服务器端口
}EN_IOTA_BIND_IE_TYPE;

HW_API_FUNC HW_INT IOTA_Bind(HW_CHAR *pcVerifyCode,ST_IOTA_DEVICE_INFO *pstInfo);

#ifdef __cplusplus
}
#endif

#endif

