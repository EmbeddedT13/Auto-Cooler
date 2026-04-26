#ifndef DMA_H
#define DMA_H

#include "../../core/STD_TYPES.h"

/* DMA Stream Register Map */
typedef struct{
    
    volatile uint32 DMA_CR;
    volatile uint32 DMA_NDTR;
    volatile uint32 DMA_PAR;
    volatile uint32 DMA_M0AR;
    volatile uint32 DMA_M1AR;
    volatile uint32 DMA_FCR;
}dma_stream_t;

/* DMA Controller Register Map */
typedef struct {

    volatile uint32 DMA_LISR;
    volatile uint32 DMA_HISR;
    volatile uint32 DMA_LIFCR;
    volatile uint32 DMA_HIFCR;
    volatile dma_stream_t STREAM[8];
}dma_t;

#define DMA1_BASE_ADDRESS 0x40026000
#define DMA2_BASE_ADDRESS 0x40026400

#define DMA1 ((dma_t *) DMA1_BASE_ADDRESS)
#define DMA2 ((dma_t *) DMA2_BASE_ADDRESS)

/* DMA Channel Selection */
#define DMA_CHANNEL_0  0
#define DMA_CHANNEL_1  1
#define DMA_CHANNEL_2  2
#define DMA_CHANNEL_3  3
#define DMA_CHANNEL_4  4
#define DMA_CHANNEL_5  5
#define DMA_CHANNEL_6  6
#define DMA_CHANNEL_7  7

/* Data Transfer Directions */
#define DMA_DIRECTION_PER_TO_MEM     0
#define DMA_DIRECTION_MEM_TO_PER     1
#define DMA_DIRECTION_MEM_TO_MEM     2

/* Memory Data Sizes */
#define DMA_MEM_DATA_SIZE_BYTE       0
#define DMA_MEM_DATA_SIZE_HALF_WORD  1
#define DMA_MEM_DATA_SIZE_WORD       2

/* Peripheral Data Sizes */
#define DMA_PER_DATA_SIZE_BYTE       0
#define DMA_PER_DATA_SIZE_HALF_WORD  1
#define DMA_PER_DATA_SIZE_WORD       2

/* Address Increment Modes */
#define DMA_MEM_INCREMENT_DISABLE    0
#define DMA_MEM_INCREMENT_ENABLE     1

#define DMA_PER_INCREMENT_DISABLE    0
#define DMA_PER_INCREMENT_ENABLE     1

/* Circular Mode Configuration */
#define DMA_CIRCULAR_DISABLE         0
#define DMA_CIRCULAR_ENABLE          1


void DMA2_Init(void);
void DMA2_SetAddresses(uint32 *PeripheralAddress, uint32 *MemoryAddress, uint16 NumberOfTransfers);
void DMA2_Enable(void);


#endif