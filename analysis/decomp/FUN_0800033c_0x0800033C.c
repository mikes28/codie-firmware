/* ==== FUN_0800033c @ 0x800033c ==== */

undefined8 FUN_0800033c(uint param_1,int param_2,uint param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  uVar3 = (param_3 & 0xffff) * (param_1 & 0xffff);
  uVar4 = (param_1 >> 0x10) * (param_3 & 0xffff);
  iVar2 = (param_1 >> 0x10) * (param_3 >> 0x10);
  uVar1 = uVar4 + (param_3 >> 0x10) * (param_1 & 0xffff) + (uVar3 >> 0x10);
  if (uVar1 <= uVar4 && uVar4 - uVar1 != 0) {
    iVar2 = iVar2 + 0x10000;
  }
  return CONCAT44(param_4 * param_1 + param_2 * param_3 + iVar2 + (uVar1 >> 0x10),
                  uVar1 * 0x10000 + (uVar3 & 0xffff));
}


