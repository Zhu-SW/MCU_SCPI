/*******************************************************************************
File name    :scpi.c
Author       :ZSW
Version      :V1.0
Date         :2025-05-12
Description  :SCPI协议栈
Others       :
History Date  Version     Author                Modification
2025-05-12     V1.0        ZSW                    初始版本
*******************************************************************************/
#include "scpi_cmd.h"
#include "scpi.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
/******************************************************************************/
#define MAX_KEYWORD_LENGTH      32 /* 每个关键字的最大长度 */
#define MAX_PARAM_LENGTH        32 /* 参数值的最大长度 */
/******************************* 全局变量声明 *********************************/
/******************************* 内部变量声明 *********************************/
/* 定义回调函数类型 */
typedef uint16_t (*SCPI_Callback)(char *pTxBuffer, uint16_t *TxLen, const char *Args);

/* 定义SCPI命令树节点结构体 */
typedef struct SCPI_Node
{
    const char *KeyWord;         /* 当前节点的关键字 */
    SCPI_Callback CallBack;      /* 回调函数（仅叶子节点有效）*/
    struct SCPI_Node *pChildren; /* 子节点 */
} SCPI_Node;

/* 命令树定义 */
const static SCPI_Node s_SourceSubCommands[] =
{
    {"VOLTage", SCPI_SourceSetVoltage_Callback, NULL},
    {"CURRent", SCPI_SourceSetCurrent_Callback, NULL},
    {"VOLTage?", SCPI_SourceQueryVoltage_Callback, NULL},
    {"CURRent?", SCPI_SourceQueryCurrent_Callback, NULL},
    {NULL, NULL, NULL},
};

const static  SCPI_Node s_MeasureSubCommands[] =
{
    {"VOLTage?", SCPI_MeasureVoltage_Callback, NULL},
    {"CURRent?", SCPI_MeasureCurrent_Callback, NULL},
    {NULL, NULL, NULL},
};

const static  SCPI_Node s_OutputSubCommands[] =
{
    {"ONOFF", SCPI_OutputSet_Callback, NULL},
    {"ONOFF?", SCPI_OutputQuery_Callback, NULL},
    {NULL, NULL, NULL},
};

const static  SCPI_Node s_SystemSubCommands[] =
{
    {"VERSion?", NULL, NULL},
    {"ERRor?", NULL, NULL},
    {NULL, NULL, NULL},
};

/* 根节点 */
const static SCPI_Node s_Root[] =
{
    {"SOURce", NULL, s_SourceSubCommands},
    {"MEASure", NULL, s_MeasureSubCommands},
    {"OUTPut", NULL, s_OutputSubCommands},
    {"SYSTem", NULL, s_SystemSubCommands},
    {NULL, NULL, NULL},
};
/******************************* 内部函数声明 *********************************/
/******************************************************************************/
/**
  * @brief  解析并执行 SCPI 协议命令
  * @param  pRxBuffer 接收缓冲区指针
  * @param  RxLen 接收缓冲区长度
  * @param  pTxBuffer 发送缓冲区指针
  * @param  TxLen 发送缓冲区长度
  * @retval 错误码，0 表示成功，非 0 表示错误
  */
uint16_t SCPI_ParseAndExecute(const char *pRxBuffer, uint16_t RxLen, char *pTxBuffer, uint16_t *TxLen)
{
    uint16_t Err = SCPI_OK;

    if (pRxBuffer == NULL|| RxLen == 0)
    {
        return SCPI_ERROR_INVALID_PARAM;
    }

    int8_t Command[MAX_KEYWORD_LENGTH] = {0}; /* 用于存储解析出的命令 */
    int8_t Args[MAX_PARAM_LENGTH] = {0};      /* 用于存储命令的参数 */
    uint16_t i = 0, j = 0;

    /* 1. 提取命令部分 */
    while (i < RxLen && pRxBuffer[i] != ' ' && pRxBuffer[i] != '\n' && pRxBuffer[i] != '\r')
    {
        if (j < MAX_KEYWORD_LENGTH - 1)
        {
            Command[j++] = pRxBuffer[i++];
        }
    }
    Command[j] = '\0'; /* 确保命令字符串以 '\0' 结尾 */

    /* 2. 跳过空格，提取参数部分 */
    while (i < RxLen && isspace(pRxBuffer[i]))
    {
        i++;
    }
    j = 0;
    while (i < (RxLen - 1) && j < MAX_PARAM_LENGTH - 1)
    {
        Args[j++] = pRxBuffer[i++];
    }
    Args[j] = '\0'; /* 确保参数字符串以 '\0' 结尾 */

    /* 3. 匹配命令并执行对应的回调函数 */
    SCPI_Node *MatchedNode = NULL;      /* 匹配的节点*/
    SCPI_Node *CurrentNode = s_Root;    /* 从根节点开始解析 */
    char *token = strtok(Command, ":"); /* 按冒号分割命令层级 */

    while (token != NULL)
    {
        uint8_t Found = 0;

        for (uint8_t index = 0; CurrentNode[index].KeyWord != NULL; index++)
        {
            if (strcasecmp(CurrentNode[index].KeyWord, token) == 0)
            {
                MatchedNode = &CurrentNode[index]; /* 记录当前匹配到的节点 */

                if (CurrentNode[index].pChildren != NULL)
                {
                    CurrentNode = CurrentNode[index].pChildren;
                }
                Found = true;
                break;
            }
        }
        if (!Found)
        {
            Err = SCPI_ERROR_COMMAND_NOT_FOUND;
        }
        token = strtok(NULL, ":");
    }

    /* 4. 执行叶子节点的回调函数 */
    /* 只有完全匹配到叶子节点才执行回调 */
    if (MatchedNode && MatchedNode->CallBack != NULL)
    {
        Err = MatchedNode->CallBack(pTxBuffer, TxLen, Args);
    }
    else
    {
        Err = SCPI_ERROR_COMMAND_NOT_FOUND; /* 未找到匹配的命令 */
    }

    return Err; /* 返回错误码 */
}
/***************************** 内部功能函数 -- start **************************/
/***************************** 内部功能函数 -- end ****************************/

