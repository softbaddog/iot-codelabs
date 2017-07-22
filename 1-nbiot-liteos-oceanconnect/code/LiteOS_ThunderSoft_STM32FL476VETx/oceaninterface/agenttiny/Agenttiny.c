#include "umini.h"
#include "umini_prv.h"

#include "stdlib.h"
#include "string.h"
#include "coap.h"
#define    nodata   0x0
//【1】==================================AgentTiny接口与数据结构==================================
//=======(1)=======接口简介：4个api、4个回调，另外对于设备无tcp/ip协议栈（如mcu直接使用GSM模块等情况）还可增加2个sockSend/sockRecv的回调，用于coap发送和接收
PFN_ONRECV    g_pfnUminiOnRecv = UM_NULL;


//【2】==================================Tiny总体业务逻辑的伪码==================================
//==============AgentTiny.c==============
board_callback_t g_board_func = {0};
coap_callback_t  g_coap_func={0};
unsigned char *g_flash = 0;
uint8_t g_Token[TOKLENMAX] = {0xf1};
uint8_t g_Token_len = 8;
ST_UMINI_MGR  g_stUminiMgr = {0};
unsigned char g_DeviceSN[DEVICESN] = {'\0'};
uint8_t   g_DeviceSNLen=0;
uint8_t   UM_outbuff[256] = {0};
uint8_t   g_outbuf[256] = {0};
uint8_t   lifetime=0;
uint16_t  msgid = 0x0101;

static unsigned char   UM_RES[3]={'t'};
static unsigned char   UM_RES1[3]={'r'};
static unsigned char   UM_RES2[16]="ep=";

UM_API_FUNC  UM_RET UMini_Initial(board_callback_t boardfunc,coap_callback_t coapfunc)
{
	g_board_func.pfnGetSN = boardfunc.pfnGetSN;
	g_board_func.pfnDataWrite = boardfunc.pfnDataWrite;
	g_board_func.pfnDataRead = boardfunc.pfnDataRead;
	g_board_func.pfnRecvCmd = boardfunc.pfnRecvCmd;
	g_board_func.pfnSendData=boardfunc.pfnSendData;
	g_board_func.flashAddr = boardfunc.flashAddr;
  g_board_func.uiLength=boardfunc.uiLength;  
  g_board_func.iSocket=boardfunc.iSocket;	
  g_flash = g_board_func.pfnDataRead(g_board_func.flashAddr,g_board_func.uiLength);
	
	
	g_coap_func.pfnCoapPktMalloc=coapfunc.pfnCoapPktMalloc;
	g_coap_func.pfnCoapPktfree=coapfunc.pfnCoapPktfree;
	g_coap_func.pfnGetRandom=coapfunc.pfnGetRandom;
	g_coap_func.pfnGetToken=coapfunc.pfnGetToken;
	g_coap_func.pfnSetCoapHeader=coapfunc.pfnSetCoapHeader;
	g_coap_func.pfnSetCoapOption=coapfunc.pfnSetCoapOption;
	g_coap_func.pfnSetCoapPayload=coapfunc.pfnSetCoapPayload;
	g_coap_func.pfnSetCoapToken=coapfunc.pfnSetCoapToken;
	g_coap_func.pfnCoapBuild=coapfunc.pfnCoapBuild;
    if(NULL!= g_flash[0])
    {    //u8BindFlag 全局变量变为flash保存  因为NB设备一次绑定 断电重启之后可以直接报数据  ?        if(/*解析g_flash，若是IotIp port DeviceSN u8BindFlag*/)
		  if(nodata!=g_flash[0]) 
			{
			 //这个是要调用函数得到的
			 g_board_func.pfnGetSN (g_DeviceSN,&g_DeviceSNLen);
			 g_stUminiMgr.u8BindFlag = 1;
			 return UM_OK;
			}
			else
      {
        g_stUminiMgr.u8BindFlag = 0;
				return UM_OK;
      }
			//todo  这种情况考虑怎么判断
			/*else if{
			//清空垃圾数据
			g_board_func.pfnDataWrite(g_board_func.flashAddr, 0, 512);
			g_stUminiMgr.u8BindFlag = 0;
			}*/
		}
		else if(NULL== g_flash[0])
		{
		  return UM_ERR;
		}
		return UM_ERR;
}

