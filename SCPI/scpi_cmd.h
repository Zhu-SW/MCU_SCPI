#ifndef __SCPI_CMD_H
#define __SCPI_CMD_H
/******************************************************************************/
#include "scpi.h"
#include <stdint.h>
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
uint16_t SCPI_SourceSetVoltage_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_SourceSetCurrent_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_SourceQueryVoltage_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_SourceQueryCurrent_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_MeasureVoltage_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_MeasureCurrent_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_OutputSet_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
uint16_t SCPI_OutputQuery_Callback(char *TxBuffer, uint16_t *TxLen, const char *Args);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif

