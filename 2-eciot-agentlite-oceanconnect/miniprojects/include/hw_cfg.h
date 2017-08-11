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
* @file hw_cfg.h
* Indicates configure Interface Functions
*/
#ifndef _HW_CFG_H_
#define _HW_CFG_H_

#include "hw_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * <b>Description:</b> This method is used to set the policy of the configuration item.
 * <br><b>Purpose:</b> UI invokes this method to set the policy of the configuration item.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] uiPolicy Specifies the policy
 *
 * @return CAAS_OK Indicates the setting succeeds
 *    <br> CAAS_FAILED Indicates the setting fails
 */
HW_API_FUNC HW_INT HW_CfgSetPolicy(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_UINT uiPolicy);

/**
 * <b>Description:</b> This method is used to set a unsigned int value to a configuration item.
 * <br><b>Purpose:</b> UI can invoke this method to set a unsigned int value to a configuration item.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] uiVal Specifies the configure value
 *
 * @return CAAS_OK Indicates the setting succeeds
 *    <br> CAAS_FAILED Indicates the setting fails
 */
HW_API_FUNC HW_INT HW_CfgSetUint(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_UINT uiVal);

/**
 * <b>Description:</b> This method is used to set a string value to a configuration item.
 * <br><b>Purpose:</b> UI can invoke this method to set a string value to a configuration item.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] pucVal Specifies the configure value
 *
 * @return CAAS_OK Indicates the setting succeeds
 *    <br> CAAS_FAILED Indicates the setting fails
 */
HW_API_FUNC HW_INT HW_CfgSetStr(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_CHAR *pucVal);

/**
 * <b>Description:</b> This method is used to get an unsigned int value of a configuration item.
 * <br><b>Purpose:</b> UI invokes this method to get an unsigned int value of a configuration item.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 *
 * @return The value indicates the current configure value
 */
HW_API_FUNC HW_UINT HW_CfgGetUint(HW_UINT uiCfgId, HW_UINT uiCfgName);

/**
 * <b>Description:</b> This method is used to get a string value of a configuration item.
 * <br><b>Purpose:</b> UI invokes this method to get a string value of a configuration item.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 *
 * @return The value indicates the current configure value
 */
HW_API_FUNC HW_CHAR *HW_CfgGetStr(HW_UINT uiCfgId, HW_UINT uiCfgName);

/**
 * <b>Description:</b> This method is used to set a unsigned int value to a configuration item.
 * <br><b>Purpose:</b> UI can invoke this method to set a unsigned int value to a configuration item with the index.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] uiIdx Specifies the index of configure item
 * @param [in] uiVal Specifies the configure value
 *
 * @return CAAS_OK Indicates the setting succeeds
 *    <br> CAAS_FAILED Indicates the setting fails
 */
HW_API_FUNC HW_INT HW_CfgArraySetUint(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_UINT uiIdx, HW_UINT uiVal);

/**
 * <b>Description:</b> This method is used to set a string value to a configuration item.
 * <br><b>Purpose:</b> UI can invoke this method to set a string value to a configuration item with the index. 
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] uiIdx Specifies the index of configure item
 * @param [in] pucVal Specifies the configure value
 *
 * @return CAAS_OK Indicates the setting succeeds
 *    <br> CAAS_FAILED Indicates the setting fails
 */
HW_API_FUNC HW_INT HW_CfgArraySetStr(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_UINT uiIdx, HW_CHAR *pucVal);

/**
 * <b>Description:</b> This method is used to get an unsigned int value of a configuration item.
 * <br><b>Purpose:</b> UI invokes this method to get an unsigned int value of a configuration item with the index.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] uiIdx Specifies the index of configure item
 *
 * @return The value indicates the current configure value
 */
HW_API_FUNC HW_UINT HW_CfgArrayGetUint(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_UINT uiIdx);

/**
 * <b>Description:</b> This method is used to get a string value of a configuration item.
 * <br><b>Purpose:</b> UI invokes this method to get a string value of a configuration item with the index.
 *
 * @param [in] uiCfgId Specifies the configure ID
 * @param [in] uiCfgName Specifies the configure name
 * @param [in] uiIdx Specifies the index of configure item
 *
 * @return The value indicates the current configure value
 */
HW_API_FUNC HW_CHAR *HW_CfgArrayGetStr(HW_UINT uiCfgId, HW_UINT uiCfgName, HW_UINT uiIdx);
    
#ifdef __cplusplus
}
#endif

#endif

