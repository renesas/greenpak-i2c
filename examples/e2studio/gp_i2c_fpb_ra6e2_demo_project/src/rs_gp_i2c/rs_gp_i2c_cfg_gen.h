/***********************************************************************************************************************
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#ifndef RS_GP_I2C_RS_GP_I2C_CFG_GEN_H_
#define RS_GP_I2C_RS_GP_I2C_CFG_GEN_H_

#include "rs_gp_i2c.h"

// These functions will manipulate the configuration to match a pre-set value for use in the demo project.

// Puts the size of the config structure into the p_size pointer.
// Returns FSP_SUCCESS when successful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_GetConfigSize (uint32_t * p_size);


// Generates a configuration and places it into p_cfg.
// Returns FSP_SUCCESS when successful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_GenConfig (gp_i2c_cfg_t * const p_cfg, uint32_t const config_size);


// Puts the size of the config structure into the p_size pointer.
// Returns FSP_SUCCESS when successful.
// Returns FSP_ERR_INVALID_ARGUMENT when an argument is malformed.
fsp_err_t rs_gp_i2c_CheckConfig (gp_i2c_cfg_t const * const p_cfg);


#endif /* RS_GP_I2C_RS_GP_I2C_CFG_GEN_H_ */
