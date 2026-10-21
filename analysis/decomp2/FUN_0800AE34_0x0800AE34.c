/* ==== FUN_0800AE34 @ 0x800ae34 ==== */

void FUN_0800AE34(int param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (param_2 & 3) << 2;
  iVar1 = ((param_2 >> 2) + 2) * 4;
  *(uint *)(&DAT_40010000 + iVar1) = *(uint *)(&DAT_40010000 + iVar1) & ~(0xf << iVar2);
  *(uint *)(&DAT_40010000 + iVar1) = param_1 << iVar2 | *(uint *)(&DAT_40010000 + iVar1);
  return;
}


