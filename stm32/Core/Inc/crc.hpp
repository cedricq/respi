#ifndef INC_CRC_HPP_
#define INC_CRC_HPP_

#include <stdint.h>

enum{
    CHECKSUM_ERROR = 0X04
};

uint8_t SF04_CheckCrc (uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum);
uint8_t SF04_CalcCrc (uint8_t data[], uint8_t nbrOfBytes);


#endif /* INC_CRC_HPP_ */
