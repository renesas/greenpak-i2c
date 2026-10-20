/***********************************************************************************************************************
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#include "rs_gp_i2c.h"
#include "rm_comms_i2c.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>

void comms_i2c_callback (i2c_master_callback_args_t *p_args) {
    FSP_PARAMETER_NOT_USED(p_args);
}

static void _gp_i2c_move_cfg_to_ctrl(gp_i2c_ctrl_t * p_ctrl, const gp_i2c_cfg_t * const p_cfg) {
    gp_i2c_cfg_t * p_dest_cfg = &(p_ctrl->cfg);

    p_dest_cfg->p_comms_instance = p_cfg->p_comms_instance;

};

static void _gp_i2c_clear_cfg(gp_i2c_ctrl_t * p_ctrl) {
    gp_i2c_cfg_t * p_cfg = &(p_ctrl->cfg);

    p_cfg->p_comms_instance = NULL;

};

static bool _gp_i2c_bus_is_open(rm_comms_i2c_bus_extended_cfg_t *p_bus) {
    i2c_master_instance_t * p_driver_instance = (i2c_master_instance_t *) p_bus->p_driver_instance;
    i2c_master_status_t status;

    p_driver_instance->p_api->statusGet(p_driver_instance->p_ctrl, &status);

    return status.open;
}

fsp_err_t rs_gp_i2c_OpenI2CBus(gp_i2c_cfg_t *p_cfg)
{
    FSP_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(NULL != p_cfg->p_comms_instance, FSP_ERR_INVALID_ARGUMENT);

    const rm_comms_instance_t * p_comms_instance = p_cfg->p_comms_instance;
    rm_comms_i2c_bus_extended_cfg_t * p_bus_extend = (rm_comms_i2c_bus_extended_cfg_t *) p_comms_instance->p_cfg->p_extend;


    FSP_ERROR_RETURN(NULL != p_bus_extend, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(false == _gp_i2c_bus_is_open(p_bus_extend), FSP_ERR_ALREADY_OPEN);

    i2c_master_instance_t * p_driver_instance = (i2c_master_instance_t *) p_bus_extend->p_driver_instance;

    fsp_err_t err = p_driver_instance->p_api->open(p_driver_instance->p_ctrl, p_driver_instance->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    // Check for blocking semaphore and recursive mutex
    FSP_ERROR_RETURN(p_bus_extend->p_blocking_semaphore != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_bus_extend->p_bus_recursive_mutex != NULL, FSP_ERR_INVALID_ARGUMENT);

    /* Create a semaphore for blocking */
    *(p_bus_extend->p_blocking_semaphore->p_semaphore_handle) =
    xSemaphoreCreateCountingStatic((UBaseType_t) 1,
                                   (UBaseType_t) 0,
                                   p_bus_extend->p_blocking_semaphore->p_semaphore_memory);

    /* Create a recursive mutex for bus lock */
    *(p_bus_extend->p_bus_recursive_mutex->p_mutex_handle) =
        xSemaphoreCreateRecursiveMutexStatic(p_bus_extend->p_bus_recursive_mutex->p_mutex_memory);

    return FSP_SUCCESS;
}