/***
****1.初始化时从flash中读取平台ip port、设备sn、bindflag
****2.把ip port  SN 拼装成ip+port+ POST+CON +/t/r?ep=id 的CoAP报文包
****3.把CoAP 报文包转化成能发送的字符串pucBuf
****4.调用注册的发送回调函数把数据发出去
***/
UM_UINT8 umini_bind()
{
    //利用 g_flash解析出需要的IP ，port，sn，组装bind报文
	  //组装绑定报文             报文组装的时候需要写特定的协商字符  /t/r

	 // unsigned char UM_buf[100];
   /* size_t option_len,option1_len,option2_len;
    uint16_t delta = 0;			 			 
    coap_packet_t *pkt; 
    coap_buffer_t tok; */	
		unsigned int rantok;
	

		unsigned char  devicesnLen;
    //todo
	  size_t buflen; 
		UM_COAPPKT pkt;
  	pkt = g_coap_func.pfnCoapPktMalloc();
		
	//  pkt = (coap_packet_t *)malloc(sizeof(coap_packet_t));

  //  memset(pkt,0,sizeof(coap_packet_t));
  /*	pkt->hdr.ver = 0x01;
    pkt->hdr.t = COAP_TYPE_CON;//COAP_TYPE_ACK;
    pkt->hdr.tkl = 0;
    pkt->hdr.code = COAP_METHOD_POST;//COAP_METHOD_PUT;
    pkt->hdr.id[0] = (msgid>>8)&0x00ff;
    pkt->hdr.id[1] = msgid&0x00ff;
		msgid++;
	  pkt->numopts = 3;*/
		//g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_CON,0,COAP_METHOD_POST);
    g_board_func.pfnGetSN(UM_RES2+3,&devicesnLen);
		rantok = g_coap_func.pfnGetRandom();
		g_Token[1] = (uint8_t)(rantok>>24);
		g_Token[2] = (uint8_t)(rantok>>16);
		g_Token[3] = (uint8_t)(rantok>>8);
		g_Token[4] = (uint8_t)(rantok);
		g_coap_func.pfnSetCoapToken(pkt, g_Token, g_Token_len);
		g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_CON, g_Token_len,COAP_METHOD_POST);
		
    // need token in response
    //if (UM_NULL==tok) {
		/*	rantok = coap_get_random();
			g_Token[1] = (uint8_t)(rantok>>24);
			g_Token[2] = (uint8_t)(rantok>>16);
			g_Token[3] = (uint8_t)(rantok>>8);
			g_Token[4] = (uint8_t)(rantok);
			tok.p = g_Token;
			tok.len = g_Token_len;
      pkt->hdr.tkl = tok.len;
      memcpy(&pkt->tok, &tok, sizeof(coap_buffer_t));
    //}*/
		
	    //option 转化为pkt->opt[]	
	    //option 转化为UM_outbuff
			
		/*  option_len = coap_opt_encode((coap_opt_t *)UM_outbuff, sizeof(UM_outbuff),COAP_OPTION_URI_PATH, (unsigned char *)res, 1);
			p = UM_outbuff;
		 //UM_outbuff 转化为pkt->opt[]
		  coap_parseOption(&pkt->opts[0], &delta, &p,option_len);
				//option	
		  option1_len = coap_opt_encode((coap_opt_t *)UM_outbuff+option_len, sizeof(UM_outbuff),COAP_OPTION_URI_PATH-delta,(unsigned char *)res1, 1);
			p = UM_outbuff+option_len;
		  coap_parseOption(&pkt->opts[1], &delta, &p,option1_len);
			
			option2_len = coap_opt_encode((coap_opt_t *)UM_outbuff+option_len+option1_len, sizeof(UM_outbuff),COAP_OPTION_URI_QUERY-delta,(unsigned char *)res2,15);
			p = UM_outbuff+option_len+option1_len;
		  coap_parseOption(&pkt->opts[2], &delta, &p ,option2_len);*/
			
	    buflen = (size_t)sizeof(g_outbuf); 
			//typedef UM_VOID (*PFN_SET_COAP_OPTION)(UM_COAPPKT pkt,UM_UINT8 optsIndex,UM_UINT8 *delta,UM_UINT8 *p,UM_UINT8 plen);//Tiny接收到控制命令后传给设备应用的回调
			g_coap_func.pfnSetCoapOption(pkt,0,COAP_OPTION_URI_PATH, (unsigned char *)UM_RES,1);
			g_coap_func.pfnSetCoapOption(pkt,1,COAP_OPTION_URI_PATH, (unsigned char *)UM_RES1,1);
			g_coap_func.pfnSetCoapOption(pkt,2,COAP_OPTION_URI_QUERY, (unsigned char *)UM_RES2,devicesnLen+3); 
			//coap pkt转换为可发送的字符串
      //coap_build(g_outbuf, &buflen, pkt);
		  g_coap_func.pfnCoapBuild(g_outbuf, &buflen, pkt); 
			g_coap_func.pfnCoapPktfree(pkt);
			//free(pkt);
	    //发送bind报文    g_IotIp,g_IotPort,g_DeviceSN;
	    g_board_func.pfnSendData(g_board_func.iSocket,g_outbuf,buflen);
	    return UM_OK;
}
/***
****1.初始化时从flash中读取平台ip port
****2.把ip port  数据data  拼装成ip+port+205content+data  的CoAP报文包
****3.把CoAP 报文包转化成能发送的字符串pucBuf
****4.调用注册的发送回调函数把数据发出去
***/
#if 0
UM_UINT8 DataReport(const coap_buffer_t* tok,unsigned char *data, unsigned int dlen)
{
   
    //todo
	  //uint8_t    UM_outbuff[256];		 
    coap_packet_t *pkt;
	  size_t option_len;
	  const uint8_t *p;
	  uint16_t delta = 0;		
    //todo
	  size_t buflen ;
  	pkt = (coap_packet_t *)malloc(sizeof(coap_packet_t));
	  //inject  header
  	memset(pkt,0,sizeof(coap_packet_t));
  	pkt->hdr.ver = 0x01;
    pkt->hdr.t = COAP_TYPE_NONCON;
    pkt->hdr.tkl = 0;
    pkt->hdr.code = COAP_RSPCODE_CONTENT;
    //pkt->hdr.id[0] = 0x01;
    //pkt->hdr.id[1] = 0x01;
    pkt->hdr.id[0] = (msgid>>8)&0x00ff;
    pkt->hdr.id[1] = msgid&0x00ff;
		msgid++;
	  pkt->numopts = 1;
	
    //inject token
    if (tok) {
         
   			pkt->hdr.tkl = tok->len;
        memcpy(&pkt->tok, tok, sizeof(coap_buffer_t));
    }
	  buflen = (size_t)sizeof(UM_outbuff);
		//option	
	    //option->UM_outbuff
		  option_len = coap_opt_encode((coap_opt_t *)UM_outbuff, sizeof(UM_outbuff),COAP_OPTION_OBSERVE, &lifetime, 1);
			p = UM_outbuff;
		  lifetime++;
		  coap_parseOption(&pkt->opts[0], &delta, &p,option_len);
		//inject payload
    if(data){
		pkt->payload.len=dlen;//strlen((const char *)data);
		pkt->payload.p =data;
		}
		//translate pkt to Outbuf
    coap_build(g_outbuf,&buflen,pkt);
		free(pkt);
	  g_board_func.pfnSendData(g_board_func.iSocket, g_outbuf,buflen);
		return UM_OK;
}
#endif
UM_UINT8 dataReport(const coap_buffer_t* tok,unsigned char *data, unsigned int dlen)
{
   
    //todo
	  //uint8_t    UM_outbuff[256];		 
	  /*size_t option_len;
	  const uint8_t *p;
	  uint16_t delta = 0;		*/
    //todo
	 // size_t buflen ;
	size_t buflen;	
	UM_COAPPKT pkt;
	pkt = g_coap_func.pfnCoapPktMalloc();
	  //inject  header
  	//memset(pkt,0,sizeof(coap_packet_t));
  	/*pkt->hdr.ver = 0x01;
    pkt->hdr.t = COAP_TYPE_NONCON;
    pkt->hdr.tkl = 0;
    pkt->hdr.code = COAP_RSPCODE_CONTENT;*/
	  
    //pkt->hdr.id[0] = 0x01;
    //pkt->hdr.id[1] = 0x01;
   /* pkt->hdr.id[0] = (msgid>>8)&0x00ff;
    pkt->hdr.id[1] = msgid&0x00ff;
		msgid++;*/
	  //pkt->numopts = 1;
	
    //inject token
   /* if (tok) {
         
   			pkt->hdr.tkl = tok->len;
        memcpy(&pkt->tok, tok, sizeof(coap_buffer_t));
    }*/
		
		g_coap_func.pfnSetCoapToken(pkt,g_Token,g_Token_len);
		g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_NONCON,g_Token_len,COAP_RSPCODE_CONTENT);
		lifetime++;
		g_coap_func.pfnSetCoapOption(pkt, 0x00, COAP_OPTION_OBSERVE, &lifetime, 1);
	 /* buflen = (size_t)sizeof(UM_outbuff);
		//option	
	    //option->UM_outbuff
		  option_len = coap_opt_encode((coap_opt_t *)UM_outbuff, sizeof(UM_outbuff),COAP_OPTION_OBSERVE, &lifetime, 1);
			p = UM_outbuff;
		  lifetime++;
		  coap_parseOption(&pkt->opts[0], &delta, &p,option_len);
		//inject payload
    if(data){
		pkt->payload.len=dlen;//strlen((const char *)data);
		pkt->payload.p =data;
		}
		//translate pkt to Outbuf
    coap_build(g_outbuf,&buflen,pkt);
		free(pkt);*/
		buflen = (size_t)sizeof(g_outbuf);
		g_coap_func.pfnSetCoapPayload(pkt,data,dlen);
	  g_coap_func.pfnCoapBuild(g_outbuf, &buflen, pkt);
		g_coap_func.pfnCoapPktfree(pkt);
	  g_board_func.pfnSendData(g_board_func.iSocket, g_outbuf,buflen);
		
		return UM_OK;
}

