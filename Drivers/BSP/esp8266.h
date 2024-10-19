#ifndef __ESP8266_H
#define __ESP8266_H

#include "sys.h"
#include <stdio.h>
#include "string.h"

#define TX2 GPIO_PIN_2      // ���Ŷ���
#define RX2 GPIO_PIN_3

#define TX2BUF_SIZE	128     // ����/���� ���ݻ�����
#define RX2BUF_SIZE	1024

//	�������ݱ�־       
#define RECV_OK   	1
#define RECV_NONE 	2
#define RECV_ERROR 	0

// esp8266ģʽ����
#define ESP8266_STA_MODE            1
#define ESP8266_AP_MODE             2
#define ESP8266_STA_AP_MODE         3
#define ESP8266_INVALID_MODE        4

// ������/������ģʽ
#define ESP8266_SINGLE_CONNECTION   0
#define ESP8266_MULTI_CONNECTION    1

// wifi��Ϣ
#define WIFI_SSID          "Honor100"              
#define WIFI_PWD           "Cc.246850@"

// ��֪������������ַ
#define TCP_SER_IP         "api.seniverse.com"
#define TCP_SER_PORT       "80"

// ���ʷ�����API�ӿ���Կ
#define API_KEY            "SW0TYis5C1ku_SYTO"

// ��ȡ��Ӧ������������
#define LOCATION           "zhuhai"

// һ�λ�ȡ���������
#define DAY_CNT            1

void Esp8266_Init(void);

/// @brief ���ݻ��������
void Rx2_Buf_clean(void);

/// @brief ��esp8266�����ַ���
void sendString_toEsp8266(char* str);

/**
 * @description: ��esp8266����ָ��, ����esp8266�Ļظ���Ϣ�����к��� 
 * @param {char*} cmd       ָ����Ϣ
 * @param {char*} reply     Ԥ��esp8266�ظ�����Ϣ
 * @return {�����յ�����Ϣ��Ԥ��ֵ�������RECV_OK, ���򷵻�RECV_ERROR}              
 */
uint8_t sendCmd_toEsp8266(char* cmd, char* reply);

/**
 * @description: ָ��esp8266��Ϊ���ֽ�ɫ����(�豸/·��/�豸+·��)
 * @param {uint8_t} mode    ģʽ��{1:�豸; 2:·��; 3:�豸+·��}
 * @return {*}
 */
uint8_t set_Esp8266_mode(uint8_t mode);

/**
 * @description: ʹ��esp8266�豸ģʽ������wifi
 * @param {char*} ssid     wifi��
 * @param {char*} pwd      wifi����
 * @return {*}
 */
uint8_t Esp8266_join_ap(char* ssid, char* pwd);

/**
 * @description: ָ��esp8266����ģʽ
 * @param {uint8_t} mode    ����ģʽ(������; ������:ֻ������·��ģʽ��ʹ��)
 */
uint8_t Esp8266_ConnectionMode(uint8_t mode);

/// @brief ���ģ���Ƿ�������������
/// @return 
uint8_t Esp8266_on_line(void);

/**
 * @description: ʹ��esp8266��Ϊ�豸ģʽ, ����tcp������
 * @param {uint8_t*} ip       ������ip��ַ 
 * @param {uint8_t*} port     �������˿ں�
 * @return {*}
 */
uint8_t Esp8266_Connect_TcpServer(char* ip, char* port);

/**
 * @description: ʹ��esp8266����͸��
 * @return {*}
 */
uint8_t Esp8266_openSeriaNet(void);
uint8_t Esp8266_closeSeriaNet(void);

/**
 * @description: ʹ��esp8266��Ϊ�ͻ���, ����tcp������
 * @param {uint8_t*} ipAddr_Ser     ������ip��ַ
 * @param {uint8_t*} port_Ser       �������˿ں�
 * @return {*}
 */
void Esp8266_asClient(char* ipAddr_Ser, char* port_Ser);

/// @brief �ӷ�������ȡ��������
/// @param date ָ����ȡ��һ�������, 0: ����, 1: ����, ��������
/// @return     ����RECV_OK/RECV_NONE, �Ƿ�ɹ����յ�����
uint8_t get_weatherData_fromServer(uint8_t date);

/// @brief ��Rx2Buf�����������ݿ�����resbuf
/// @param resbuf   Ŀ�Ļ�����
/// @return         ���ؿ��������ݳ���
uint16_t copy_Rx2Buf_data(char* resbuf);

#endif