fsp_err_t rs_gp_i2c_CloseI2CBus(gp_i2c_cfg_t *p_cfg)
{
    FSP_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(NULL != p_cfg->p_comms_instance, FSP_ERR_INVALID_ARGUMENT);

    const rm_comms_instance_t * p_comms_instance = p_cfg->p_comms_instance;
    rm_comms_i2c_bus_extended_cfg_t * p_bus_extend = (rm_comms_i2c_bus_extended_cfg_t *) p_comms_instance->p_cfg->p_extend;

    FSP_ERROR_RETURN(NULL != p_bus_extend, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(_gp_i2c_bus_is_open(p_bus_extend), FSP_ERR_NOT_OPEN);

    i2c_master_instance_t * p_driver_instance = (i2c_master_instance_t *) p_bus_extend->p_driver_instance;

    fsp_err_t err = p_driver_instance->p_api->close(p_driver_instance->p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    // Check for blocking semaphore and recursive mutex
    FSP_ERROR_RETURN(p_bus_extend->p_blocking_semaphore != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_bus_extend->p_bus_recursive_mutex != NULL, FSP_ERR_INVALID_ARGUMENT);

    vSemaphoreDelete(*(p_bus_extend->p_bus_recursive_mutex->p_mutex_handle));
    vSemaphoreDelete(*(p_bus_extend->p_blocking_semaphore->p_semaphore_handle));

    return FSP_SUCCESS;
}

fsp_err_t rs_gp_i2c_Open(gp_i2c_ctrl_t *p_ctrl, gp_i2c_cfg_t *p_cfg)
{
    FSP_ERROR_RETURN(p_ctrl != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_cfg != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_cfg->p_comms_instance != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(!(p_ctrl->open), FSP_ERR_ALREADY_OPEN);

    const rm_comms_instance_t * p_comms_instance = p_cfg->p_comms_instance;
    rm_comms_i2c_bus_extended_cfg_t * p_bus_extend = (rm_comms_i2c_bus_extended_cfg_t *) p_comms_instance->p_cfg->p_extend;

    FSP_ERROR_RETURN(_gp_i2c_bus_is_open(p_bus_extend), FSP_ERR_COMMS_BUS_NOT_OPEN);

    // Open the comms instance
    fsp_err_t err = RM_COMMS_I2C_Open(p_comms_instance->p_ctrl, p_comms_instance->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_ABORTED);

    _gp_i2c_move_cfg_to_ctrl(p_ctrl, p_cfg);
    p_ctrl->open = true;


    return FSP_SUCCESS;
};

fsp_err_t rs_gp_i2c_Close(gp_i2c_ctrl_t *p_ctrl) {

    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(true == p_ctrl->open, FSP_ERR_NOT_OPEN);

    const rm_comms_instance_t * p_comms_instance = p_ctrl->cfg.p_comms_instance;

    fsp_err_t err = RM_COMMS_I2C_Close(p_comms_instance->p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_ABORTED);

    _gp_i2c_clear_cfg(p_ctrl);
    p_ctrl->open = false;

    return FSP_SUCCESS;

}

fsp_err_t rs_gp_i2c_WriteBytes(gp_i2c_ctrl_t *p_ctrl, uint8_t address, const uint8_t *p_bytes, uint8_t num_bytes)
{
    FSP_ERROR_RETURN(p_ctrl != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_bytes != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(num_bytes > 0, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(num_bytes < GP_I2C_WRITE_BUFFER_SIZE, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(true == p_ctrl->open, FSP_ERR_NOT_OPEN);

    const rm_comms_instance_t * p_comms_instance = p_ctrl->cfg.p_comms_instance;
    rm_comms_i2c_bus_extended_cfg_t * p_bus_extend = (rm_comms_i2c_bus_extended_cfg_t *) p_comms_instance->p_cfg->p_extend;

    FSP_ERROR_RETURN(_gp_i2c_bus_is_open(p_bus_extend), FSP_ERR_COMMS_BUS_NOT_OPEN);

    uint8_t write_buf[GP_I2C_WRITE_BUFFER_SIZE] = {0};

    write_buf[0] = address;
    memcpy(&write_buf[1], p_bytes, num_bytes);

    fsp_err_t err = RM_COMMS_I2C_Write(p_comms_instance->p_ctrl, write_buf, num_bytes + 1);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_ABORTED);

    return FSP_SUCCESS;

}

// Write a single byte value to the desired address.
fsp_err_t rs_gp_i2c_WriteByte(gp_i2c_ctrl_t *p_ctrl, uint8_t address, const uint8_t byte)
{
    return rs_gp_i2c_WriteBytes(p_ctrl, address, &byte, 1);
}

fsp_err_t rs_gp_i2c_ReadBytes(gp_i2c_ctrl_t *p_ctrl, uint8_t address, uint8_t *p_read_bytes, const uint8_t num_bytes) {
    FSP_ERROR_RETURN(p_ctrl != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_read_bytes != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(num_bytes > 0, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(true == p_ctrl->open, FSP_ERR_NOT_OPEN);

    const rm_comms_instance_t * p_comms_instance = p_ctrl->cfg.p_comms_instance;
    rm_comms_i2c_bus_extended_cfg_t * p_bus_extend = (rm_comms_i2c_bus_extended_cfg_t *) p_comms_instance->p_cfg->p_extend;
    FSP_ERROR_RETURN(_gp_i2c_bus_is_open(p_bus_extend), FSP_ERR_COMMS_BUS_NOT_OPEN);

    rm_comms_write_read_params_t write_read_params =
    {
         .p_src = &address,
         .src_bytes = 1,
         .p_dest = p_read_bytes,
         .dest_bytes = num_bytes
    };

    fsp_err_t err = RM_COMMS_I2C_WriteRead(p_comms_instance->p_ctrl, write_read_params);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_ABORTED);

    return FSP_SUCCESS;
}

fsp_err_t rs_gp_i2c_ReadByte(gp_i2c_ctrl_t *p_ctrl, uint8_t address, uint8_t *p_read_byte) {
    return rs_gp_i2c_ReadBytes(p_ctrl, address, p_read_byte, 1);
};

fsp_err_t rs_gp_i2c_CurrentAddressReadBytes(gp_i2c_ctrl_t *p_ctrl, uint8_t *p_read_bytes, const uint8_t num_bytes) {
    FSP_ERROR_RETURN(p_ctrl != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_read_bytes != NULL, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(num_bytes > 0, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(true == p_ctrl->open, FSP_ERR_NOT_OPEN);

    const rm_comms_instance_t * p_comms_instance = p_ctrl->cfg.p_comms_instance;
    rm_comms_i2c_bus_extended_cfg_t * p_bus_extend = (rm_comms_i2c_bus_extended_cfg_t *) p_comms_instance->p_cfg->p_extend;
    FSP_ERROR_RETURN(_gp_i2c_bus_is_open(p_bus_extend), FSP_ERR_COMMS_BUS_NOT_OPEN);


    fsp_err_t err = RM_COMMS_I2C_Read(p_comms_instance->p_ctrl, p_read_bytes, num_bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_ABORTED);

    return FSP_SUCCESS;
}

fsp_err_t rs_gp_i2c_CurrentAddressReadByte(gp_i2c_ctrl_t *p_ctrl, uint8_t *p_read_byte) {
    return rs_gp_i2c_CurrentAddressReadBytes(p_ctrl, p_read_byte, 1);
}