/*
*正常情况下会发送4中情况的报文:
主动发送:
1.绑定时的报文:CON post ip:port+/t/r?ep=SN
2.上报订阅数据数据:     ip:port+2.05content+data
*/
//改为case或者ifelse 分别处理然后去掉UMini_Drive
UM_API_FUNC UM_RET UMini_Report(unsigned char *pucData, unsigned int uiLen)
//unsigned int UMini_Report(unsigned char *pucData, unsigned int uiLen)
{
	//（1）把pucData存入g_stUminiMgr.stReportBuf中
	//（2）
	 coap_buffer_t  tok;
	 tok.p = (const uint8_t*)g_Token;
	 tok.len = g_Token_len;
	if(0 == g_stUminiMgr.u8BindFlag)
	{
		umini_bind();//如果 g_flash 内容为空，即无AppID、DeviceID、DeviceSecret等，说明还没绑定。
	}
	else//设备已经绑定平台，可直接上报数据
	{
		dataReport((const coap_buffer_t *)&tok, pucData, uiLen);//for循环，每次都把g_stUminiMgr.stReportBuf中缓存的设备数据，通过网络发送给云网关
	}
	return 0; 
}

//无os或单任务os，就在网络接收数据中断后调用NetRecv
//有os且多任务的，就网络接收线程调用NetRecv
/*
*正常情况会收到三种情况的报文:
1.绑定成功后平台回应 ACK 204 Changed/201Created?
2.绑定成功后平台订阅GET /t/d  observe=0
3.命令下发时的 POST /t/d  data
*/
//UM_API_FUNC UM_RET  NetRecv(coap_packet_t *pucData, unsigned int uiLen)
//三种情况都是Coap  endpoint 处理，NetRecv只是对接收到的报文pkt做正确性验证。
UM_API_FUNC UM_RET UMini_NetRecv(uint8_t t, uint8_t code,uint8_t  delta, const uint8_t *pToken,uint8_t pTokenLen)
{
    if(COAP_TYPE_CON == t && COAP_METHOD_GET==code && COAP_OPTION_OBSERVE==delta)//bind success
    {
		  memcpy(g_Token,pToken,pTokenLen);
			g_stUminiMgr.u8BindFlag = 1;
			g_Token_len = pTokenLen; 
			return UM_CON;
		}
    if(COAP_TYPE_RESET == t)//bind failed
    {
		  memset(g_Token,0xf1,sizeof(g_Token));
			g_stUminiMgr.u8BindFlag = 0; 
			g_Token_len = 8;
			return UM_CON;
		}
		if(COAP_TYPE_ACK == t && COAP_METHOD_EMPTY == code)//encode/decode success&&commond
		{
			return UM_NON;
		}

		return UM_CON;//defualt no ack

}
#if 0
UM_API_FUNC UM_RET  NetRecv(uint8_t *buff, unsigned int uiLen)
{
	  
	
  #if 0 
	//(1)coap对pucData进行解析得到****，UMini_CoapDecode
	 if(COAP_TYPE_ACK==pkt->hdr.t)
   {
    g_stUminiMgr.u8BindFlag = EN_UMINI_STA_BIND;
		 //todo
	  //g_board_func.pfnDataWrite(g_board_func.flashAddr, 0, 512);
		return UM_OK;
	 }
	else if(COAP_RSPCODE_CREATED==pkt->hdr.code||COAP_RSPCODE_CHANGED==pkt->hdr.code)//1.平台订阅后响应:      ip:port+2.05content
	{  /*带 coap Token 发送被动响应1*/
		 if(UM_NULL!=pkt->tok.p)
		 {
			 //todo
		  g_board_func.pfnDataWrite(g_board_func.flashAddr,pkt->tok.p ,pkt->tok.len);
		  return UM_OK;
		 }
		 else{
		   return UM_ERR;
		 } 
	}
	else if(COAP_RSPCODE_CREATED==pkt->hdr.code)  //2.平台发命令后响应:     ip+port+2.04changed
	{
    //解析CoAP Token，
	  /*带 coap Token 发送被动响应2*/
	  g_board_func.pfnRecvCmd(/*(****)*/, /*(****的长度)*/);
	}
	else{
     return UM_ERR;
	}
	#endif
	          //size_t rsplen = sizeof(buff)+1;   
    int rc;
    coap_packet_t pkt; 
    coap_packet_t rsppkt;
	    //todo
	  //uint8_t scratch_raw[1024]; 
		uint8_t scratch_raw[128];  
    coap_rw_buffer_t scratch_buf; 
    scratch_buf.len=sizeof(scratch_raw);
    scratch_buf.p=scratch_raw;
		rc = coap_parse(&pkt,(const uint8_t*)buff,uiLen/*strlen((const char*)buff)*/);
	  if(0 != rc)        //prase the received "buff" to "pkt"
    {
      //printf("Bad packet rc=%d\r\n", rc);
		  return UM_ERR;
    }
    else if(COAP_TYPE_CON==pkt.hdr.t && COAP_METHOD_GET==pkt.hdr.code && COAP_OPTION_OBSERVE==pkt.opts[0].num)//bind success
    {
		  memcpy(g_Token,pkt.tok.p,pkt.tok.len);
			g_stUminiMgr.u8BindFlag=1;
			g_Token_len = pkt.tok.len; 
		}
		else if(COAP_TYPE_CON==pkt.hdr.t&&COAP_METHOD_POST==pkt.hdr.code&&'t'==pkt.opts[0].buf.p[0])//encode/decode success&&commond
		{
		}
    else if(COAP_TYPE_RESET==pkt.hdr.t)//bind failed
    {
		  memset(g_Token,0xf1,sizeof(g_Token));
			g_stUminiMgr.u8BindFlag=0; 
			g_Token_len = 8;
		}
		//else if(COAP_TYPE_ACK==pkt.hdr.t&&COAP_METHOD_EMPTY==pkt.hdr.code)//encode/decode success&&commond
		//{
		//	return   UM_OK;
		//}
	  size_t rsplen = uiLen+1;//sizeof(buff)+1;                                         
    coap_handle_req(&scratch_buf, &pkt, &rsppkt);                  //handle the prased  pkt then make response pkt
		rc = coap_build((uint8_t*)buff, &rsplen, &rsppkt);
    if (0 != rc) //
    {
       return  UM_ERR;
    }
    else
    {  
				 g_board_func.pfnSendData(g_board_func.iSocket, buff,rsplen);
				 return   UM_OK;
    }
}
#endif

