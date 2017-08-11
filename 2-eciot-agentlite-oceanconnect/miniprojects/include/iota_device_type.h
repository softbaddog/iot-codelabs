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

#ifndef _IOTA_DEVICE_TYPE_H_
#define _IOTA_DEVICE_TYPE_H_
#ifdef __cplusplus
extern "C" {
#endif


/**
 * <b>Description:</b> It is used to notify caller login successed. 
 * <br><b>Purpose:</b> After the caller login, When the UI receives this notification, it means the caller 
 * login successed, and the UI must be updated. The UI can not receive this notification if caller login 
 * failed. The UI is recommended to perform interception on this notification during the entire running 
 * of the application.
 */
typedef struct stru_ST_IOTA_DEVICE_INFO
{
    HW_CHAR *pcNodeId;
    HW_CHAR *pcName;
    HW_CHAR *pcDescription;
    HW_CHAR *pcManufacturerId;
    HW_CHAR *pcManufacturerName;
    HW_CHAR *pcMac;
    HW_CHAR *pcLocation;
    HW_CHAR *pcDeviceType;
    HW_CHAR *pcModel;
    HW_CHAR *pcSwVersion;
    HW_CHAR *pcFwVersion;
    HW_CHAR *pcHwVersion;
    HW_CHAR *pcProtocolType;
    HW_CHAR *pcBridgeId;
    HW_CHAR *pcStatus;
    HW_CHAR *pcStatusDetail;
    HW_CHAR *pcMute;
}ST_IOTA_DEVICE_INFO;

/**
 * <b>Description:</b> This method is used to login to the u-portal and network.
 * <br><b>Purpose:</b> After call this method, SDK will send request to login the u-portal and network.
 * If login success, a @ref IOTA_CALLER_LOGIN_CONNECTED will be sent to UI. If login fail, a @ref IOTA_CALLER_LOGIN_DISCONNECTED 
 * will be sent to UI.
 *
 * @param [in] pcPhoneNumber Specifies the caller phone number to login
 * @param [in] pcPhonePwd Specifies the caller phone password to login
 * @param [in] uExtParas Specifies the extend parameters, see @ref EN_IOTA_LOGIN_IE_TYPE, which may include as follows, 
 * If the UI doesn't need to set this extended parameter, it can enter the @ref IOTA_NULL here.
 * <ul>
 * <li><em>{@link #EN_IOTA_LOGIN_EXT_COUNTRY_CODE}</em></li>
 * <li><em>{@link #EN_IOTA_LOGIN_EXT_REMEMBER_PWD_FLAG}</em></li>
 * <li><em>{@link #EN_IOTA_LOGIN_EXT_AUTO_LOGIN_FLAG}</em></li>
 * </ul>      
 * @return IOTA_OK Indicates invoking the interface succeed
 *   <br>  IOTA_FAILED Indicates invoking the interface failed
 */

#ifdef __cplusplus
}
#endif

#endif

