/**
 * @file hw_json.h
 * Indicates HW json Interface Functions
 */
#ifndef _HW_JSON_H_
#define _HW_JSON_H_
 
#include "hw_sys.h"
//#include "usp_sys.h"

#ifdef __cplusplus
 extern "C" {
#endif
 
 typedef struct stru_HW_JSONOBJ    { HW_INT unused; } * HW_JSONOBJ;
 typedef struct stru_HW_JSON       { HW_INT unused; } * HW_JSON;
 typedef struct stru_HW_JSONARRAY { HW_INT unused; } * HW_JSON_ARRAY;
 
 /**
  * <b>Description:</b> This method is used to create json decode. 
  * <br><b>Purpose:</b> UI invokes this method to create json decode in order to obtain json object.
  * such as: hJsonObj = HW_JsonDecodeCreate(pucStr, HW_FALSE);
  *
  * @param [in] pucStr Specifies the json string
  * @param [in] bStrCpy Specifies whether to support string copy 
  *
  * @return The value indicates the json object
  */
HW_API_FUNC HW_JSONOBJ HW_JsonDecodeCreate(HW_CHAR *pucStr, HW_BOOL bStrCpy);
 
 /**
  * <b>Description:</b> This method is used to create json object. 
  * <br><b>Purpose:</b> UI invokes this method to create json object.
  * After using this method UI needs to invoke HW_JsonObjDelete to delete
  *
  * @return The value indicates the json object
  */
 HW_API_FUNC HW_JSONOBJ HW_JsonObjCreate();
 
 /**
  * <b>Description:</b> This method is used to delete json object. 
  * <br><b>Purpose:</b> UI invokes this method to delete json object.
  *
  * @param [in] phJson Specifies the json object
  *
  * @return null
  */
 HW_API_FUNC HW_VOID HW_JsonObjDelete(HW_JSONOBJ* phJson);

 /**
  * <b>Description:</b> This method is used to get json from json object. 
  * <br><b>Purpose:</b> UI invokes this method to get json. such as: pstJson = HW_JsonGetJson(hJsonObj);
  *
  * @param [in] hJson Specifies the join object
  *
  * @return The value indicates the json
  */
 HW_API_FUNC HW_JSON HW_JsonGetJson(HW_JSONOBJ hJson);
 
 /**
  * <b>Description:</b> This method is used to get string value from json. 
  * <br><b>Purpose:</b> UI invokes this method to get string value from json.
  *
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_CHAR *HW_JsonGetStr(HW_JSON pstJson, HW_CHAR *pucKey);
 
 /**
  * <b>Description:</b> This method is used to get unsigned int value from json. 
  * <br><b>Purpose:</b> UI invokes this method to get unsigned int value from json.
  *
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  * @param [in] uiDft Specifies the default value
  *              which will be returned if the uiDft is not exist in the json.
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_UINT HW_JsonGetUint(HW_JSON pstJson, HW_CHAR *pucKey, HW_UINT uiDft);
 
 /**
  * <b>Description:</b> This method is used to get bool value from json. 
  * <br><b>Purpose:</b> UI invokes this method to get bool value from json.
  *
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  * @param [in] bDft Specifies the default value
  *              which will be returned if the bDft is not exist in the json.
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_BOOL HW_JsonGetBool(HW_JSON pstJson, HW_CHAR *pucKey, HW_BOOL bDft);
 
 /**
  * <b>Description:</b> This method is used to get array from json. 
  * <br><b>Purpose:</b> UI invokes this method to get array from json.
  *
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_JSON_ARRAY HW_JsonGetArray(HW_JSON pstJson, HW_CHAR *pucKey);
 
 /**
  * <b>Description:</b> This method is used to get json from json array. 
  * <br><b>Purpose:</b> UI invokes this method to get json from json array.
  *
  * @param [in] pstArray Specifies the json array
  * @param [in] uiIndex Specifies the index
  *
  * @return The json indicates the field value
  */
 HW_API_FUNC HW_JSON HW_JsonArrayGetJson(HW_JSON_ARRAY pstArray, HW_UINT uiIndex);
 
 /**
  * <b>Description:</b> This method is used to get unsigned int value from json array. 
  * <br><b>Purpose:</b> UI invokes this method to get unsigned int value from json array.
  *
  * @param [in] pstArray Specifies the json array
  * @param [in] uiIndex Specifies the index
  * @param [in] uiDft Specifies the default value
  *              which will be returned if the uiDft is not exist in the json array.
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_UINT HW_JsonArrayGetUint(HW_JSON_ARRAY pstArray, HW_UINT uiIndex, HW_UINT uiDft);
 
 /**
  * <b>Description:</b> This method is used to get bool value from json array. 
  * <br><b>Purpose:</b> UI invokes this method to get bool value from json array.
  *
  * @param [in] pstArray Specifies the json array
  * @param [in] uiIndex Specifies the index
  * @param [in] bDft Specifies the default value
  *              which will be returned if the bDft is not exist in the json.
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_BOOL HW_JsonArrayGetBool(HW_JSON_ARRAY pstArray, HW_UINT uiIndex, HW_BOOL bDft);
 
 /**
  * <b>Description:</b> This method is used to get string value from json array. 
  * <br><b>Purpose:</b> UI invokes this method to get string value from json array.
  *
  * @param [in] pstArray Specifies the json array
  * @param [in] uiIndex Specifies the index
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_CHAR *HW_JsonArrayGetStr(HW_JSON_ARRAY pstArray, HW_UINT uiIndex);
 
 /**
  * <b>Description:</b> This method is used to get array from json array. 
  * <br><b>Purpose:</b> UI invokes this method to get array from json array.
  *
  * @param [in] pstArray Specifies the json array
  * @param [in] uiIndex Specifies the index
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_JSON_ARRAY HW_JsonArrayGetArray(HW_JSON_ARRAY pstArray, HW_UINT uiIndex);
 
 /**
  * <b>Description:</b> This method is used to get array counts.
  * <br><b>Purpose:</b> UI invokes this method to query array counts.
  *
  * @param [in] pstArray Specifies the json array
  *
  * @return The value indicates the array counts
  */
 HW_API_FUNC HW_UINT HW_JsonArrayGetCount(HW_JSON_ARRAY pstArray);
 
 /**
  * <b>Description:</b> This method is used to add a Json value to json.
  * <br><b>Purpose:</b> After UI creates a json, UI can invoke this method to add
  * a Json value to the json.
  * 
  * @param [in] pstJson Specifies the json
  * @param [in] pcKey Specifies the key
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_JSON HW_JsonAddJson(HW_JSON pstJson, HW_CHAR *pcKey);
 
 /**
  * <b>Description:</b> This method is used to add a unsigned int value to json.
  * <br><b>Purpose:</b> After UI creates a json, UI can invoke this method to add
  * a unsigned int value to the json.
  * 
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  * @param [in] uiVal Specifies the field value
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_INT HW_JsonAddUint(HW_JSON pstJson, HW_CHAR *pucKey, HW_UINT uiVal);
 
 /**
  * <b>Description:</b> This method is used to add a bool value to json.
  * <br><b>Purpose:</b> After UI creates a json, UI can invoke this method to add
  * a bool value to the json.
  * 
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  * @param [in] bVal Specifies the field value
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */

 HW_API_FUNC HW_INT HW_JsonAddBool(HW_JSON pstJson, HW_CHAR *pucKey, HW_BOOL bVal);

 /**
  * <b>Description:</b> This method is used to add a string value to json.
  * <br><b>Purpose:</b> After UI creates a json, UI can invoke this method to add
  * a string value to the json.
  * 
  * @param [in] pstJson Specifies the json
  * @param [in] pucKey Specifies the key
  * @param [in] pcValue Specifies the field value
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_INT HW_JsonAddStr(HW_JSON pstJson, HW_CHAR *pucKey, HW_CHAR *pucVal);

 /**
 * <b>Description:</b> This method is used to add a array string to json.
 * <br><b>Purpose:</b> After UI creates a json, UI can invoke this method to add
 * a array string to the json.
 * 
 * @param [in] pstJson Specifies the json
 * @param [in] pucKey Specifies the key
 *
 * @return HW_UJSON_ARRAY Indicates the adding succeeds
 *    <br> HW_NULL Indicates the adding fails
 */
 HW_API_FUNC HW_JSON_ARRAY HW_JsonAddArray(HW_JSON pstJson, HW_CHAR *pucKey);

 /**
  * <b>Description:</b> This method is used to add a json array value to json array.
  * <br><b>Purpose:</b> UI can invoke this method to add a json value to the json array.
  * 
  * @param [in] pstArray Specifies the json array
  * @param [in] uiVal Specifies the field value
  *
  * @return CAAS_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_JSON HW_JsonArrayAddJson(HW_JSON_ARRAY pstArray);

 /**
  * <b>Description:</b> This method is used to add a unsigned int value to json array.
  * <br><b>Purpose:</b> UI can invoke this method to add a unsigned int value to the json array.
  * 
  * @param [in] pstArray Specifies the json array
  * @param [in] uiVal Specifies the field value
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_INT HW_JsonArrayAddUint(HW_JSON_ARRAY *pstArray, HW_UINT uiVal);
 
 /**
  * <b>Description:</b> This method is used to add a bool value to json array.
  * <br><b>Purpose:</b> UI can invoke this method to add a bool value to the json array.
  * 
  * @param [in] pstArray Specifies the json array
  * @param [in] bVal Specifies the field value
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_INT HW_JsonArrayAddBool(HW_JSON_ARRAY *pstArray, HW_BOOL bVal);

 /**
  * <b>Description:</b> This method is used to add a string value to json array.
  * <br><b>Purpose:</b> UI can invoke this method to add a string value to the json array.
  * 
  * @param [in] pstArray Specifies the json array
  * @param [in] pcValue Specifies the field value
  *
  * @return HW_OK Indicates the adding succeeds
  *    <br> Otherwise Indicates the adding fails
  */
 HW_API_FUNC HW_INT HW_JsonArrayAddStr(HW_JSON_ARRAY *pstArray, HW_UCHAR*pucVal);

 /**
  * <b>Description:</b> This method is used to add array string to json. 
  * <br><b>Purpose:</b> UI invokes this method to add array string to json.
  *
  * @param [in] pstArray Specifies the json array
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_JSON_ARRAY *HW_JsonArrayAddArray(HW_JSON_ARRAY *pstArray);

 /**
  * <b>Description:</b> This method is used to get string value from json. 
  * <br><b>Purpose:</b> UI invokes this method to get string value from json;
  *
  * @param [in] phJson Specifies the json object
  *
  * @return The value indicates the field value
  */
 HW_API_FUNC HW_CHAR *HW_JsonEncodeStr(HW_JSONOBJ hJson);
 
#ifdef __cplusplus
 }
#endif
 
#endif

