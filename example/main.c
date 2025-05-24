/*******************************************************************************
File name    :main.c
Author       :ZSW
Version      :V1.0
Date         :2025-05-24
Description  :SCPI协议栈演示例程
Others       :
History Date  Version     Author                Modification
2025-05-24     V1.0        ZSW                    初始版本
*******************************************************************************/
#include "scpi.h"
#include <string.h>
#include <stdio.h>
/******************************************************************************/
#define TXRX_BUFFER_LEN             24 /* 发送和接收缓冲区长度 */
/******************************* 全局变量声明 *********************************/
/******************************* 内部变量声明 *********************************/
/******************************* 内部函数声明 *********************************/
/******************************************************************************/
int main()
{
    char txbuf[TXRX_BUFFER_LEN];
    char rxbuf[TXRX_BUFFER_LEN];
    uint16_t txlen = 0;
    uint16_t Err = 0;

    for (uint8_t i = 0; i < TXRX_BUFFER_LEN; i++)
    {
        txbuf[i] = rxbuf[i] = 0;
    }

    fgets(rxbuf, 20, stdin);

    Err = SCPI_ParseAndExecute(rxbuf, TXRX_BUFFER_LEN, txbuf, &txlen);

    printf("------------------------------------\n");
    printf("执行结果:\n");
    if (Err == 0)
    {
        printf("%s\n", txbuf);
    }
    else
    {
        printf("Err:%d\n", Err);
    }
    return 0;
}
/***************************** 内部功能函数 -- start **************************/
/***************************** 内部功能函数 -- end ****************************/

