#include <avr/eeprom.h>

typedef struct STRUCT_CONFIG
{
  double PitchP;
  double PitchI;
  double PitchD;
  
  double RollP;
  double RollI;
  double RollD;
  
  double YawP;
  double YawI;
  double YawD;
  
  double ElevationP;
  double ElevationI;
  double ElevationD;

  double BalancePitch;
  double BalanceRoll;
  double BalanceYaw;
  
} StructConfig, *pStructConfig;

#define ADDR_CONFIG 0x30

void WriteRom(StructConfig* p)
{
  if (p) eeprom_write_block((void*)p, (void*)ADDR_CONFIG, sizeof(StructConfig));
}

void ReadRom(StructConfig* p)
{
  if (p)
  {
    eeprom_read_block((void*)p, (void*)ADDR_CONFIG, sizeof(StructConfig));
    for (int i = 0; i < sizeof(StructConfig) / 4; i++)
      if (isnan(*((double*)p + i)))
        *((double*)p + i) = 0;
  }
}
