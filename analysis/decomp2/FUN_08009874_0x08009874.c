/* ==== FUN_08009874 @ 0x8009874 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009874(void)

{
  do {
    FUN_0800A558(0x200008e0,&DAT_20000fe0);
    if (_DAT_20000fe4 != 0) break;
  } while (_DAT_20000fe0 < 0x191);
  if (DAT_200008ca != '\0') {
    _DAT_200008e8 = 0;
    DAT_200008ca = 0;
    DAT_200008c9 = 0;
    FUN_080097E8(0);
  }
  _DAT_200008e8 = 1;
  DAT_200008ca = 0;
  DAT_200008c9 = 0;
  return;
}


