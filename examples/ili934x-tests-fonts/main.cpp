#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ili934x.h"
#include "FreeMono12pt7b.h"

#define PIN_SCK  2
#define PIN_MOSI 3
#define PIN_MISO 4
//not used
#define PIN_CS   8

#define PIN_DC   7
#define PIN_RST  5

#define SPI_PORT spi0

ILI934X *display = NULL;

uint64_t lastTime1hz;

char text1[] = "This is a test";
char text2[] = "This is other";
int main()
{
    stdio_init_all();

    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_init(PIN_DC);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    display = new ILI934X(SPI_PORT, PIN_CS, PIN_DC, PIN_RST);
    display->reset();
    display->init();
	display->clear();
	display->drawCircle(50, 50, 25, display->colour565(255,255,255));
	int16_t x;
	int16_t y;
	int16_t minx;
	int16_t miny;
	int16_t maxx;
	int16_t maxy;
	GFXfont *font = (GFXfont *)&FreeMono12pt7b;
    while (1)
    {
        
        if (lastTime1hz + 1000000 < time_us_64())
        {
            lastTime1hz = time_us_64();
            
            display->clear();
            //display->drawCircle(50, 50, 25, display->colour565(255,0,0));
			display->fillRect(100,200,150,220,display->colour565(255,255,255));
			char c;
			char *str = text1;
			x = 10;
			y = 100;
			minx = 0;
			miny = 0;
			maxx = 0;
			maxy = 0;
			while ((c = *str++)) {
				// charBounds() modifies x/y to advance for each character,
				// and min/max x/y are updated to incrementally build bounding rect.
				display->charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy, font);
				display->drawChar(x,y,c,display->colour565(255,0,0), font);
			}
			printf("text1 minx=%d maxx=%d miny=%d maxy=%d\n",minx, maxx, miny, maxy);
			str = text2;
			x = 10;
			y += 18;
			minx = 0;
			miny = 0;
			maxx = 0;
			maxy = 0;
			while ((c = *str++)) {
				// charBounds() modifies x/y to advance for each character,
				// and min/max x/y are updated to incrementally build bounding rect.
				display->charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy, font);
				display->drawChar(x,y,c,display->colour565(255,0,0), font);
			}
			printf("text2 minx=%d maxx=%d miny=%d maxy=%d\n",minx, maxx, miny, maxy);
        }
        
    }

    return 0;
}
