//#include "Adafruit_ST7735.h"
//Adafruit_ST7735 tft(10, 9, 8);
//#include "Adafruit_ST7789.h"
//Adafruit_ST7789 tft(10, 9, 8);
#include "Adafruit_ST7796S_kbv.h"
Adafruit_ST7796S_kbv tft(10, 9, 8);
//#include "Adafruit_ILI9341.h"
//Adafruit_ILI9341 tft(10, 9, 8);

#include "bitmap_mono.h"
#include "bitmap_RGB.h"
#include "bitmap_gray.h"
#include "xbm_mono.h"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410
#define ORANGE  0xE880

void setup()
{
    Serial.begin(9600);

    Serial.println(F("DEMONSTRATE Adafruit_GFX DRAWING METHODS"));
    Serial.println(F(""));

    Serial.println(F("GFX drawBitmap() plots one mono pixel at a time"));
    Serial.println(F("it defaults to transparent plotting"));
    Serial.println(F("unless you specify foreground and background colours"));
    Serial.println(F("it can plot a monochrome bitmap from Flash or SRAM"));

    Serial.println(F(""));
    Serial.println(F("GFX drawXBitmap() plots an XBM bitmap stored in flash"));
    Serial.println(F("GFX drawGrayscaleBitmap() is not shown"));
    Serial.println(F("GFX drawRGBBitmap() plots one colour pixel at a time"));
    Serial.println(F("from Flash or SRAM and with a monochrome transparent bitmask"));
    Serial.println(F(""));
    Serial.println(F("Adafruit TFT libraries optimise drawRGBBitmap() from SRAM"));
    Serial.println(F("it is worth reading one row at a time from flash"));
    Serial.println(F("look at the user functions that do this"));
    Serial.println(F("they are much faster than the GFX drawxxx() base methods"));
    Serial.println(F(""));

#if 0
#elif defined(_ADAFRUIT_ST7735H_)
    tft.initR(INITR_BLACKTAB);
#elif defined(_ADAFRUIT_ST7789H_)
    tft.init(240, 240);
#else
    tft.begin();
#endif
    tft.setRotation(0);
    tft.fillScreen(BLACK);
    delay(1000);
}

int adv_space(int req, String msg = "")
{
    static int cur_y = 0;
    static uint32_t prev_time;
    int ht = tft.height();
    uint32_t t = millis();
    int y = cur_y + req - 16;
    tft.setCursor(0, y); //  64, 144, 224, 304
    tft.print(msg);
    //    tft.print(" ");
    tft.print(t - prev_time);
    tft.print(F("ms"));
    cur_y += req;
    if (cur_y + req > ht) {
        cur_y = 0;
        delay(5000);
        tft.fillScreen(BLACK);
    }
    prev_time = millis();
    return cur_y;
}

void RGB_2_SRAM(int16_t x, int16_t y, const uint16_t rgb_flash[], int16_t w, int16_t h)
{
    uint16_t sram[w];
    for (int row = 0; row < h; row++) {
        memcpy_P(sram, rgb_flash + row * w, w * 2);
        tft.drawRGBBitmap(x, y + row, sram, w, 1);  //Adafruit is fast from SRAM        
    }
}

void bmap_2_SRAM(int16_t x, int16_t y, const uint8_t bmap_flash[], int16_t w, int16_t h, uint16_t color, uint16_t bg = 0)
{
    uint16_t sram[w];
    uint8_t c, mask, wid = (w + 7) / 8; //bytes per row
    for (int row = 0; row < h; row++) {
        uint8_t *p = bmap_flash + row * wid;
        mask = 0;
        for (int col = 0; col < w; col++, mask >>= 1) {
            if (mask == 0) c = pgm_read_byte(p++), mask = 0x80;
            sram[col] = (c & mask) ? color : bg;
        }
        tft.drawRGBBitmap(x, y + row, sram, w, 1);  //Adafruit is fast from SRAM        
    }
}

void gray_2_SRAM(int16_t x, int16_t y, const uint8_t gray_flash[], int16_t w, int16_t h = 0)
{
    const uint8_t *p = gray_flash;
    uint8_t depth = 4;
    if (h == 0) {  //read geometry from image header instead of function arguments
        depth = pgm_read_word(p + 1);
        w = pgm_read_byte(p + 2);
        h = pgm_read_byte(p + 4);
        p += 6;
    }
    uint16_t sram[w];
    uint8_t sigmask = 0xFF << (8 - depth);
    for (int16_t row = 0; row < h; row++) {
        uint8_t mask = 0, c;
        for (int16_t i = 0; i < w; i++ ) {
            if (mask == 0) {
                c = pgm_read_byte(p++);
                mask = 0xFF;
            }
            uint8_t r = c & sigmask;
            uint16_t color = tft.color565(r, r, r);
            //tft.drawPixel(x + i, y + row, color); //plot each pixel
            sram[i] = color;  //store in buffer
            c <<= depth;
            mask <<= depth;
        }
        tft.drawRGBBitmap(x, y + row, sram, w, 1);  //Adafruit is fast from SRAM        
    }
}

void loop(void)
{
    static int y = 0;
    int x = 5, w = 128, h = 64;
    const int SZ = w * h / 8;
    uint8_t sram[SZ];
    memcpy_P(sram, tractor_128x64, SZ);

    tft.drawBitmap(x, y, tractor_128x64, 128, 64, BLUE);
    y = adv_space(80, F("drawBitmap() transparent flash "));
    tft.drawBitmap(x, y, tractor_128x64, 128, 64, RED, YELLOW);
    y = adv_space(80, F("drawBitmap() background flash "));
    tft.fillRect(x, y, w, h, GREY);
    tft.drawBitmap(x, y, sram, 128, 64, GREEN);
    y = adv_space(80, F("drawBitmap() fill bgd SRAM "));
    tft.drawXBitmap(x, y, truck_128x64_xbm, 128, 64, BLUE);
    y = adv_space(80, F("drawXBitmap() XBM from flash "));

    tft.drawRGBBitmap(x, y, marilyn_64x64, 64, 64);
    y = adv_space(80, F("drawRGBBitmap() flash "));
    w = 64;
    tft.fillRect(x, y, w, h, GREY);
    tft.drawRGBBitmap(x, y, marilyn_64x64, magnify_64x64, 64, 64);
    y = adv_space(80, F("drawRGBBitmap() with mask "));
    w = 64;
    tft.fillRect(x, y, w, h, GREY);
    tft.drawRGBBitmap(x, y, marilyn_64x64, magnify_inv_64x64, 64, 64);
    y = adv_space(80, F("drawRGBBitmap() with mask "));
    bmap_2_SRAM(x, y, tractor_128x64, 128, 64, YELLOW, RED);
    y = adv_space(80, F("copy bmap to RGB SRAM "));
    RGB_2_SRAM(x, y, marilyn_64x64, 64, 64);
    y = adv_space(80, F("copy RGB flash to RGB SRAM "));
    gray_2_SRAM(x, y, gImage_flower, 123, 0);
    y = adv_space(80, F("copy GrayScale to RGB SRAM "));
#if !defined(__AVR_ATmega328P__)
    gray_2_SRAM(x, y, marilyn_grayx4 + 0, 128, 0);
    y = adv_space(144, F("copy GrayScale to RGB SRAM "));
#endif
}
