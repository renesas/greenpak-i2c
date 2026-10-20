/***********************************************************************************************************************
 * Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ***********************************************************************************************************************/

#include "bsp_api.h"
#include "rs_gpio_debounce.h"
#include "r_ioport_api.h"

#define IDLE    0

/*****************************************************************************
 * Internal functions                                                        *
 *****************************************************************************/

/* Helper function to reset button callback argument structure */
static void gpio_reset_callback_memory (gpio_debounce_callback_args_t * p_callback_memory)
{
    p_callback_memory->p_context = NULL;
    p_callback_memory->event     = GPIO_DEBOUNCE_EVENT_NONE;
    p_callback_memory->portpin   = 0;
}

/* Helper function to move elements from a gpio_debounce_cfg_t to a gpio_debounce_ctrl_t */
static void gpio_move_cfg_to_ctrl (gpio_debounce_ctrl_t * p_ctrl, const gpio_debounce_cfg_t * const p_cfg)
{
    gpio_debounce_cfg_t * p_dest_cfg = &p_ctrl->cfg;
    p_dest_cfg->portpin           = p_cfg->portpin;
    p_dest_cfg->active_level      = p_cfg->active_level;
    p_dest_cfg->event_enable_mask = p_cfg->event_enable_mask;
    p_dest_cfg->count_to_active   = p_cfg->count_to_active;
    p_dest_cfg->count_to_inactive = p_cfg->count_to_inactive;
    p_dest_cfg->p_ioport          = p_cfg->p_ioport;
    p_dest_cfg->p_callback        = p_cfg->p_callback;
    p_dest_cfg->p_context         = p_cfg->p_context;
}

/* Saturating counter increment for update function.                       */
/* Increment num unless it is greater than or equal to the threshold value */
static uint32_t sat_incr (uint32_t num, uint32_t threshold)
{
    if (num < threshold)
    {
        num += 1;
    }

    return num;
}

/* Saturating counter decrement for update function.                    */
/* Decrement num unless it is less than or equal to the threshold value */
static uint32_t sat_decr (uint32_t num, uint32_t threshold)
{
    if (num > threshold)
    {
        num -= 1;
    }

    return num;
}

/**********************************************************************************************************************
 * @addtogroup BUTTON
 * @{
 **********************************************************************************************************************/

/*****************************************************************************************************************
 * Open and configure the button for operation.
 *
 * @param[in]  p_ctrl  Pointer to control block structure.
 * @param[in]  p_cfg   Pointer to configuration structure.
 *
 * @retval FSP_SUCCESS                   Button instance opened successfully
 * @retval FSP_ERR_ALREADY_OPEN          Button already open.
 * @retval FSP_ERR_ASSERTION             Null pointer presented.
 * @retval FSP_ERR_INVALID_ARGUMENT      Configuration parameter is invalid.
 *****************************************************************************************************************/
fsp_err_t rs_gpio_debounce_Open (gpio_debounce_ctrl_t * const p_ctrl, gpio_debounce_cfg_t const * const p_cfg)
{
    FSP_ASSERT(p_ctrl != NULL)
    FSP_ASSERT(p_cfg != NULL)
    FSP_ASSERT(p_cfg->p_ioport != NULL)

    FSP_ERROR_RETURN(p_ctrl->open == false, FSP_ERR_ALREADY_OPEN)
    FSP_ERROR_RETURN(p_cfg->count_to_active != 0, FSP_ERR_INVALID_ARGUMENT)
    FSP_ERROR_RETURN(p_cfg->count_to_inactive != 0, FSP_ERR_INVALID_ARGUMENT)

    p_ctrl->open    = true;
    p_ctrl->counter = IDLE;
    p_ctrl->level   = GPIO_DEBOUNCE_LEVEL_LOW;

    gpio_move_cfg_to_ctrl(p_ctrl, p_cfg);
    gpio_reset_callback_memory(&p_ctrl->callback_memory);

    return FSP_SUCCESS;
}

