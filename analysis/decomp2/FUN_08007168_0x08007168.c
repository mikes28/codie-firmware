/* ==== FUN_08007168 @ 0x8007168 ==== */

void FUN_08007168(void)

{
  int iVar1;
  
  if (-1 < (int)((uint)DAT_200004b0 << 0x1d)) {
    FUN_0800A704(0x40012400);
    FUN_0800A6DC(0x40012400,1);
    do {
      iVar1 = FUN_0800A78C(0x40012400,1);
    } while (iVar1 == 0);
    FUN_0800A87C(0x4002001c,1);
    FUN_0800A760(0x40012400,1);
    FUN_0800a754(0x40012400);
    DAT_200004b0 = DAT_200004b0 | 4;
  }
  return;
}


