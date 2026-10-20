/***********************************************************************************************************************
 * Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ***********************************************************************************************************************/

#include "rs_gp_i2c_cfg_gen.h"
#include "bsp_api.h"
#include "rs_gp_i2c.h"
#include <string.h>

extern const gp_i2c_cfg_t g_gp_i2c_cfg_ref;

fsp_err_t rs_gp_i2c_GetConfigSize (uint32_t * p_size)
{
    FSP_ASSERT(p_size != NULL);
    *p_size = sizeof(gp_i2c_cfg_t);

    return FSP_SUCCESS;
}

fsp_err_t rs_gp_i2c_GenConfig (gp_i2c_cfg_t * const p_cfg, uint32_t const config_size)
{
    FSP_ERROR_RETURN(p_cfg != NULL, FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN(sizeof(gp_i2c_cfg_t) == config_size, FSP_ERR_INVALID_ARGUMENT);

    p_cfg->p_comms_instance = g_gp_i2c_cfg_ref.p_comms_instance;

    return FSP_SUCCESS;
}

fsp_err_t rs_gp_i2c_CheckConfig (gp_i2c_cfg_t const * const p_cfg)
{
    FSP_ERROR_RETURN(p_cfg != NULL, FSP_ERR_INVALID_POINTER);

    FSP_ERROR_RETURN(p_cfg->p_comms_instance == g_gp_i2c_cfg_ref.p_comms_instance, FSP_ERR_INVALID_DATA);
    return FSP_SUCCESS;
}
