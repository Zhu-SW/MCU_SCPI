/**
  * @file scpi_cmd.c
  * @author ZSW
  * @brief SCPI命令回调函数实现
  * @version 1.0
  * @date 2025-05-12
  *
  * SPDX-License-Identifier: BSD 3-Clause
  * Copyright (c) 2025, ZSW
  * All rights reserved.
  */

#include "scpi.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/******************************************************************************/
#define ON_ASCII          "ON\0"
#define OFF_ASCII         "OFF\0"
/******************************* 全局变量声明 *********************************/
/******************************* 内部变量声明 *********************************/
/******************************* 内部函数声明 *********************************/
static int IsNumericString(const char *str);
static float ASCIIToNumber(const char *str);
static uint16_t ScaleAndConvertToASCIIWithDecimal(uint32_t num, uint32_t divisor, char *str, uint16_t base);
/******************************************************************************/
/**
  * @brief
  *
  * @param COM
  * @param Args
  * @retval uint16_t
  */
uint16_t SCPI_SourceSetVoltage_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    /* 检查参数是否为数字字符串 */
    if (IsNumericString(Args) != 0)
    {
        return SCPI_ERROR_INVALID_PARAM;
    }

    float Voltage = ASCIIToNumber(Args);

    /* 检查电压是否为正数 */
    if (Voltage < 0)
    {
        return SCPI_ERROR_INVALID_PARAM;
    }

    // Voltage *= 1000;
    // *TxLen = 0;

    // return 0;

    uint32_t V = (uint32_t)(Voltage);
    *TxLen = ScaleAndConvertToASCIIWithDecimal(V, 1, TxBuffer, 10);

    return 0;
}

uint16_t SCPI_SourceSetCurrent_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
//     /* 检查参数是否为数字字符串 */
//     if (IsNumericString(Args) != 0)
//     {
//         return SCPI_ERROR_INVALID_PARAM;
//     }

//     float Current = ASCIIToNumber(Args);

//     /* 检查电压是否为正数 */
//     if (Current < 0)
//     {
//         return SCPI_ERROR_INVALID_PARAM;
//     }

//     Current *= 10000;
//     *TxLen = 0;

//     return 0;

    uint32_t Voltage = 1000;
    *TxLen = ScaleAndConvertToASCIIWithDecimal(Voltage, 1, TxBuffer, 10);
}

uint16_t SCPI_SourceQueryVoltage_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    uint32_t Voltage = 110;
    *TxLen = ScaleAndConvertToASCIIWithDecimal(Voltage, 1, TxBuffer, 10);
    return 0;
}

uint16_t SCPI_SourceQueryCurrent_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    uint32_t Current = 120;
    *TxLen = ScaleAndConvertToASCIIWithDecimal(Current, 1, TxBuffer, 10);
    return 0;
}

uint16_t SCPI_MeasureVoltage_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    TxBuffer[0] = 'M';
    TxBuffer[1] = 'e';
    TxBuffer[2] = 'a';
    TxBuffer[3] = 's';
    TxBuffer[4] = 'u';
    TxBuffer[5] = 'r';
    TxBuffer[6] = 'e';
    TxBuffer[7] = 'V';
    TxBuffer[8] = 'o';
    TxBuffer[9] = 'l';
    TxBuffer[10] = 't';
    TxBuffer[11] = 'a';
    TxBuffer[12] = 'g';
    TxBuffer[13] = 'e';

    TxBuffer[14] = '\0';
    return 0;
}

uint16_t SCPI_MeasureCurrent_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    TxBuffer[0] = 'M';
    TxBuffer[1] = 'e';
    TxBuffer[2] = 'a';
    TxBuffer[3] = 's';
    TxBuffer[4] = 'u';
    TxBuffer[5] = 'r';
    TxBuffer[6] = 'e';
    TxBuffer[7] = 'C';
    TxBuffer[8] = 'u';
    TxBuffer[9] = 'r';
    TxBuffer[10] = 'r';
    TxBuffer[11] = 'e';
    TxBuffer[12] = 'n';
    TxBuffer[13] = 't';

    TxBuffer[14] = '\0';
    return 0;
}

uint16_t SCPI_OutputSet_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    TxBuffer[0] = 'O';
    TxBuffer[1] = 'u';
    TxBuffer[2] = 't';
    TxBuffer[3] = 'p';
    TxBuffer[4] = 'u';
    TxBuffer[5] = 't';
    TxBuffer[6] = 'S';
    TxBuffer[7] = 'e';
    TxBuffer[8] = 't';

    TxBuffer[9] = '\0';
    return 0;
}

