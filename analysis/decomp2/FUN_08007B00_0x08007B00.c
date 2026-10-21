/* ==== FUN_08007B00 @ 0x8007b00 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007B00(void)

{
  undefined4 in_r3;
  
  if ((DAT_200004f8 & 6) == 2) {
    FUN_0800B244(0x40014000,0,1,&DAT_200004f8,in_r3);
    _DAT_20000528 = 0;
    _DAT_2000052c = 0;
    DAT_200004f8 = DAT_200004f8 | 4;
  }
  return;
}


