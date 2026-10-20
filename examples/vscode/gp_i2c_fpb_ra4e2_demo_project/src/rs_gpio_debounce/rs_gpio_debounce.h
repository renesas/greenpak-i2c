/***********************************************************************************************************************
 * Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ***********************************************************************************************************************/

#ifndef RS_GPIO_DEBOUNCE_H_
#define RS_GPIO_DEBOUNCE_H_
#include "bsp_api.h"
#include "r_ioport_api.h"

FSP_HEADER

/**********************************************************************************************************************
 * @defgroup BUTTON Button Instance
 * @addtogroup BUTTON
 * @{
 **********************************************************************************************************************/

/**
 * @enum gpio_debounce_event_t
 * @brief Represents an event that fires when the button state changes.
 */
typedef enum
{
    GPIO_DEBOUNCE_EVENT_NONE,                  /**< Default event state. */
    GPIO_DEBOUNCE_EVENT_RELEASED = (1UL << 0), /**< Button was released. */
    GPIO_DEBOUNCE_EVENT_PRESSED  = (1UL << 1), /**< Button was pressed. */
} gpio_debounce_event_t;

/**
 * @enum gpio_debounce_level_t
 * @brief Represents a button level.
 */
typedef enum
{
    GPIO_DEBOUNCE_LEVEL_INVALID = -1,  /**< Button level is invalid. */
    GPIO_DEBOUNCE_LEVEL_LOW     = 0,   /**< Button level is low. */
    GPIO_DEBOUNCE_LEVEL_HIGH    = 1,   /**< Button level is high. */
} gpio_debounce_level_t;

/**
 * @struct gpio_debounce_callback_args_t
 * @brief Arguments passed to the user callback function.
 */
typedef struct st_gpio_debounce_callback_args
{
    /// Port and pin number.
    bsp_io_port_pin_t portpin;

    /// Event that triggered the callback.
    gpio_debounce_event_t event;

    /// Pointer that can point to a user-controlled region in memory for use in the callback function.
    void * p_context;
} gpio_debounce_callback_args_t;

/**
 * @struct gpio_debounce_cfg_t
 * @brief Configuration parameters for a button.
 */
typedef struct st_gpio_debounce_cfg
{
    bsp_io_port_pin_t         portpin;                                      /**< Port and pin number. */
    bsp_io_level_t            active_level;                                 /**< Pin level considered to be active. */
    gpio_debounce_event_t     event_enable_mask;                            /**< Enabled event notifications. */
    uint32_t                  count_to_active;                              /**< Debounce ticks to active state. */
    uint32_t                  count_to_inactive;                            /**< Debounce ticks to inactive state. */
    const ioport_instance_t * p_ioport;                                     /**< IOPORT instance. */
    void (*p_callback)(gpio_debounce_callback_args_t const * const p_args); /**< User callback function. */
    void * p_context;                                                       /**< Pointer that can point to a user-controlled region in memory for use in the callback function. */
} gpio_debounce_cfg_t;

/** Button instance control block. */
typedef struct st_gpio_debounce_ctrl
{
    bool     open;                                 /**< Flag indicating if the instance is open. */
    uint32_t counter;                              /**< Internal counter for debounce timing. */

    // TODO: change to level
    gpio_debounce_level_t level;                   /**< Current button state. */

    gpio_debounce_callback_args_t callback_memory; /**< Memory for callback function parameters. **/

    gpio_debounce_cfg_t cfg;                       /**< Internal copy of button configuration. **/
} gpio_debounce_ctrl_t;

/**
 * @struct gpio_debounce_instance_t
 * @brief Instance structure for a button.
 */
typedef struct st_gpio_debounce_instance
{
    gpio_debounce_ctrl_t      * p_ctrl; /**< Pointer to control block. */
    gpio_debounce_cfg_t const * p_cfg;  /**< Pointer to configuration. */
} gpio_debounce_instance_t;

fsp_err_t rs_gpio_debounce_Open(gpio_debounce_ctrl_t * const p_ctrl, gpio_debounce_cfg_t const * const p_cfg);
fsp_err_t rs_gpio_debounce_Close(gpio_debounce_ctrl_t * const p_ctrl);
fsp_err_t rs_gpio_debounce_GetLevel(gpio_debounce_ctrl_t * const p_ctrl, gpio_debounce_level_t * const p_level);
fsp_err_t rs_gpio_debounce_SetCallback(gpio_debounce_ctrl_t * const p_ctrl,
                                       void (                     * p_callback ) (
                                           gpio_debounce_callback_args_t const * const p_args),
                                       void * const                 p_context);
fsp_err_t rs_gpio_debounce_Update(gpio_debounce_ctrl_t * const p_ctrl);
fsp_err_t rs_gpio_debounce_IsOpen(gpio_debounce_ctrl_t * const p_ctrl, bool * result);

FSP_FOOTER

#endif

/**********************************************************************************************************************
 * @} (end BUTTON)
 **********************************************************************************************************************/