uint16_t SCPI_OutputQuery_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args)
{
    TxBuffer[0] = 'O';
    TxBuffer[1] = 'u';
    TxBuffer[2] = 't';
    TxBuffer[3] = 'p';
    TxBuffer[4] = 'u';
    TxBuffer[5] = 't';
    TxBuffer[6] = 'Q';
    TxBuffer[7] = 'u';
    TxBuffer[8] = 'e';
    TxBuffer[9] = 'r';
    TxBuffer[10] = 'y';

    TxBuffer[11] = '\0';
    return 0;
}
/***************************** 内部功能函数 -- start **************************/
/**
  * @brief 检查字符串是否为数字字符串（支持整数和小数）
  * @param str 输入的字符串
  * @return 0 表示是数字字符串，1 表示不是数字字符串
  */
static int IsNumericString(const char *str)
{
    if (str == NULL || *str == '\0')
    {
        return 1; /* 空字符串或 NULL 不是数字字符串 */
    }

    /* 标记是否遇到小数点 */
    int hasDecimalPoint = 0;

    /* 如果第一个字符是正负号，跳过 */
    if (*str == '+' || *str == '-')
    {
        str++;
    }

    /* 遍历字符串 */
    while (*str)
    {
        if (*str == '.')
        {
            /* 如果已经遇到过小数点，则不是数字字符串 */
            if (hasDecimalPoint)
            {
                return 1;
            }
            hasDecimalPoint = 1; /* 标记遇到小数点 */
        }
        else if (*str == '\0')
        {
            return 0;
        }
        else if (!isdigit(*str))
        {
            /* 如果不是数字字符 */
            return 1;
        }
        str++;
    }

    /* 是数字字符串 */
    return 0;
}

/**
  * @brief 将数字字符串转换为浮点数
  * @param str 输入的数字字符串
  * @return 转换后的浮点数值
  */
static float ASCIIToNumber(const char *str)
{
    if (str == NULL || *str == '\0')
    {
        return -1.0f; /* 参数无效，返回 0 */
    }

    float result = 0.0f;
    float fraction = 0.0f;
    int sign = 1;        /* 标记正负号 */
    int hasFraction = 0; /* 标记是否遇到小数点 */
    float fractionDivider = 10.0f;

    /* 处理正负号 */
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    /* 遍历字符串 */
    while (*str)
    {
        /* 如果是数字字符 */
        if (isdigit(*str))
        {
            if (hasFraction)
            {
                /* 处理小数部分 */
                fraction += (*str - '0') / fractionDivider;
                fractionDivider *= 10.0;
            }
            else
            {
                /* 处理整数部分 */
                result = result * 10.0 + (*str - '0');
            }
        }
        else if (*str == '.') /* 遇到小数点 */
        {
            if (hasFraction)
            {
                /* 如果已经遇到过小数点，则非法 */
                return -1.0f;
            }
            hasFraction = 1;
        }
        else
        {
            /* 遇到非法字符，停止转换 */
            break;
        }
        str++;
    }

    /* 返回最终结果 */
    return sign * (result + fraction);
}

/**
  * @brief 将正整数缩小指定倍数后转换为字符串
  * @param num 输入的正整数
  * @param divisor 缩小的倍数（必须大于 0）
  * @param str 存储转换结果的字符串缓冲区
  * @param base 进制（支持 10 进制、16 进制等）
  * @return 转换后的字符串占用的字节数（不包括字符串结束符），返回 0 表示失败
  */
static uint16_t ScaleAndConvertToASCIIWithDecimal(uint32_t num, uint32_t divisor, char *str, uint16_t base)
{
    if (str == NULL || divisor == 0 || base != 10)
    {
        return 0; /* 参数无效，返回 0 */
    }

    uint32_t integerPart = num / divisor;    /* 整数部分 */
    uint32_t fractionalPart = num % divisor; /* 小数部分 */

    char *ptr = str; /* 指向字符串缓冲区 */

    /* 转换整数部分 */
    do
    {
        *ptr++ = (integerPart % 10) + '0';
        integerPart /= 10;
    } while (integerPart);

    /* 反转整数部分 */
    char *start = str;
    char *end = ptr - 1;
    while (start < end)
    {
        char tmp = *start;
        *start++ = *end;
        *end-- = tmp;
    }

    if (divisor > 1)
    {
        /* 添加小数点 */
        *ptr++ = '.';

        /* 转换小数部分 */
        for (uint32_t i = 1; i < divisor; i *= 10)
        {
            fractionalPart *= 10;
            *ptr++ = (fractionalPart / divisor) + '0';
            fractionalPart %= divisor;
        }
    }

    /* 添加字符串结束符 */
    *ptr = '\0';
    /* 返回字符串占用的字节数（不包括结束符）*/
    return ptr - str;
}
/***************************** 内部功能函数 -- end ****************************/

