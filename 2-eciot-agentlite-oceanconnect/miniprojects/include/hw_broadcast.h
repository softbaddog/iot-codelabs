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
 * @file hw_broadcast.h
 * Indicates HW broadcast Interface Functions
 */

#ifndef _HW_BROADCAST_H_
#define _HW_BROADCAST_H_
#include "hw_sys.h"
#include "hw_msg.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Indicates type define of notification handle function. */
typedef HW_INT (*PFN_HW_BROADCAST_RECV)(HW_UINT uiCookie, HW_MSG pstMsg);
/**
 * <b>Description:</b> This method is used to initiate HW BroadCast.
 * <br><b>Purpose:</b> Gateway must init HW BroadCast first before register HW BroadCast.
 * @return HW_OK Indicates that function registration succeeds
 *    <br> HW_FAILED Indicates that function registration fails
 */
HW_API_FUNC HW_INT HW_BroadCastInit();
/**
 * <b>Description:</b>  This method is used to initiate HW BroadCast.
 * <br><b>Purpose:</b> Gateway can destroy the HW BroadCast when it needs to stop the whole BroadCast.
 * @return HW_OK Indicates that function registration succeeds
 *    <br> HW_FAILED Indicates that function registration fails
 */

HW_API_FUNC HW_INT HW_BroadCastDestroy();
/**
 * <b>Description:</b> This method is used to register the handle function of a notification,
 * which will be invoked as the notification with the name of pcTopic is sent by some componets.
 * <br><b>Purpose:</b> During the initialization, UI invoke this interface to set the handle function of a notification,
 * when receiving the notification, it will perform the function.
 * @param [in] pfnReceiver Specifies the notification handle function, see @ref PFN_HW_BROADCAST_RECV
 * @param [in] pcTopic Specifies the notification name
 *
 * @return HW_OK Indicates that function registration succeeds
 *    <br> HW_FAILED Indicates that function registration fails
 */
HW_API_FUNC HW_INT HW_BroadCastReg(HW_CHAR *pcTopic, PFN_HW_BROADCAST_RECV pfnReceiver);

/**
 * <b>Description:</b> This method is used to unregister the handle function of a notification.
 * which function pointer is pfnReceiver and name is pcTopic.
 * <br><b>Purpose:</b> When UI wants to change the handle function of a notification, it can invoke
 * this method to unregister the current handle function.
 * @param [in] pfnReceiver Specifies the notification handle function, see @ref PFN_HW_BROADCAST_RECV
 * @param [in] pcTopic Specifies the notification name
 */
HW_API_FUNC HW_INT HW_BroadCastUnreg(HW_CHAR *pcTopic, PFN_HW_BROADCAST_RECV pfnReceiver);

/**
 * <b>Description:</b> This method is used to send notification with given name.
 * <br><b>Purpose:</b> After UI defines a notification name, and adds the information to the notification,
 * then UI invokes this method to send the notification.
 * 
 * @param [in] pcAction Specifies the notification name
 * @param [in] uiCookie Specifies the notification ID
 * @param [in] pstMsg Specifies the  HW message to send
 *
 * @return HW_OK Indicates that sending succeeds
 *    <br> HW_FAILED Indicates that sending fails
 */
HW_API_FUNC HW_INT HW_BroadCastSend(HW_CHAR *pcAction, HW_UINT uiCookie, HW_MSG pstMsg);

#ifdef __cplusplus
}
#endif

#endif

