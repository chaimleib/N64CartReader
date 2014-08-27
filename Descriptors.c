#include "Descriptors.h"

/** Device descriptor structure. This descriptor, located in FLASH memory,
 * describes the overall device characteristics, including the supported USB
 * version, control endpoint size and the number of device configurations. The
 * descriptor is read out by the USB host when the enumeration process begins.
 */
USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
    .Header                 = {
                                .Size = sizeof(USB_Descriptor_Device_t),
                                .Type = DTYPE_Device
                              },

    .USBSpecification       = VERSION_BCD(01.10),
    .Class                  = USB_CSCP_NoDeviceClass,
    .SubClass               = USB_CSCP_NoDeviceSubclass,
    .Protocol               = USB_CSCP_NoDeviceProtocol,

    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

    .VendorID               = 0x03EB,
    .ProductID              = 0x2002,
    .ReleaseNumber          = VERSION_BCD(00.01),

    .ManufacturerStrIndex   = 0x01,
    .ProductStrIndex        = 0x02,
    .SerialNumStrIndex      = 0x01,

    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH
 * memory, describes the usage of the device in one of its supported
 * configurations, including information about any device interfaces and
 * endpoints. The descriptor is read out by the USB host during the enumeration
 * process when selecting a configuration so that the host may correctly
 * communicate with the USB device.
 */
USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
    .Config =
        {
            .Header                 = {
                                        .Size = sizeof(USB_Descriptor_Configuration_Header_t),
                                        .Type = DTYPE_Configuration
                                      },

            .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
            .TotalInterfaces        = 1,

            .ConfigurationNumber    = 1,
            .ConfigurationStrIndex  = NO_DESCRIPTOR,

            .ConfigAttributes       = USB_CONFIG_ATTR_BUSPOWERED,

            .MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
        },

    .MS_Interface =
        {
            .Header                 = {
                                        .Size = sizeof(USB_Descriptor_Interface_t), 
                                        .Type = DTYPE_Interface
                                      },

            .InterfaceNumber        = 0,
            .AlternateSetting       = 0,

            .TotalEndpoints         = 2,

            .Class                  = MS_CSCP_MassStorageClass,
            .SubClass               = MS_CSCP_SCSITransparentSubclass,
            .Protocol               = MS_CSCP_BulkOnlyTransportProtocol,

            .InterfaceStrIndex      = NO_DESCRIPTOR
        },

    .MS_DataInEndpoint =
        {
            .Header                 = {
                                        .Size = sizeof(USB_Descriptor_Endpoint_t), 
                                        .Type = DTYPE_Endpoint
                                      },

            .EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | MASS_STORAGE_IN_EPNUM),
            .Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
            .EndpointSize           = MASS_STORAGE_IO_EPSIZE,
            .PollingIntervalMS      = 0x01
        },

    .MS_DataOutEndpoint =
        {
            .Header                 = {
                                        .Size = sizeof(USB_Descriptor_Endpoint_t), 
                                        .Type = DTYPE_Endpoint
                                      },

            .EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_OUT | MASS_STORAGE_OUT_EPNUM),
            .Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
            .EndpointSize           = MASS_STORAGE_IO_EPSIZE,
            .PollingIntervalMS      = 0x01
        }
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is
 * returned when the host requests the string descriptor with index 0 (the
 * first index). It is actually an array of 16-bit integers, which indicate via
 * the language ID table available at USB.org what languages the device
 * supports for its string descriptors.
 */
USB_Descriptor_String_t PROGMEM LanguageString =
{
    .Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},

    .UnicodeString          = {LANGUAGE_ID_ENG}
};

/** Manufacturer descriptor string. This is a Unicode string containing the
 * manufacturer's details in human readable form, and is read out upon request
 * by the host when the appropriate string ID is requested, listed in the
 * Device Descriptor.
 */
USB_Descriptor_String_t PROGMEM ManufacturerString =
{
    .Header                 = {.Size = USB_STRING_LEN(18), .Type = DTYPE_String},

    .UnicodeString          = L"Chaim-Leib Halbert"
};

/** Product descriptor string. This is a Unicode string containing the
 * product's details in human readable form, and is read out upon request by
 * the host when the appropriate string ID is requested, listed in the Device
 * Descriptor.
 */
USB_Descriptor_String_t PROGMEM ProductString =
{
    .Header                 = {.Size = USB_STRING_LEN(22), .Type = DTYPE_String},

    .UnicodeString          = L"N64 Cartridge Reader"
};

/** This function is called by the library when in device mode, and must be
 * overridden (see library "USB Descriptors" documentation) by the application
 * code so that the address and size of a requested descriptor can be given to
 * the USB library. When the device receives a Get Descriptor request on the
 * control endpoint, this function is called so that the descriptor details can
 * be passed back and the appropriate descriptor sent back to the USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress) {
    const uint8_t  DescriptorType   = (wValue >> 8);
    const uint8_t  DescriptorNumber = (wValue & 0xFF);

    const void* Address = NULL;
    uint16_t    Size    = NO_DESCRIPTOR;

    switch (DescriptorType) {
    case DTYPE_Device:
        Address = &DeviceDescriptor;
        Size    = sizeof(USB_Descriptor_Device_t);
        break;
    case DTYPE_Configuration:
        Address = &ConfigurationDescriptor;
        Size    = sizeof(USB_Descriptor_Configuration_t);
        break;
    case DTYPE_String:
        switch (DescriptorNumber) {
        case 0x00:
            Address = &LanguageString;
            Size    = pgm_read_byte(&LanguageString.Header.Size);
            break;
        case 0x01:
            Address = &ManufacturerString;
            Size    = pgm_read_byte(&ManufacturerString.Header.Size);
            break;
        case 0x02:
            Address = &ProductString;
            Size    = pgm_read_byte(&ProductString.Header.Size);
            break;
        }

        break;
    }

    *DescriptorAddress = Address;
    return Size;
}

