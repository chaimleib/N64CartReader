#define  INCLUDE_FROM_DATAFLASHMANAGER_C
#include "DataflashManager.h"

/** Reads blocks (OS blocks, not Dataflash pages) from the storage medium, the
 * board Dataflash IC(s), into the pre-selected data IN endpoint. This routine
 * reads in Dataflash page sized blocks from the Dataflash and writes them in
 * OS sized blocks to the endpoint.
 *
 *  \param[in] MSInterfaceInfo  Pointer to a structure containing a Mass
 *  Storage Class configuration and state
 *  
 *  \param[in] BlockAddress  Data block starting address for the read sequence
 *  
 *  \param[in] TotalBlocks   Number of blocks of data to read
 */
void DataflashManager_ReadBlocks(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo,
                                 const uint32_t BlockAddress,
                                 uint16_t TotalBlocks) {
    uint16_t blockNum;
    // We are writing 16 bytes at a time
    uint8_t BytesInBlockDiv16;
    uint8_t data[VIRTUAL_MEMORY_BLOCK_SIZE], *dataPtr;

    /* Wait until endpoint is ready before continuing */
    if (Endpoint_WaitUntilReady()) return;

    for (blockNum = 0; blockNum < TotalBlocks; blockNum++) {
        getCartBlock(data, BlockAddress + blockNum);
        /* Write an endpoint packet sized data block to the Dataflash */
        for (BytesInBlockDiv16 = 0; 
             BytesInBlockDiv16 < (VIRTUAL_MEMORY_BLOCK_SIZE >> 4); 
             BytesInBlockDiv16++) {
            /* Check if the endpoint is currently full */
            if (!(Endpoint_IsReadWriteAllowed())) {
                /* Clear the endpoint bank to send its contents to the host */
                Endpoint_ClearIN();

                /* Wait until the endpoint is ready for more data */
                if (Endpoint_WaitUntilReady()) return;
            }
            dataPtr = &data[BytesInBlockDiv16 << 4];

            /* Read one 16-byte chunk of data from the Dataflash */
            Endpoint_Write_Byte(dataPtr[0]);
            Endpoint_Write_Byte(dataPtr[1]);
            Endpoint_Write_Byte(dataPtr[2]);
            Endpoint_Write_Byte(dataPtr[3]);
            Endpoint_Write_Byte(dataPtr[4]);
            Endpoint_Write_Byte(dataPtr[5]);
            Endpoint_Write_Byte(dataPtr[6]);
            Endpoint_Write_Byte(dataPtr[7]);
            Endpoint_Write_Byte(dataPtr[8]);
            Endpoint_Write_Byte(dataPtr[9]);
            Endpoint_Write_Byte(dataPtr[10]);
            Endpoint_Write_Byte(dataPtr[11]);
            Endpoint_Write_Byte(dataPtr[12]);
            Endpoint_Write_Byte(dataPtr[13]);
            Endpoint_Write_Byte(dataPtr[14]);
            Endpoint_Write_Byte(dataPtr[15]);

            /* Check if the current command is being aborted by the host */
            if (MSInterfaceInfo->State.IsMassStoreReset) return;
        }
    }

    /* If the endpoint is full, send its contents to the host */
    if (!(Endpoint_IsReadWriteAllowed())) Endpoint_ClearIN();
}

bool DataflashManager_CheckDataflashOperation(void) {
    uint8_t firstBlock[VIRTUAL_MEMORY_BLOCK_SIZE];
    getCartBlock(firstBlock, 0);
    return firstBlock[0]==0x80 && 
           firstBlock[1]==0x37 && 
           firstBlock[2]==0x12 && 
           firstBlock[3]==0x40;
}

