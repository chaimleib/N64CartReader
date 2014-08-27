#define  INCLUDE_FROM_DATAFLASHMANAGER_C
#include "DataflashManager.h"

uint8_t data[2048];

/** Writes blocks (OS blocks, not Dataflash pages) to the storage medium, the
 * board Dataflash IC(s), from the pre-selected data OUT endpoint. This routine
 * reads in OS sized blocks from the endpoint and writes them to the Dataflash
 * in Dataflash page sized blocks.
 *
 *  \param[in] MSInterfaceInfo  Pointer to a structure containing a Mass
 *  Storage Class configuration and state
 *  
 *  \param[in] BlockAddress  Data block starting address for the write sequence
 *  
 *  \param[in] TotalBlocks   Number of blocks of data to write
 */
void DataflashManager_WriteBlocks(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo,
                                  const uint32_t BlockAddress,
                                  uint16_t TotalBlocks) {
	return;
}

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
	uint8_t *dataPtr;

	/* Wait until endpoint is ready before continuing */
	if (Endpoint_WaitUntilReady()) return;

	for (blockNum = 0; blockNum < TotalBlocks; blockNum++) {
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
			dataPtr = &data[
                (blockNum * VIRTUAL_MEMORY_BLOCK_SIZE) + 
                (BytesInBlockDiv16 << 4)
            ];

			PORTC |= (1<<6);
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
			PORTC &= ~(1<<6);
			/* Check if the current command is being aborted by the host */
			if (MSInterfaceInfo->State.IsMassStoreReset) return;
		}
	}

	/* If the endpoint is full, send its contents to the host */
	if (!(Endpoint_IsReadWriteAllowed())) Endpoint_ClearIN();
}

/** Performs a simple test on the attached Dataflash IC(s) to ensure that they
 * are working.
 *
 *  \return Boolean true if all media chips are working, false otherwise
 */
bool DataflashManager_CheckDataflashOperation(void)
{
	uint8_t TempByte;
	TempByte = data[1];
	data[1] = 0x7f;
	if (data[1] == 0x7f) {
		data[1] = TempByte;
		return true;
	} 
	return false;
}

