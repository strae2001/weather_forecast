#include "esp8266.h"
#include "OLED.h"
#include "led.h"
#include "uart1.h"
#include "delay.h"

uint8_t Rx2_Buf[RX2BUF_SIZE] = {0};	// ���ճ����ݻ�����

uint16_t recv_cnt = 0;
uint16_t recv_PreCnt = 0;

UART_HandleTypeDef huart2;

void esp8266_uart_Init(void)
{
	huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.Mode = UART_MODE_TX_RX;        // �շ�ģʽ
    huart2.Init.Parity = UART_PARITY_NONE;     // ����ҪУ��λ
    huart2.Init.StopBits = UART_STOPBITS_1;    // 1λֹͣλ
    huart2.Init.WordLength = UART_WORDLENGTH_8B;       // ÿ�����ݴ���8λ
    HAL_UART_Init(&huart2);

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);    // ʹ��UART2 ��ʹ��UART_IT_RXNE���������ж�
}

void USART2_IRQHandler(void)
{
	static uint8_t recv = 0;
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE) != RESET)
	{
		
		if(recv_cnt >= RX2BUF_SIZE)
			recv_cnt = 0;
		HAL_UART_Receive(&huart2, &recv, 1, 1000);
		Rx2_Buf[recv_cnt++] = recv;
	}
}

uint8_t wait_uart2_recv(void)
{
	if(recv_cnt == 0)
	{
		return RECV_NONE;
	}
	if(recv_cnt == recv_PreCnt)
	{
		Rx2_Buf[recv_cnt] = '\0';		// ÿ������һ�������� �ڻ�����������\0
		recv_cnt = 0;					// ������������0
		return RECV_OK;					// ���ؽ��ճɹ���ʶ
	}
	
	recv_PreCnt = recv_cnt;
	return RECV_NONE;
}

uint8_t get_weatherData_fromServer(uint8_t date)
{
	char http_request[256] = {0};
	sprintf(http_request, "GET https://api.seniverse.com/v3/weather/daily.json?key=%s&location=%s&language=zh-Hans&unit=c&start=%d&days=%d\r\n",
	API_KEY, LOCATION, date, DAY_CNT);

	// ����http�����������, �������������ݽ��
	if(sendCmd_toEsp8266(http_request, "results") == RECV_OK)
	{
		printf("%s\r\n", Rx2_Buf);
		return RECV_OK;
	}
	else
		printf("���ݽ��մ���!\r\n");

	return RECV_NONE;
}

uint16_t copy_Rx2Buf_data(char* resbuf)
{
	memcpy(resbuf, Rx2_Buf, strlen((const char*)Rx2_Buf));
	return strlen((const char*)Rx2_Buf);
}

/*	stm32��esp8266����ָ�� �����ջظ�	*/
uint8_t sendCmd_toEsp8266(char* cmd, char* reply)
{
	uint8_t time_out = 200;	
	Rx2_Buf_clean();				// ��ս��ջ�����
	sendString_toEsp8266(cmd);		// ����ָ��
	
	while(time_out--)
	{
		if(wait_uart2_recv() == RECV_OK)
		{
			if(strstr((char*)Rx2_Buf, reply) != NULL)
				return RECV_OK;
		}
		delay_ms(6);
	}
	
	return RECV_ERROR;
}

uint8_t Esp8266_on_line(void)
{
	return sendCmd_toEsp8266("AT\r\n", "OK");
}

uint8_t set_Esp8266_mode(uint8_t mode)
{
	switch (mode)
	{
	case ESP8266_STA_MODE:
		return sendCmd_toEsp8266("AT+CWMODE=1\r\n", "OK");

	case ESP8266_AP_MODE:
		return sendCmd_toEsp8266("AT+CWMODE=2\r\n", "OK");

	case ESP8266_STA_AP_MODE:
		return sendCmd_toEsp8266("AT+CWMODE=3\r\n", "OK");
	
	default:
		return ESP8266_INVALID_MODE;
	}
}

