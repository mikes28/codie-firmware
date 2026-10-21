/* ==== FUN_08009444 @ 0x8009444 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int FUN_08009444(void)

{
  int iVar1;
  int iVar2;
  undefined4 in_r3;
  int iVar3;
  
  iVar3 = (int)_DAT_20000872;
  iVar1 = FUN_080071DC(0x20,0,0,in_r3,in_r3);
  iVar2 = FUN_080071DC(0x40,0,0);
  iVar1 = (iVar3 - iVar1) + (iVar2 - _DAT_20000874);
  return (int)(short)((uint)(iVar1 - (iVar1 >> 0x1f)) >> 1);
}


