/***********************************************************************************************************************
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "rm_comms_i2c.h"
#include "rm_comms_api.h"

#include "rs_gp_i2c.h"

#if GP_I2C_PRESET_CONFIG_USE
#if defined(BOARD_RA4E2_FPB)

// HAL DATA
/* I2C Callback */
#ifndef comms_i2c_callback
void comms_i2c_callback(rm_comms_callback_args_t *p_args);
#endif

/* I2C Communication Device */
extern const rm_comms_instance_t g_comms_i2c_device0_ref;
extern rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl_ref;
extern const rm_comms_cfg_t g_comms_i2c_device0_cfg_ref;

#ifndef rm_comms_i2c_callback
void rm_comms_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

// COMMON DATA
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_controller0_ref;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_b_master_instance_ctrl_t g_i2c_controller0_ctrl_ref;
extern const i2c_master_cfg_t g_i2c_controller0_cfg_ref;

/* I2C Shared Bus */
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus0_extended_cfg_ref;



#endif
#endif
