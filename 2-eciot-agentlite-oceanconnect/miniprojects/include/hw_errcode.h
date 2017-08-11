/**
 * @file hw_errcode.h
 */
#ifndef _HW_ERRCODE_H_
#define _HW_ERRCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define HW_OK               0   /**< Indicates ok */                                           
#define HW_ERR              1   /**< Indicates err */                                                
#define HW_ERR_PTR          2   /**< Indicates error ptr */                                       
#define HW_ERR_ID           3   /**< Indicates error id */                                  
#define HW_ERR_PARA         4   /**< Indicates error para */
#define HW_ERR_KEY          5   /**< Indicates error key */  
#define HW_ERR_NOMEM        6   /**< Indicates no mem*/                                             
#define HW_ERR_MAGIC        7   /**< Indicates error magic*/                                                   
#define HW_ERR_OVERFLOW     8   /**< Indicates overflow */                                  
#define HW_ERR_GVAR         9   /**< Indicates error gvar*/                                       
#define HW_ERR_POOL         10  /**< Indicates error pool*/                                       
#define HW_ERR_NO_MUTEX     11  /**< Indicates no mutex */                                         
#define HW_ERR_PID          12  /**< Indicates error pid*/                                         
#define HW_ERR_FILEOPEN     13  /**< Indicates open file failed */                                   
#define HW_ERR_FD           14  /**< Indicates file descriptor err*/                                 
#define HW_ERR_SOCKET       15  /**< Indicates socket err*/                                        
#define HW_ERR_NOTSUPPORT   16  /**< Indicates not support*/                                       
#define HW_ERR_NOTLOAD      17  /**< Indicates not load*/                                            
#define HW_ERR_ENCODE       18  /**< Indicates encode err*/                                          
#define HW_ERR_DECODE       19  /**< Indicates decode err*/                                           
#define HW_ERR_CALLBACK     22  /**< Indicates callback err*/                                  
#define HW_ERR_STATE        23  /**< Indicates state err*/                                            
#define HW_ERR_OVERTIMES    24  /**< Indicates retry overtimes*/                                   
#define HW_ERR_ENDOVER      25  /**< reserved*/                                             
#define HW_ERR_ENDLINE      26  /**< reserved*/                                            
#define HW_ERR_NUMBER       27  /**< Indicates error number*/                                           
#define HW_ERR_NOMATCH      28  /**< Indicates not match*/                                         
#define HW_ERR_NOSTART      29  /**< Indicates not start*/                                               
#define HW_ERR_NOEND        30  /**< Indicates not end*/                                                 
#define HW_ERR_OVERLAP      31  /**< reserved*/                                                    
#define HW_ERR_DROP         32  /**< Indicates drop*/                                                 
#define HW_ERR_NODATA       33  /**< Indicates nodata*/                                               
#define HW_ERR_CRC_CHK      34  /**< Indicates crc check failed*/                                       
#define HW_ERR_AUTH         35  /**< Indicates authentication failed*/                                              
#define HW_ERR_LENGTH       36  /**< Indicates invalid length*/                                       
#define HW_ERR_NOTALLOW     37  /**< Indicates Not allowed to operate*/                                    
#define HW_ERR_TOKEN        38  /**< Indicates token err*/                                             
#define HW_ERR_NOTIPV4      39  /**< Indicates not support IPV4*/                                           
#define HW_ERR_NOTIPV6      40  /**< Indicates not support IPV6*/                                        
#define HW_ERR_IELOST       41  /**< reserved*/                                                 
#define HW_ERR_IELOST1      42  /**< reserved*/                                               
#define HW_ERR_IELOST2      43  /**< reserved*/                                             
#define HW_ERR_AUDIO        44  /**< reserved*/                                                   
#define HW_ERR_VIDEO        45  /**< reserved*/                                                   
#define HW_ERR_MD5          46  /**< reserved*/                                                 
#define HW_ERR_MD5_HA1      47  /**< reserved*/                                               
#define HW_ERR_MD5_RES      48  /**< reserved*/                                             
#define HW_ERR_DIALOG       49  /**< Indicates error dialog*/                                         
#define HW_ERR_OBJ          50  /**< Indicates error object*/           
#ifdef __cplusplus
}
#endif

#endif
