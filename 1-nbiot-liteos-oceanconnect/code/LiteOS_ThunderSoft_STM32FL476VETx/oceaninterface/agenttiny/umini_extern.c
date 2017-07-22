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

#include "umini.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#if _MSC_VER > 1200
//#include <iphlpapi.h>
#endif
//#include <windows.h>
//#include <fcntl.h>
//#include <direct.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <io.h>

UM_VOID UMini_MemCpy(UM_VOID *pDst, UM_SIZE nDstMax, UM_VOID *pSrc, UM_SIZE nSize)
{
    memcpy(pDst, pSrc, nSize);
}

UM_VOID UMini_MemSet(UM_VOID *pDst, UM_SIZE nDstMax, UM_UINT8 ucVal, UM_SIZE nSize)
{
    memset(pDst, ucVal, nSize);
}
#if 0
UM_SOCK UMini_SockCreate()
{   
   // return (UM_SOCK)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UM_SIZE UMini_SockSendTo(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen, UM_UINT8 *pucIp, UM_UINT16 usPort)
{
   /* struct sockaddr_in stV4Addr = {0};
    struct sockaddr *pstSockAddr;
    int iSize, iRet;

    iSize = sizeof(struct sockaddr_in);
    stV4Addr.sin_family = AF_INET;
    stV4Addr.sin_port = ntohs(usPort);
    stV4Addr.sin_addr.s_addr = inet_addr(pucIp);
    pstSockAddr = (struct sockaddr *)&stV4Addr;
           
    iRet = sendto((int)hSock, pucVal, nLen, 0, pstSockAddr, iSize); 
    if (iRet != SOCKET_ERROR)
    {
        return iRet;
    }

    return 0;*/
}

UM_SIZE UMini_SockRecvFrom(UM_SOCK hSock, UM_UINT8 *pucBuf, UM_SIZE nLen)
{
   /* struct sockaddr_in  stV4Addr = {0};
    struct timeval stTimeout = {0};
    struct sockaddr *pstSockAddr;
    fd_set readFds = {0};
    int iSize, iRet;

    stTimeout.tv_sec = 2;
    FD_SET((int)hSock, &readFds);

    iRet = select((int)hSock, &readFds, 0, 0, &stTimeout);
    if (iRet == SOCKET_ERROR || iRet == 0) 
    {
        return 0;
    }

    if (!FD_ISSET((int)hSock, &readFds))
    {
        return 0;
    }

    iSize = sizeof(struct sockaddr_in);
    pstSockAddr = (struct sockaddr *)&stV4Addr;

    iRet = recvfrom((int)hSock, pucBuf, nLen, 0, pstSockAddr, &iSize);
    if (iRet == SOCKET_ERROR)
    {
        return 0;
    }

    return iRet;*/
}

UM_VOID UMini_SockClose(UM_SOCK hSock)
{
    /*closesocket((int)hSock);*/
}

UM_VOID UMini_Printf(UM_CONST UM_CHAR *pcFormat, ...)
{
    printf("%s", pcFormat);
}
#endif
