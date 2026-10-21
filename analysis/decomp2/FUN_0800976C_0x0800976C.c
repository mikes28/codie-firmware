/* ==== FUN_0800976C @ 0x800976c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800976C(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a948(0x2000);
  if (iVar1 != 0) {
    iVar1 = FUN_08009758();
    if (iVar1 == 0) {
      FUN_0800A4DC(0x200008d8);
      DAT_200008ca = DAT_200008ca + '\x01';
      _DAT_4001040c = _DAT_4001040c & 0xffffdfff;
      _DAT_40010408 = _DAT_40010408 | 0x2000;
    }
    else {
      FUN_0800A4DC(0x200008d0);
      DAT_200008c9 = DAT_200008c9 + '\x01';
      _DAT_4001040c = _DAT_4001040c | 0x2000;
      _DAT_40010408 = _DAT_40010408 & 0xffffdfff;
    }
    FUN_0800a958(0x2000);
  }
  return;
}


