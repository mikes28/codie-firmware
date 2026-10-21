/* ==== FUN_0800A558 @ 0x800a558 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A558(uint *param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  undefined8 uVar4;
  
  uVar2 = _DAT_40001024 + _DAT_20001018 * 0x10000;
  uVar3 = (_DAT_20001018 >> 0x10) + (uint)CARRY4(_DAT_40001024,_DAT_20001018 * 0x10000);
  if (param_1 == (uint *)0x0) {
    param_1 = (uint *)&DAT_20001020;
  }
  if ((uVar3 < param_1[1]) || ((param_1[1] == uVar3 && (uVar2 < *param_1)))) {
    *(undefined4 *)param_2 = 0;
    *(undefined4 *)((int)param_2 + 4) = 0;
    uVar1 = 0;
  }
  else {
    uVar4 = FUN_080002fc(uVar2 - *param_1,(uVar3 - param_1[1]) - (uint)(uVar2 < *param_1),1000,0);
    *param_2 = uVar4;
    uVar1 = 1;
  }
  return uVar1;
}


