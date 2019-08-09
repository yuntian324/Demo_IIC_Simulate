#ifndef __xBSP_I2C_SIMU_H
#define __xBSP_I2C_SIMU_H
/**************************************************START OF FILE*****************************************************/

/*------------------------------------------------------------------------------------------------------------------
包含头文件
*/
#include "stdint.h"


/*------------------------------------------------------------------------------------------------------------------
函数声明
*/
void xBSP_I2C_Simu_Init(void);
void xBSP_I2C_Simu_Send_Byte(uint8_t txd);
uint8_t xBSP_I2C_Simu_Read_Byte(uint8_t ack);
uint8_t xBSP_I2C_Simu_WriteOneByte(uint8_t Addr, uint8_t reg, uint8_t value);
uint8_t xBSP_I2C_Simu_ReadOneByte(uint8_t Addr, uint8_t Reg);
#endif
/**************************************************END OF FILE**********************************************************/

