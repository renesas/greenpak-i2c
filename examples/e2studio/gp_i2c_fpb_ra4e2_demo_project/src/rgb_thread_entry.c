/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include <rgb_thread.h>
#include "rm_comms_api.h"

#include "rs_gp_i2c/rs_gp_i2c.h"

#if GP_I2C_PRESET_CONFIG_USE
#include "rs_gp_i2c/rs_gp_i2c_cfg_gen.h"
#endif

#include <stdio.h>

#define RED_ADDRESS 0xAA
#define GREEN_ADDRESS 0xAF
#define BLUE_ADDRESS 0xB3


gp_i2c_ctrl_t gp_ctrl;
gp_i2c_cfg_t gp_cfg;

gp_i2c_ctrl_t *p_gp_ctrl = &gp_ctrl;
gp_i2c_cfg_t *p_gp_cfg = &gp_cfg;

const uint8_t rainbow[] = {
    0xFF, 0x00, 0x00,
    0xFF, 0x70, 0x10,
    0xFF, 0xAF, 0x00,
    0x00, 0xFF, 0x00,
    0x00, 0xFF, 0xFF,
    0x00, 0x00, 0xFF,
    0xFF, 0x00, 0xFF,
};


static fsp_err_t SLG46826_SetRGB(uint8_t r, uint8_t g, uint8_t b)
{
    fsp_err_t err = rs_gp_i2c_OpenI2CBus(p_gp_cfg);
    if (FSP_SUCCESS != err && FSP_ERR_ALREADY_OPEN != err)
    {
        return err;
    }

    err = rs_gp_i2c_Open(p_gp_ctrl, p_gp_cfg);
    if (FSP_SUCCESS != err && FSP_ERR_ALREADY_OPEN != err)
    {
        return err;
    }

    err = rs_gp_i2c_WriteByte(p_gp_ctrl, RED_ADDRESS, r);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    err = rs_gp_i2c_WriteByte(p_gp_ctrl, GREEN_ADDRESS, g);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    err = rs_gp_i2c_WriteByte(p_gp_ctrl, BLUE_ADDRESS, b);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);


    return FSP_SUCCESS;
}

/* RGB LED Thread entry function */
void rgb_thread_entry (void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

#if GP_I2C_PRESET_CONFIG_USE
    rs_gp_i2c_GenConfig(p_gp_cfg, sizeof(gp_i2c_cfg_t));
#else
    gp_cfg.p_comms_instance = &g_comms_i2c_device0;
#endif

    fsp_err_t err = SLG46826_SetRGB(0, 0, 0);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    uint8_t current = 0;
    size_t num_colors = (sizeof(rainbow) / sizeof(uint8_t)) / 3;
    while (1) {

        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

        uint8_t r = rainbow[current * 3];
        uint8_t g = rainbow[(current * 3) + 1];
        uint8_t b = rainbow[(current * 3) + 2];

        err = SLG46826_SetRGB(r, g, b);
        if (FSP_SUCCESS != err)
        {
            __BKPT(0);
        }

        printf("The LED color has been changed. ");

        rs_gp_i2c_ReadByte(p_gp_ctrl, RED_ADDRESS, &r);
        rs_gp_i2c_ReadByte(p_gp_ctrl, GREEN_ADDRESS, &g);
        rs_gp_i2c_ReadByte(p_gp_ctrl, BLUE_ADDRESS, &b);

        printf("New Value: %02x %02x %02x\r\n", r, g, b);

        current = (current + 1) % (num_colors);
    }
}
