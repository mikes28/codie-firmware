/* ==== FUN_080098C4 @ 0x80098c4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080098C4(void)

{
  int iVar1;
  
  if (-1 < (int)((uint)DAT_200008c8 << 0x1d)) {
    _DAT_48000818 = 0x1000;
    iVar1 = FUN_0800aabc(0x48000800,0x2000);
    if (iVar1 == 0) {
      _DAT_200008e8 = 0;
      DAT_200008ca = 0;
      DAT_200008c9 = 0;
      FUN_080097E8();
    }
    else {
      DAT_200008ca = 0;
      DAT_200008c9 = 0;
      _DAT_4001040c = _DAT_4001040c | 0x2000;
      _DAT_40010408 = _DAT_40010408 & 0xffffdfff;
      FUN_0800A4DC(0x200008e0);
      DAT_200008c8 = DAT_200008c8 | 4;
    }
  }
  return;
}