/*****************************************************************************************************************
 * Close the button.
 *
 * @param[in]  p_ctrl  Pointer to control block structure.
 *
 * @retval FSP_SUCCESS                   Button instance closed successfully.
 * @retval FSP_ERR_NOT_OPEN              Button is not open.
 * @retval FSP_ERR_ASSERTION             Null pointer presented.
 *****************************************************************************************************************/
fsp_err_t rs_gpio_debounce_Close (gpio_debounce_ctrl_t * const p_ctrl)
{
    FSP_ASSERT(p_ctrl != NULL)
    FSP_ERROR_RETURN(p_ctrl->open == true, FSP_ERR_NOT_OPEN)

    gpio_debounce_cfg_t * p_cfg = &p_ctrl->cfg;

    p_ctrl->open    = false;
    p_ctrl->counter = IDLE;
    p_ctrl->level   = 0;

    p_cfg->portpin           = 0;
    p_cfg->active_level      = 0;
    p_cfg->event_enable_mask = GPIO_DEBOUNCE_EVENT_NONE;
    p_cfg->count_to_active   = 0;
    p_cfg->count_to_inactive = 0;
    p_cfg->p_ioport          = NULL;
    p_cfg->p_callback        = NULL;
    p_cfg->p_context         = NULL;

    gpio_reset_callback_memory(&p_ctrl->callback_memory);

    return FSP_SUCCESS;
}

/*****************************************************************************************************************
 * Get the logic level of a button instance.
 *
 * @param[in]  p_ctrl  Pointer to control block structure.
 * @param[out] p_level Pointer to a button_level_t.
 *
 * @retval FSP_SUCCESS                   Button level retrieved successfully.
 * @retval FSP_ERR_NOT_OPEN              Button already open.
 * @retval FSP_ERR_ASSERTION             Null pointer presented.
 *****************************************************************************************************************/
fsp_err_t rs_gpio_debounce_GetLevel (gpio_debounce_ctrl_t * const p_ctrl, gpio_debounce_level_t * const p_level)
{
    FSP_ASSERT(p_ctrl != NULL)
    FSP_ASSERT(p_level != NULL)
    FSP_ERROR_RETURN(p_ctrl->open == true, FSP_ERR_NOT_OPEN)

    * p_level = p_ctrl->level;

    return FSP_SUCCESS;
}

/*****************************************************************************************************************
 * Set a callback function for the button instance that is called when a change to the button level occurs.
 *
 * @param[in]  p_ctrl     Pointer to control block structure.
 * @param[out] p_callback Pointer to a callback function.
 * @param[out] p_context  Pointer to user data that is passed to the callback function.
 *
 * @retval FSP_SUCCESS                   Callback set successfully.
 * @retval FSP_ERR_NOT_OPEN              Button already open.
 * @retval FSP_ERR_ASSERTION             Null pointer presented.
 *****************************************************************************************************************/
fsp_err_t rs_gpio_debounce_SetCallback (gpio_debounce_ctrl_t * const p_ctrl,
                                        void (                      *p_callback ) (
                                            gpio_debounce_callback_args_t const * const p_args),
                                        void * const                 p_context)
{
    FSP_ASSERT(p_ctrl != NULL)
    FSP_ERROR_RETURN(p_ctrl->open == true, FSP_ERR_NOT_OPEN)

    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;
    p_ctrl->cfg.p_callback = p_callback;
    p_ctrl->cfg.p_context  = p_context;
    FSP_CRITICAL_SECTION_EXIT;

    return FSP_SUCCESS;
}

/*****************************************************************************************************************
 * Trigger an update cycle on a button instance.
 *
 * This function works by reading an IOPORT instance and incrementing or decrementing a saturating counter
 * inside the control block depending on the result.
 *
 * When the counter reaches the maximum value, the button instance's level will change to high.
 * When the counter reaches zero, the button instance's level will change to low.
 *
 * This function will also call a user-defined callback when the level of the button changes.
 *
 * To properly debounce a GPIO input, this function should be called at a fixed interval.
 *
 * @param[in]  p_ctrl  Pointer to control block structure.
 *
 * @retval FSP_SUCCESS                   Button instance opened successfully
 * @retval FSP_ERR_NOT_OPEN              Button already open.
 * @retval FSP_ERR_ASSERTION             Null pointer presented.
 *****************************************************************************************************************/
