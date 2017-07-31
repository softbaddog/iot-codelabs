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
 
 #include "ocean_porting.h"
 #include "main.h"
 //#include "ocean_sample.h"
 #include "los_bsp_led.h" 
 #include "ocean_interface.h"
 
 
static unsigned char flashdata[256] = {0};

/* load verify data from flash, now we just test, so read it in ram */
unsigned char * user_dev_load_data(unsigned int uiAddr, unsigned int uiLen)
{
	return (unsigned char *)flashdata;
}

/* save verify data to flash, now we just test, so keep it in ram */
void user_dev_save_data(unsigned int uiAddr, unsigned char *pucData, unsigned int uiLen)
{
	int tmp = 0;
	if(0 == uiAddr || NULL == pucData || 0 == uiLen)
	{
		return ;
	}
	tmp = sizeof(flashdata);
	if (uiLen > tmp)
	{
		uiLen = tmp;
	}
	memcpy(flashdata, pucData, uiLen);
	return ;
}
extern unsigned char g_devsn[16];
void user_dev_get_sn(unsigned char *pucData, unsigned char * uiLen)
{
	if(NULL == pucData || NULL == uiLen)
	{
		return ;
	}
    *uiLen = strlen((const char *)g_devsn);
    memcpy(pucData, g_devsn, *uiLen);
	return ;
}

void user_recv_cmd(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen)
{
	//here we should get the cmd and control the led_status
	if (NULL == pucVal || 0 == nLen)
	{
		return ;
	}
    ocean_dev_control(pucVal, nLen);
	
	return ;
}