uint8_t Esp8266_join_ap(char* ssid, char* pwd)
{
	char cmd[64]={0};
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);

	return sendCmd_toEsp8266(cmd, "WIFI GOT IP");
}

uint8_t Esp8266_ConnectionMode(uint8_t mode)
{
	switch (mode)
	{
	case ESP8266_SINGLE_CONNECTION:
		return sendCmd_toEsp8266("AT+CIPMUX=0\r\n", "OK");

	case ESP8266_MULTI_CONNECTION:
		return sendCmd_toEsp8266("AT+CIPMUX=1\r\n", "OK");

	default:
		return ESP8266_INVALID_MODE;
	}
}

uint8_t Esp8266_Connect_TcpServer(char* ip, char* port)
{
	char cmd[64] = {0};
	sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", ip, port);
	return sendCmd_toEsp8266(cmd, "CONNECT");
}

uint8_t Esp8266_openSeriaNet(void)
{
	uint8_t ret;
	ret = sendCmd_toEsp8266("AT+CIPMODE=1\r\n", "OK");
	ret += sendCmd_toEsp8266("AT+CIPSEND\r\n", ">");

	if(ret == RECV_OK*2)
		return RECV_OK;
	return RECV_ERROR;
}

uint8_t Esp8266_closeSeriaNet(void)
{
	char cmd[64] = {0};
	sprintf(cmd, "+++");
	return sendCmd_toEsp8266(cmd, "");
}

void Esp8266_asClient(char* ipAddr_Ser, char* port_Ser)
{
	printf("��������tcp������(server_ip:%s, port:%s)...\r\n", ipAddr_Ser, port_Ser);
	while(!Esp8266_Connect_TcpServer(ipAddr_Ser, port_Ser));		// ���ӷ�����
	printf("%s\r\n", Rx2_Buf);

	printf("���ڿ���͸��...\r\n");
	while(!Esp8266_openSeriaNet());									// ����͸��
	printf("%s\r\n", Rx2_Buf);

	printf("�ѳɹ�����TCP������,������͸��ģʽ��\r\n");
	printf("���ڿ��Կ�ʼͨ���ˣ�\r\n");
}

/*	esp8266�豸ģʽ������wifi��ʼ��	*/
void Esp8266_Init(void)
{
	printf("esp8266��ʼ����ʼ...\r\n");
	esp8266_uart_Init();
	
	// esp8266������ʼ��
	printf("���ڲ���esp8266�Ƿ���������...\r\n");
	while(!Esp8266_on_line())
	{
		delay_ms(500);
	}
	printf("esp8266: %s\r\n", Rx2_Buf);

	printf("���ù���ģʽΪSTA(�豸ģʽ)...\r\n");
	while(!set_Esp8266_mode(ESP8266_STA_MODE))
	{
		delay_ms(500);
	}
	printf("esp8266: %s\r\n", Rx2_Buf);

	printf("���õ�����ģʽ...\r\n");
	while(!Esp8266_ConnectionMode(ESP8266_SINGLE_CONNECTION))
	{
		delay_ms(500);
	}
	printf("esp8266: %s\r\n", Rx2_Buf);

	printf("��������wifi: %s...\r\n", WIFI_SSID);
	while(!Esp8266_join_ap(WIFI_SSID, WIFI_PWD))
	{
		delay_ms(1500);
	}
	printf("esp8266: %s\r\n", Rx2_Buf);

	printf("esp8266��ʼ�����...\r\n");
}


/*	 �������ݷ�װ	*/
void serial2_sendByte(uint8_t Byte)
{
	HAL_UART_Transmit(&huart2, &Byte, 1, 100);
}

void sendString_toEsp8266(char* str)
{
	for(uint8_t i=0; str[i]!='\0'; i++)
	{
		serial2_sendByte(str[i]);
	}
}

// �������ݻ���������
void Rx2_Buf_clean(void)
{
	memset(Rx2_Buf, 0, sizeof(Rx2_Buf));
	recv_cnt = 0;
}
