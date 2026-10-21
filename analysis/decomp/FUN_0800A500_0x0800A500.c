/* ==== FUN_0800A500 @ 0x800a500 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A500(uint *param_1,int *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = _DAT_40001024 + _DAT_20001018 * 0x10000;
  uVar5 = (_DAT_20001018 >> 0x10) + (uint)CARRY4(_DAT_40001024,_DAT_20001018 * 0x10000);
  if (param_1 == (uint *)0x0) {
    param_1 = (uint *)&DAT_20001020;
  }
  if ((uVar5 < param_1[1]) || ((param_1[1] == uVar5 && (uVar4 < *param_1)))) {
    *param_2 = 0;
    param_2[1] = 0;
    uVar1 = 0;
  }
  else {
    uVar3 = param_1[1];
    uVar2 = *param_1;
    *param_2 = uVar4 - uVar2;
    param_2[1] = (uVar5 - uVar3) - (uint)(uVar4 < uVar2);
    uVar1 = 1;
  }
  return uVar1;
}


