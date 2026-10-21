/* ==== FUN_0800AAC8 @ 0x800aac8 ==== */

void FUN_0800AAC8(int param_1,uint param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (param_2 & 7) << 2;
  iVar1 = ((param_2 >> 3) + 8) * 4;
  *(uint *)(iVar1 + param_1) = *(uint *)(iVar1 + param_1) & ~(0xf << iVar2);
  *(uint *)(iVar1 + param_1) = param_3 << iVar2 | *(uint *)(iVar1 + param_1);
  return;
}


