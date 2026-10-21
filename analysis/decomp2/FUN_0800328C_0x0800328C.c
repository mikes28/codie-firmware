/* ==== FUN_0800328C @ 0x800328c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800328C(char param_1,int param_2)

{
  int iVar1;
  
  if ('d' < param_1) {
    param_1 = 'd';
  }
  if (param_1 < -100) {
    param_1 = -100;
  }
  iVar1 = (int)param_1;
  if (param_2 << 0x1f < 0) {
    _DAT_200002f4 = FUN_08001248(iVar1);
    if (_DAT_200002b8 == 0) {
      if (iVar1 == 0) {
        FUN_0800A4DC(0x20000340);
        FUN_08008BD4(0,1);
      }
      else {
        FUN_08008BD4(0,1);
      }
    }
    else {
      _DAT_200002f4 = FUN_08000818(_DAT_200002f4,0x40a00000);
    }
  }
  if (param_2 << 0x1e < 0) {
    _DAT_20000320 = FUN_08001248(iVar1);
    if (_DAT_200002b8 == 0) {
      if (iVar1 == 0) {
        FUN_0800A4DC(0x20000348);
        FUN_08008BD4(0,2);
      }
      else {
        FUN_08008BD4(0,2);
      }
    }
    else {
      _DAT_20000320 = FUN_08000818(_DAT_20000320,0x40a00000);
    }
  }
  DAT_200002bc = param_1;
  return;
}


