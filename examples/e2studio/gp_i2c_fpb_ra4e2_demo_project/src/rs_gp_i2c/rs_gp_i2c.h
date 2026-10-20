/***********************************************************************************************************************
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#ifndef RS_GP_I2C_RS_GP_I2C_H_
#define RS_GP_I2C_RS_GP_I2C_H_

#include "stdbool.h"
#include "bsp_api.h"
#include "rm_comms_api.h"
#include "rm_comms_i2c.h"

#define GP_I2C_WRITE_BUFFER_SIZE 17

typedef struct st_gp_i2c_cfg {
    // Pointer to Communications Middleware instance. Used for I2C transactions.
    const rm_comms_instance_t * p_comms_instance;
} gp_i2c_cfg_t;

typedef struct st_gp_i2c_ctrl {
    // A flag that is internally used to determine whether the module is open or not
    bool open;
    // Stores the GreenPAK I2C instance-level configuration while the instance is open.
    gp_i2c_cfg_t cfg;
} gp_i2c_ctrl_t;

// Convenience function to open the I2C Controller instance and create the Comms bus mutex+semaphore
// Returns FSP_SUCCESS when successful.
// Returns code from I2C controller if opening I2C controller fails.
fsp_err_t rs_gp_i2c_OpenI2CBus(gp_i2c_cfg_t *p_cfg);

// Convenience function to close the I2C Controller instance and destroy the Comms bus mutex+semaphore
// Returns FSP_SUCCESS when successful.
// Returns code from I2C controller if opening I2C controller fails.
fsp_err_t rs_gp_i2c_CloseI2CBus(gp_i2c_cfg_t *p_cfg);

// Open the GP_I2C instance
// This can only be successfully called while the instance is in the closed state.
// This will instantiate the provided rm_comms_i2c instance.
// Returns FSP_SUCCESS when successful.
// Returns FSP_ERR_ALREADY_OPEN when the instance is already open.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
// Returns FSP_ERR_ABORTED when initialization failed.
fsp_err_t rs_gp_i2c_Open(gp_i2c_ctrl_t *p_ctrl, gp_i2c_cfg_t *p_cfg);

// Close the GP_I2C instance
// This can only be successfully called while the instance is in the open state.
// This will de-instantiate the instance's embedded rm_comms_i2c instance.
// Returns FSP_SUCCESS when successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_Close(gp_i2c_ctrl_t *p_ctrl);

// Write some number of bytes (dictated by p_bytes, length is num_bytes) to the desired address.
// This function is blocking.
// This can only be successfully called while the instance is in the open state.
// Returns FSP_SUCCESS when the write is successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_WRITE_FAILED when the write operation was unsuccessful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_WriteBytes(gp_i2c_ctrl_t *p_ctrl, uint8_t address, const uint8_t *p_bytes, uint8_t num_bytes);

// Write a single byte value to the desired address.
// This function is blocking.
// This can only be successfully called while the instance is in the open state.
// Returns FSP_SUCCESS when the write is successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_WRITE_FAILED when the write operation was unsuccessful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_WriteByte(gp_i2c_ctrl_t *p_ctrl, uint8_t address, const uint8_t byte);

// Read some number of bytes dictated by num_bytes from the desired address. The results will be placed into *p_read_bytes.
// This function is blocking.
// This can only be successfully called while the instance is in the open state.
// Returns FSP_SUCCESS when the write is successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_ABORTED when the read operation was unsuccessful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_ReadBytes(gp_i2c_ctrl_t *p_ctrl, uint8_t address, uint8_t *p_read_bytes, const uint8_t num_bytes);

// Read a single byte to the desired address. The results will be placed into *p_read_byte.
// This function is blocking.
// This can only be successfully called while the instance is in the open state.
// Returns FSP_SUCCESS when the write is successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_ABORTED when the read operation was unsuccessful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_ReadByte(gp_i2c_ctrl_t *p_ctrl, uint8_t address, uint8_t *p_read_byte);

// Read some number of bytes dictated by num_bytes without specifying an address.
// The GreenPAK device will default to reading from the address after the last address read to or written to by the user.
// This can only be successfully called while the instance is in the open state.
// Returns FSP_SUCCESS when the write is successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
// Returns FSP_ERR_ABORTED when the read operation was unsuccessful.
fsp_err_t rs_gp_i2c_CurrentAddressReadBytes(gp_i2c_ctrl_t *p_ctrl, uint8_t *p_read_bytes, const uint8_t num_bytes);

// Read a single byte without specifying an address.
// The GreenPAK device will default to reading from the address after the last address read to or written to by the user.
// This can only be successfully called while the instance is in the open state.
// Returns FSP_SUCCESS when the write is successful.
// Returns FSP_ERR_NOT_OPEN when the instance is not open.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
// Returns FSP_ERR_ABORTED when the read operation was unsuccessful.
fsp_err_t rs_gp_i2c_CurrentAddressReadByte(gp_i2c_ctrl_t *p_ctrl, uint8_t *p_read_byte);

#endif /* RS_GP_I2C_RS_GP_I2C_H_ */
