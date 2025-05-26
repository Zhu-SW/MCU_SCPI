/**
  * @file main.c
  * @author ZSW
  * @brief SCPI协议栈演示例程
  * @version 0.1
  * @date 2025-05-26
  *
  * SPDX-License-Identifier: BSD 3-Clause
  * Copyright (c) 2025, ZSW
  * All rights reserved.
  */

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

