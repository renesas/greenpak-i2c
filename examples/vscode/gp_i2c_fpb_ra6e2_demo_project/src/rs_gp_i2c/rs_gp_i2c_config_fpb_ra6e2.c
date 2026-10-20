
#if GP_I2C_PRESET_CONFIG_USE
#include <rs_gp_i2c/rs_gp_i2c_config_fpb_ra6e2.h>

gp_i2c_ctrl_t g_gp_i2c_ctrl_ref;
gp_i2c_cfg_t  g_gp_i2c_cfg_ref = {
    .p_comms_instance = &g_comms_i2c_device0_ref,
};


/* I2C Communication Device */
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl_ref;

/* Lower level driver configuration */
const i2c_master_cfg_t g_comms_i2c_device0_lower_level_cfg_ref =
{ .slave = 0x08, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT, .p_callback = rm_comms_i2c_callback, };

const rm_comms_cfg_t g_comms_i2c_device0_cfg_ref =
{ .semaphore_timeout = 1000, .p_lower_level_cfg = (void*) &g_comms_i2c_device0_lower_level_cfg_ref, .p_extend =
          (void*) &g_comms_i2c_bus0_extended_cfg_ref,
  .p_callback = comms_i2c_callback,
#if defined(NULL)
    .p_context          = NULL,
#else
  .p_context = (void*) &NULL,
#endif
        };

const rm_comms_instance_t g_comms_i2c_device0_ref =
{ .p_ctrl = &g_comms_i2c_device0_ctrl_ref, .p_cfg = &g_comms_i2c_device0_cfg_ref, .p_api = &g_comms_on_comms_i2c, };





iic_b_master_instance_ctrl_t g_i2c_controller0_ctrl_ref;
const iic_b_master_extended_cfg_t g_i2c_controller0_extend_ref =
{ .timeout_mode = IIC_B_MASTER_TIMEOUT_MODE_SHORT,
  .timeout_scl_low = IIC_B_MASTER_TIMEOUT_SCL_LOW_ENABLED,
  .smbus_operation = 0,
  /* Actual calculated bitrate: 99800. Actual calculated duty cycle: 50%. */
  .clock_settings.brl_value = 244,
  .clock_settings.brh_value = 243,
  .clock_settings.cks_value = 2,
  .clock_settings.sdod_value = 0,
  .clock_settings.sdodcs_value = 0,
  .iic_clock_freq = 50000000, };
const i2c_master_cfg_t g_i2c_controller0_cfg_ref =
{ .channel = 0, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .p_callback = rm_comms_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IICB0_RXI)
    .rxi_irq             = VECTOR_NUMBER_IICB0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IICB0_TXI)
    .txi_irq             = VECTOR_NUMBER_IICB0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IICB0_TEND)
    .tei_irq             = VECTOR_NUMBER_IICB0_TEND,
#elif defined(VECTOR_NUMBER_IICB0_TEI)
    .tei_irq             = VECTOR_NUMBER_IICB0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IICB0_EEI)
    .eri_irq             = VECTOR_NUMBER_IICB0_EEI,
#elif defined(VECTOR_NUMBER_IICB0_ERI)
    .eri_irq             = VECTOR_NUMBER_IICB0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (2),
  .p_extend = &g_i2c_controller0_extend_ref, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_controller0_ref =
{ .p_ctrl = &g_i2c_controller0_ctrl_ref, .p_cfg = &g_i2c_controller0_cfg_ref, .p_api = &g_i2c_master_on_iic_b };
#if BSP_CFG_RTOS
#if BSP_CFG_RTOS == 1
#if !defined(g_comms_i2c_bus0_recursive_mutex)
TX_MUTEX g_comms_i2c_bus0_recursive_mutex_handle;
CHAR g_comms_i2c_bus0_recursive_mutex_name[] = "g_comms_i2c_bus0 recursive mutex";
#endif
#if !defined(g_comms_i2c_bus0_blocking_semaphore)
TX_SEMAPHORE g_comms_i2c_bus0_blocking_semaphore_handle;
CHAR g_comms_i2c_bus0_blocking_semaphore_name[] = "g_comms_i2c_bus0 blocking semaphore";
#endif
#elif BSP_CFG_RTOS == 2
#if !defined(g_comms_i2c_bus0_recursive_mutex_ref)
SemaphoreHandle_t g_comms_i2c_bus0_recursive_mutex_handle_ref;
StaticSemaphore_t g_comms_i2c_bus0_recursive_mutex_memory_ref;
#endif
#if !defined(g_comms_i2c_bus0_blocking_semaphore_ref)
SemaphoreHandle_t g_comms_i2c_bus0_blocking_semaphore_handle_ref;
StaticSemaphore_t g_comms_i2c_bus0_blocking_semaphore_memory_ref;
#endif
#endif

#if !defined(g_comms_i2c_bus0_recursive_mutex_ref)
/* Recursive Mutex for I2C bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus0_recursive_mutex_ref =
{
    .p_mutex_handle = &g_comms_i2c_bus0_recursive_mutex_handle_ref,
#if BSP_CFG_RTOS == 1 // ThradX
    .p_mutex_name = &g_comms_i2c_bus0_recursive_mutex_name_ref[0],
#elif BSP_CFG_RTOS == 2 // FreeRTOS
    .p_mutex_memory = &g_comms_i2c_bus0_recursive_mutex_memory_ref,
#endif
};
#endif

#if !defined(g_comms_i2c_bus0_blocking_semaphore)
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus0_blocking_semaphore_ref =
{
    .p_semaphore_handle = &g_comms_i2c_bus0_blocking_semaphore_handle_ref,
#if BSP_CFG_RTOS == 1 // ThreadX
    .p_semaphore_name = &g_comms_i2c_bus0_blocking_semaphore_name_ref[0],
#elif BSP_CFG_RTOS == 2 // FreeRTOS
    .p_semaphore_memory = &g_comms_i2c_bus0_blocking_semaphore_memory_ref,
#endif
};
#endif
#endif

/* Shared I2C Bus */
#define RA_NOT_DEFINED (1)
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus0_extended_cfg_ref =
{
#if !defined(g_i2c_controller0)
  .p_driver_instance = (void*) &g_i2c_controller0_ref,
#elif !defined(RA_NOT_DEFINED)
    .p_driver_instance      = (void*)&RA_NOT_DEFINED,
#elif !defined(RA_NOT_DEFINED)
    .p_driver_instance      = (void*)&RA_NOT_DEFINED,
#endif
  .p_current_ctrl = NULL,
  .bus_timeout = 1000,
#if BSP_CFG_RTOS
#if !defined(g_comms_i2c_bus0_blocking_semaphore_ref)
    .p_blocking_semaphore = &g_comms_i2c_bus0_blocking_semaphore_ref,
#if !defined(g_comms_i2c_bus0_recursive_mutex_ref)
    .p_bus_recursive_mutex = &g_comms_i2c_bus0_recursive_mutex_ref,
#else
    .p_bus_recursive_mutex = NULL,
#endif
#else
    .p_bus_recursive_mutex = NULL,
    .p_blocking_semaphore = NULL,
#endif
#else
#endif

#if (0)
    .p_elc = (void*)&g_elc,
    .p_timer = (void*)&g_timer,
#else
  .p_elc = NULL,
  .p_timer = NULL,
#endif
        };
#undef RA_NOT_DEFINED
#endif
