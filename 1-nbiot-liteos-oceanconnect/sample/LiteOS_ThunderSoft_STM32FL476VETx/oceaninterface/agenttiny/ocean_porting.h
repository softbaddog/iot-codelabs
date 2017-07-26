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
 
#ifndef _OCEAN_PORTING_H
#define _OCEAN_PORTING_H

#include "coap_liteos_port.h"

/* load verrify data from flash, now we just test , so read it in ram */
unsigned char * user_dev_load_data(unsigned int uiAddr, unsigned int uiLen);
/* save verify data to flashj, now we just test , so keep it in ram */
void user_dev_save_data(unsigned int uiAddr, unsigned char *pucData, unsigned int uiLen);

void user_dev_get_sn(unsigned char *pucData, unsigned char *uiLen);
void user_recv_cmd(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen);

#endif /* _OCEAN_PORTING_H */
