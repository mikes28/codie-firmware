/* ==== FUN_0800AB30 @ 0x800ab30 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AB30(byte *param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1[2] == 0) {
    _DAT_e000e180 = 1 << (*param_1 & 0x1f);
  }
  else {
    iVar2 = ((*param_1 >> 2) + 0xc0) * 4;
    iVar1 = (*param_1 & 3) << 3;
    *(uint *)(&DAT_e000e100 + iVar2) =
         *(uint *)(&DAT_e000e100 + iVar2) & ~(0xff << iVar1) | ((param_1[1] & 3) << 6) << iVar1;
    _DAT_e000e100 = 1 << (*param_1 & 0x1f);
  }
  return;
}


