#ifndef _DATAFLASH_MANAGER_H_
#define _DATAFLASH_MANAGER_H_

	/* Includes: */
		#include <avr/io.h>

		#include "N64CartridgeReader.h"
		#include "Descriptors.h"
		#include "Lib/N64Cart.h"

		#include <LUFA/Common/Common.h>
		#include <LUFA/Drivers/USB/USB.h>

	/* Defines: */
		/** Block size of the device. This is kept at 512 to remain compatible with the OS despite the underlying
		 *  storage media (Dataflash) using a different native block size. Do not change this value.
		 */
		#define VIRTUAL_MEMORY_BLOCK_SIZE           (1 << ROM_BLOCK_SIZE)

		/** Total number of blocks of the virtual memory for reporting to the host as the device's total capacity. Do not
		 *  change this value; change VIRTUAL_MEMORY_BYTES instead to alter the media size.
		 */
		#define VIRTUAL_MEMORY_BLOCKS               (262144)

		/** Total number of bytes of the storage medium, comprised of one or more Dataflash ICs. */
		#define VIRTUAL_MEMORY_BYTES                (VIRTUAL_MEMORY_BLOCKS << ROM_BLOCK_SIZE)


	/* Function Prototypes: */
		void DataflashManager_WriteBlocks(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo,
		                                  const uint32_t BlockAddress,
		                                  uint16_t TotalBlocks);
		void DataflashManager_ReadBlocks(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo,
		                                 const uint32_t BlockAddress,
		                                 uint16_t TotalBlocks);
		bool DataflashManager_CheckDataflashOperation(void);

#endif