fsp_err_t rs_gpio_debounce_Update (gpio_debounce_ctrl_t * const p_ctrl)
{
    FSP_ASSERT(p_ctrl != NULL)
    FSP_ERROR_RETURN(p_ctrl->open == true, FSP_ERR_NOT_OPEN)

    gpio_debounce_cfg_t * p_cfg = &p_ctrl->cfg;

    const ioport_instance_t * p_ioport      = p_cfg->p_ioport;
    bsp_io_level_t            pin_level     = BSP_IO_LEVEL_LOW;
    gpio_debounce_event_t     active_events = GPIO_DEBOUNCE_EVENT_NONE;

    /* Determine what the maximum counter value should be. */
    /* If the current debounced level is high, counter should cap at count_to_inactive. */
    /* Otherwise, counter should cap at count_to_active */
    uint32_t counter_max;
    if (p_ctrl->level == GPIO_DEBOUNCE_LEVEL_HIGH)
    {
        counter_max = p_cfg->count_to_inactive;
    }
    else
    {
        counter_max = p_cfg->count_to_active;
    }

    /* Read the level of the debounce instance's portpin */
    fsp_err_t err = p_ioport->p_api->pinRead(p_ioport->p_ctrl, p_cfg->portpin, &pin_level);
    FSP_ERROR_RETURN(err == FSP_SUCCESS, FSP_ERR_ABORTED)

    /* Use the IOPORT pin level to increment or decrement the counter. */
    if (pin_level == p_cfg->active_level)
    {
        p_ctrl->counter = sat_incr(p_ctrl->counter, counter_max);
    }
    else
    {
        p_ctrl->counter = sat_decr(p_ctrl->counter, 0);
    }

    /* Check debounced level and counter value, and change debounced level accordingly. */
    /* Additionally, set active events flag for use in callback. */
    if ((p_ctrl->counter == counter_max) && (p_ctrl->level == GPIO_DEBOUNCE_LEVEL_LOW))
    {
        p_ctrl->level  = GPIO_DEBOUNCE_LEVEL_HIGH;
        active_events |= GPIO_DEBOUNCE_EVENT_PRESSED;

        /* If the state transitions to high, count_to_inactive should become the max counter value. */
        p_ctrl->counter = p_cfg->count_to_inactive;
    }
    else if ((p_ctrl->counter == 0) && (p_ctrl->level == GPIO_DEBOUNCE_LEVEL_HIGH))
    {
        p_ctrl->level  = GPIO_DEBOUNCE_LEVEL_LOW;
        active_events |= GPIO_DEBOUNCE_EVENT_RELEASED;
    }

    /* Call user-provided callback function if an event has occurred */
    if ((p_cfg->p_callback != NULL) && ((active_events & p_cfg->event_enable_mask) != 0))
    {
        p_ctrl->callback_memory.event     = active_events;
        p_ctrl->callback_memory.portpin   = p_cfg->portpin;
        p_ctrl->callback_memory.p_context = p_cfg->p_context;
        p_cfg->p_callback(&p_ctrl->callback_memory);
    }

    return FSP_SUCCESS;
}

/*****************************************************************************************************************
 * Check whether a button instance is open.
 *
 * @param[in]  p_ctrl  Pointer to control block structure.
 * @param[out] result  Pointer to an integer.
 *
 * @retval FSP_SUCCESS                   Button instance opened successfully
 * @retval FSP_ERR_NOT_OPEN              Button already open.
 * @retval FSP_ERR_ASSERTION             Null pointer presented.
 *****************************************************************************************************************/
fsp_err_t rs_gpio_debounce_IsOpen (gpio_debounce_ctrl_t * const p_ctrl, bool * result)
{
    FSP_ASSERT(p_ctrl != NULL)
    FSP_ASSERT(result != NULL)

    * result = p_ctrl->open;

    return FSP_SUCCESS;
}

/** @} (end addtogroup BUTTON) */
