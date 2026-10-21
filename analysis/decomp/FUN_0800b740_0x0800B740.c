/* ==== FUN_0800b740 @ 0x800b740 ==== */

int FUN_0800b740(byte *param_1,byte *param_2)

{
  byte bVar1;
  uint uVar2;
  
  do {
    uVar2 = (uint)*param_1;
    bVar1 = *param_2;
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
    if (uVar2 == 0) break;
  } while (uVar2 == bVar1);
  return uVar2 - bVar1;
}


