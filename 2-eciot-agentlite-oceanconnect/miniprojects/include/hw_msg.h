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
 * @file hw_msg.h
 * Indicates HW messge Interface Functions
 */
#ifndef _HW_MSG_H_
#define _HW_MSG_H_

#include "hw_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stru_HW_MSG     { HW_INT unused; } * HW_MSG;  /**< @brief Caas message type. */
typedef struct stru_HW_BUNDLE { HW_INT unused; } * HW_BUNDLE; /**< @brief Caas bundle type. */

/**
 * <b>Description:</b> This method is used to create a new HW message with message type.
 * <br><b>Purpose:</b> After UI invokes this method to create a HW message, UI can add string values, boolean values,
 * int values and so on to the HW message.
 *
 * @param [in] uiMsg Specifies the type of the HW message
 * @return The value indicates the New HW message, and is HW_NULL when failing.
 */
HW_API_FUNC HW_MSG  HW_MsgAlloc(HW_UINT uiMsg);

/**
 * <b>Description:</b> This method is used to delete a HW message.
 * <br><b>Purpose:</b> After invoking this method, all memory allocated from it will also be freed. 
 * 
 * @param [in] pstMsg Specifies the ID of the HW message to be deleted
 */
HW_API_FUNC HW_VOID HW_MsgFree(HW_MSG pstMsg);

/**
 * <b>Description:</b> This method is used to send a HW message.
 * <br><b>Purpose:</b> After invoking this method, all memory allocated from it will also be freed. 
 * 
 * @param [in] pstMsg Specifies the ID of the HW message to be deleted
 */
HW_API_FUNC HW_INT  HW_MsgSend(HW_MSG pstMsg, HW_UINT uiDstPid, HW_UINT uiCookie, HW_UINT uiDstId);

/**
 * <b>Description:</b> This method is used to get the type of the HW message.
 * <br><b>Purpose:</b> After invoking this method, all memory allocated from it will also be freed. 
 * 
 * @param [in] pstMsg Specifies the ID of the HW message
 * @return The value indicates the type of the HW message
 */
HW_API_FUNC HW_UINT HW_MsgGetType(HW_MSG pstMsg);

/**
 * <b>Description:</b> This method is used to add a unsigned int value to a HW message.
 * <br><b>Purpose:</b> After UI creates a new HW message, UI can invoke this method to add
 * an unsigned int value to the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 * @param [in] uiValue Specifies the configure value
 *
 * @return HW_OK Indicates the adding succeeds
 *    <br> HW_FAILED Indicates the adding fails
 */
HW_API_FUNC HW_INT HW_MsgAddUint(HW_MSG pstMsg, HW_UINT uiTag, HW_UINT uiValue);

/**
 * <b>Description:</b> This method is used to add a string value to a HW message.
 * <br><b>Purpose:</b> After UI creates a new HW message, UI can invoke this method to
 * add a string value to the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 * @param [in] pcVal Specifies the configure value
 *
 * @return HW_OK Indicates the adding succeeds
 *    <br> HW_FAILED Indicates the adding fails
 */
HW_API_FUNC HW_INT HW_MsgAddStr(HW_MSG pstMsg, HW_UINT uiTag, HW_CHAR *pcVal);

/**
 * <b>Description:</b> This method is used to add a string value with lenth to a HW message.
 * <br><b>Purpose:</b> After UI creates a new HW message, UI can invoke this method to
 * add a string value with lenth to the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 * @param [in] pcVal Specifies the configure value
 * @param [in] uiLen Specifies the lenth of the configure value
 *
 * @return HW_OK Indicates the adding succeeds
 *    <br> HW_FAILED Indicates the adding fails
 */
HW_API_FUNC HW_INT HW_MsgAddStrN(HW_MSG pstMsg, HW_UINT uiTag, HW_CHAR *pcVal, HW_UINT uiLen);

/**
 * <b>Description:</b> This method is used to add an array with Byte value to a HW message.
 * <br><b>Purpose:</b> After UI creates a new HW message, UI can invoke this method to
 * add an array with Byte value to the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 * @param [in] pcByte Specifies the value of the array
 * @param [in] uiArrLen Specifies the lenth of the array 
 *
 * @return HW_OK Indicates the adding succeeds
 *    <br> HW_FAILED Indicates the adding fails
 */
HW_API_FUNC HW_INT HW_MsgAddByteArray(HW_MSG pstMsg, HW_UINT uiTag, HW_CHAR *pcByte, HW_UINT uiArrLen);
/**
 * <b>Description:</b> This method is used to get an unsigned int value from a HW message.
 * <br><b>Purpose:</b> After UI gets a new HW message, UI can invoke this method to get
 * an unsigned int value from the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 * @param [in] uiDefault Specifies the default value when getting fails
 *
 * @return Indicates the unsigned int value from the HW message
 */
HW_API_FUNC HW_UINT  HW_MsgGetUint(HW_MSG pstMsg, HW_UINT uiTag, HW_UINT uiDefault);

/**
 * <b>Description:</b> This method is used to get a string value from a HW message.
 * <br><b>Purpose:</b> After UI gets a new HW message, UI can invoke this method to get
 * a string value from the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 *
 * @return Indicates the string value from the HW message
 */
HW_API_FUNC HW_CHAR *HW_MsgGetStr(HW_MSG pstMsg, HW_UINT uiTag);

/**
 * <b>Description:</b> This method is used to get an array with Byte value from a HW message.
 * <br><b>Purpose:</b> After UI gets a new HW message, UI can invoke this method to
 * get an array with Byte value from the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 *
 * @return Indicates the array with Byte value from the HW message
 */
HW_API_FUNC HW_BYTES HW_MsgGetByteArray(HW_MSG pstMsg, HW_UINT uiTag);

/**
 * <b>Description:</b> This method is used to add a HW_BYTES value to a HW message.
 * <br><b>Purpose:</b> After UI creates a new HW message, UI can invoke this method to
 * add a HW_BYTES value to the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 * @param [in] pcVal Specifies the configure value
 *
 * @return HW_OK Indicates the adding succeeds
 *    <br> HW_FAILED Indicates the adding fails
 */
HW_API_FUNC HW_INT HW_MsgAddBstr(HW_MSG pstMsg, HW_UINT uiTag, HW_BYTES *pbsBstr);

/**
 * <b>Description:</b> This method is used to get a HW_BYTES value from a HW message.
 * <br><b>Purpose:</b> After UI gets a new HW message, UI can invoke this method to get
 * a HW_BYTES value from the message.
 *
 * @param [in] pstMsg Specifies the ID of the HW message
 * @param [in] uiTag Specifies the configure ID
 *
 * @return Indicates the string value from the HW message
 */
HW_API_FUNC HW_BYTES * HW_MsgGetBstr(HW_MSG pstMsg,HW_UINT uiTag);

#ifdef __cplusplus
}
#endif

#endif


