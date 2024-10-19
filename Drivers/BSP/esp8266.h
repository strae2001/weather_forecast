#ifndef __ESP8266_H
#define __ESP8266_H

#include "sys.h"
#include <stdio.h>
#include "string.h"

#define TX2 GPIO_PIN_2      // 引脚定义
#define RX2 GPIO_PIN_3

#define TX2BUF_SIZE	128     // 发送/接收 数据缓冲区
#define RX2BUF_SIZE	1024

//	接收数据标志       
#define RECV_OK   	1
#define RECV_NONE 	2
#define RECV_ERROR 	0

// esp8266模式定义
#define ESP8266_STA_MODE            1
#define ESP8266_AP_MODE             2
#define ESP8266_STA_AP_MODE         3
#define ESP8266_INVALID_MODE        4

// 单连接/多连接模式
#define ESP8266_SINGLE_CONNECTION   0
#define ESP8266_MULTI_CONNECTION    1

// wifi信息
#define WIFI_SSID          "Honor100"              
#define WIFI_PWD           "Cc.246850@"

// 心知天气服务器地址
#define TCP_SER_IP         "api.seniverse.com"
#define TCP_SER_PORT       "80"

// 访问服务器API接口密钥
#define API_KEY            "SW0TYis5C1ku_SYTO"

// 获取对应城市天气数据
#define LOCATION           "zhuhai"

// 一次获取几天的数据
#define DAY_CNT            1

void Esp8266_Init(void);

/// @brief 数据缓冲区清空
void Rx2_Buf_clean(void);

/// @brief 向esp8266发送字符串
void sendString_toEsp8266(char* str);

/**
 * @description: 向esp8266发送指令, 接收esp8266的回复信息并进行核验 
 * @param {char*} cmd       指令信息
 * @param {char*} reply     预期esp8266回复的信息
 * @return {若接收到的信息与预期值相符返回RECV_OK, 否则返回RECV_ERROR}              
 */
uint8_t sendCmd_toEsp8266(char* cmd, char* reply);

/**
 * @description: 指定esp8266作为何种角色运行(设备/路由/设备+路由)
 * @param {uint8_t} mode    模式号{1:设备; 2:路由; 3:设备+路由}
 * @return {*}
 */
uint8_t set_Esp8266_mode(uint8_t mode);

/**
 * @description: 使能esp8266设备模式下连上wifi
 * @param {char*} ssid     wifi名
 * @param {char*} pwd      wifi密码
 * @return {*}
 */
uint8_t Esp8266_join_ap(char* ssid, char* pwd);

/**
 * @description: 指定esp8266连接模式
 * @param {uint8_t} mode    连接模式(单连接; 多连接:只允许在路由模式下使用)
 */
uint8_t Esp8266_ConnectionMode(uint8_t mode);

/// @brief 检测模块是否在线正常运行
/// @return 
uint8_t Esp8266_on_line(void);

/**
 * @description: 使能esp8266作为设备模式, 连接tcp服务器
 * @param {uint8_t*} ip       服务器ip地址 
 * @param {uint8_t*} port     服务器端口号
 * @return {*}
 */
uint8_t Esp8266_Connect_TcpServer(char* ip, char* port);

/**
 * @description: 使能esp8266开启透传
 * @return {*}
 */
uint8_t Esp8266_openSeriaNet(void);
uint8_t Esp8266_closeSeriaNet(void);

/**
 * @description: 使能esp8266作为客户端, 连接tcp服务器
 * @param {uint8_t*} ipAddr_Ser     服务器ip地址
 * @param {uint8_t*} port_Ser       服务器端口号
 * @return {*}
 */
void Esp8266_asClient(char* ipAddr_Ser, char* port_Ser);

/// @brief 从服务器获取天气数据
/// @param date 指定获取哪一天的数据, 0: 今天, 1: 明天, 依次延续
/// @return     返回RECV_OK/RECV_NONE, 是否成功接收到数据
uint8_t get_weatherData_fromServer(uint8_t date);

/// @brief 将Rx2Buf缓冲区的数据拷贝到resbuf
/// @param resbuf   目的缓冲区
/// @return         返回拷贝的数据长度
uint16_t copy_Rx2Buf_data(char* resbuf);

#endif

