/* ==== FUN_08006CA0 @ 0x8006ca0 ==== */

undefined4 FUN_08006CA0(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_08007980();
  uVar2 = 1;
  if (iVar1 == 0) {
    if (DAT_200004ab == '\x01') {
      FUN_08006C80(1);
      DAT_200004ab = '\x02';
      uVar2 = 1;
    }
    else {
      uVar2 = 1;
      if (DAT_200004ab == '\x02') {
        DAT_200004ab = '\0';
        uVar2 = 0;
      }
    }
  }
  return uVar2;
}


