#include <LiquidCrystalIO.h>
#include "helpers.h"

void defChar(LiquidCrystal &thelcd, int asc, const unsigned char row[8])
{
  int i;

  if ((asc < 0) || (asc > 7))
    return;

  thelcd.command(0x40 | (asc << 3));

  for (i = 0; i < 8; i++)
    thelcd.write(row[i]);

  thelcd.home();
}
