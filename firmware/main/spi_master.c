#include "spi_master.h"


static const spi_bus_config_t bus_conf = {
    .miso_io_num = SDO,
    .mosi_io_num = SDA,
    .sclk_io_num = SCL,
    .data_io_default_level = true, // 1
    .max_transfer_sz = 4096
};