//10秒定时器
void TimerPro(UM_VOID)
{
	if(0 == g_stUminiMgr.u8BindFlag)
	{
		umini_bind();//如果 g_flash 内容为空，即无AppID、DeviceID、DeviceSecret等，说明还没绑定。
	}
}

UM_API_FUNC UM_VOID UMnini_Reset(UM_VOID)
{
	size_t buflen;	
	UM_COAPPKT pkt;
	pkt = g_coap_func.pfnCoapPktMalloc();
	g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_RESET,0,COAP_RSPCODE_RESET);
	buflen = (size_t)sizeof(g_outbuf);

	g_coap_func.pfnCoapBuild(g_outbuf, &buflen, pkt);
	g_coap_func.pfnCoapPktfree(pkt);
	g_board_func.pfnSendData(g_board_func.iSocket, g_outbuf,buflen);
}

//【3】==================================关键问题==================================
//1、【ok方案一】由于硬件受限，无法打日志，那错误信息如何定位？——>方案一：AgentTiny提供pringLog接口，输出错误or调试信息，至于设备厂家是液晶、数码管、led还是咋样就自己实现？
//
//1【ok,重新走流程，重新绑定】ig生成的coap token，NB设备要持久化存储？——>因为NB设备第一次上电要bind，得到coap token；然后下电再上电就不能再bind，此时report时是否拿着之前的coap token去？

//2、【ok，写入flash，使用串口可配置】NB设备上对于server ip/port如何获取？
//
