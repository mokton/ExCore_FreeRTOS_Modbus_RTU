#ifndef _ESP8266_H_
#define _ESP8266_H_


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

#define ESP8266_RESET_Pin	GPIO_Pin_4


#define ESP8266_MODE			"AT+CWMODE_CUR=1\r\n"  //OK    // STA Mode
#define ESP8266_WIFI_INFO		"AT+CWJAP_CUR=\"Wireless_AP_Home\",\"mQl@XmJ$home_AP*1\"\r\n"  //WIFI GOT IP\r\n\r\nOK
#define ESP8266_WIFI_CLOSE		"AT+CWQAP\r\n"  //OK\r\nWIFI DISCONNECT

#define ESP8266_NET_INFO		"AT+CIPSTART=\"TCP\",\"192.168.1.107\",20000\r\n"

#define ESP8266_MUX_LINK		"AT+CIPMUX=1\r\n"
#define ESP8266_NET_SERVER		"AT+CIPSERVER=1,502\r\n"
#define ESP8266_NET_SERVER_CAL	"AT+CIPSERVER=0\r\n"

#define ESP8266_NET_SEND		"AT+CIPSEND\r\n"
#define ESP8266_NET_CLOSE		"AT+CIPCLOSE\r\n"
#define ESP8266_NET_MODE		"AT+CIPMODE=1\r\n"  //透传
#define ESP8266_NET_QUIT		"+++" 				//推出透传


void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *data, unsigned short len);

_Bool ESP8266_SendCmd(char *cmd, char *res);

#endif
