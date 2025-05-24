#ifndef __SCPI_H
#define __SCPI_H
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
typedef enum
{
    SCPI_OK = 0,                  /* 成功 */
    SCPI_ERROR_INVALID_PARAM,     /* 参数错误 */
    SCPI_ERROR_COMMAND_NOT_FOUND, /* 未找到匹配的命令 */
    SCPI_ERROR_EXECUTION_FAILED   /* 执行命令失败 */
} eSCPI_Error;
/******************************************************************************/
uint16_t SCPI_ParseAndExecute(const char *pRxBuffer, uint16_t RxLen, char *pTxBuffer, uint16_t *TxLen);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif

