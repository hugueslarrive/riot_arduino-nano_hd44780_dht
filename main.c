/*
 * Copyright (c) 2023 Hugues Larrive <hlarrive@pm.me>
 *
 * SPDX-License-Identifier: CECILL-2.1
 */

#include "dht.h"
#include "dht_params.h"

#include "hd44780.h"
#include "hd44780_params.h"

static hd44780_t dev;
static dht_t dht_dev;
static int16_t temp, hum;

static void lcd_write_string(char *str, uint8_t x, uint8_t y)
{
    hd44780_set_cursor(&dev, x, y);
    hd44780_print(&dev, str);
}


int main(void)
{
    char rh_i[3], t_i[3], rh_d[2], t_d[2];
    
    hd44780_init(&dev, &hd44780_params[0]);
    hd44780_clear(&dev);

    lcd_write_string("RIOT!", 0, 0);
    lcd_write_string("DHT init", 0, 1); 
    dht_init(&dht_dev, &dht_params[0]);

    hd44780_clear(&dev);
    lcd_write_string("   . ", 0, 0);

    /* write ° character */
    hd44780_set_cursor(&dev, 5, 0);
    hd44780_write(&dev, 0xdf);
   
    lcd_write_string("C", 6, 0);
    lcd_write_string("   . %", 0, 1);

    while (1) {
        dht_read(&dht_dev, &temp, &hum);
        if (temp < 0) {
            lcd_write_string("-", 0, 0);
        }
        else lcd_write_string(" ", 0, 0);
        sprintf(rh_i, "%d", hum/10);
        sprintf(rh_d, "%d", hum%10);
        sprintf(t_i, "%d", temp/10);
        sprintf(t_d, "%d", temp%10);
        if (temp / 10 < 10) {
            lcd_write_string("0", 1, 0);
            lcd_write_string(t_i, 2, 0);
        }
        else lcd_write_string(t_i, 1, 0);
        lcd_write_string(t_d, 4, 0);
        if(hum/10 < 10)
        {
            lcd_write_string("0", 1, 1);
            lcd_write_string(rh_i, 2, 1);
        }
        else lcd_write_string(rh_i, 1, 1);
        lcd_write_string(rh_d, 4, 1);
    }

    return 0;
}
