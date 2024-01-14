#include "screen.h"

screen::screen()
{
  tft.begin();
  backgroundColor = COLOR_BLACK;
  tft.setBackgroundColor(backgroundColor);
}
void screen::clean()
{
  tft.clear();
}
void screen::cleanArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  tft.fillRectangle(x, y, x + w - 1, y + h - 1, COLOR_BLACK);
}
void screen::drawColorPixelmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap)
{
  tft.drawColorPixelmap(x, y, bitmap, w, h, true);
}
void screen::showLines(struct lines *lines, uint16_t num)
{
  int i = 0;
  for (i = 0; i < num; i++)
  {
    tft.drawLine(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2, lines[i].color);
  }
}
void screen::printText(uint16_t x, uint16_t y, STRING buf, uint8_t* font, uint16_t bg, uint16_t ft)
{
  tft.setFont(font);
  tft.setBackgroundColor(backgroundColor);
  tft.drawText(x, y, buf, ft);
}