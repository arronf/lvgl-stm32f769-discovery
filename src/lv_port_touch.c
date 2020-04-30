/**
 * @file indev.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
//#include "hal_stm_lvgl/tft/tft.h"
#include "lvgl/src/lv_hal/lv_hal.h"
//
//#include "stm32f4xx.h"
//#include "../stm32f429i_discovery.h"
//#include "stmpe811.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool touchpad_read(lv_indev_drv_t * drv, lv_indev_data_t *data);
static bool touchpad_get_xy(int16_t *x, int16_t *y);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
TS_StateTypeDef  TS_State = {0};

/**
 * Initialize your input devices here
 */
void touchpad_init(void)
{
//  stmpe811_Init(TS_I2C_ADDRESS);
//  stmpe811_TS_Start(TS_I2C_ADDRESS);
	  uint32_t ts_status = TS_OK;
	  /* Touchscreen initialization */
	  ts_status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.read_cb = touchpad_read;
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Read an input device
 * @param indev_id id of the input device to read
 * @param x put the x coordinate here
 * @param y put the y coordinate here
 * @return true: the device is pressed, false: released
 */
static bool touchpad_read(lv_indev_drv_t * drv, lv_indev_data_t *data)
{
	static int16_t last_x = 0;
	static int16_t last_y = 0;

	bool detected;
	int16_t x;
	int16_t y;
	detected = touchpad_get_xy(&x, &y);
	if(detected) {
		data->point.x = x;
		data->point.y = y;
		last_x = data->point.x;
		last_y = data->point.y;

		data->state = LV_INDEV_STATE_PR;
	} else {
		data->point.x = last_x;
		data->point.y = last_y;
		data->state = LV_INDEV_STATE_REL;
	}

	return false;
}


static bool touchpad_get_xy(int16_t *x, int16_t *y)
{
	  uint32_t ts_status = TS_OK;
      ts_status = BSP_TS_GetState(&TS_State);
      if(ts_status == TS_OK && TS_State.touchDetected)
      {
          *x = TS_State.touchX[0];
          *y = TS_State.touchY[0];
          return true;
      }else
    	  return false;
}
