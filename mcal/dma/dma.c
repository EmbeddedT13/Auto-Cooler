#include "dma.h"
#include "../../core/BIT_MATH.h"



void DMA_Init(volatile dma_stream_t *Stream, DMA_Config_t *Config){

    CLEAR_BIT(Stream->DMA_CR, 0); /*The stream must be disabled before configuring it */
    while(READ_BIT(Stream->DMA_CR, 0) == 1); /*Making sure it is disabled before configuring it*/

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x07, 25, Config->Channel);      /* Channel selection */

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x03, 6,  Config->Direction);    /* Data transfer direction */

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x03, 13, Config->MemDataSize);  /* Memory data size */

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x03, 11, Config->PerDataSize);  /* Peripheral data size */

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x01, 10, Config->MemIncrement); /* Memory increment mode */

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x01, 9,  Config->PerIncrement); /* Peripheral increment mode */

    WRITE_BIT_FIELD(Stream->DMA_CR, 0x01, 8, Config->CircularMode);  /* Circular mode */
}


void DMA_SetAddresses(volatile dma_stream_t *Stream, uint32 PeripheralAddress, uint32 MemoryAddress, uint16 NumberOfTransfers){

    /* Assign the source and destination addresses */
    Stream->DMA_PAR = PeripheralAddress;
    Stream->DMA_M0AR = MemoryAddress;


    Stream->DMA_NDTR = NumberOfTransfers; /* Set the number of data items to transfer */
}


void DMA_Enable(volatile dma_stream_t *Stream){

    
    SET_BIT(Stream->DMA_CR, 0); /* Enable the stream */
}

