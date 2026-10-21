/* ==== FUN_0800AEC4 @ 0x800aec4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AEC4(ushort *param_1,ushort *param_2)

{
  ushort uVar1;
  
  uVar1 = *param_1;
  if (((param_1 == (ushort *)0x40012c00) || (param_1 == (ushort *)0x40000000)) ||
     (param_1 == (ushort *)0x40000400)) {
    uVar1 = uVar1 & 0xff8f | param_2[1];
  }
  if (param_1 == (ushort *)&DAT_40001000) {
    _DAT_4000102c = *(undefined4 *)(param_2 + 2);
    _DAT_40001028 = *param_2;
    _DAT_40001000 = uVar1;
  }
  else {
    *param_1 = uVar1 & 0xfcff | param_2[4];
    *(undefined4 *)(param_1 + 0x16) = *(undefined4 *)(param_2 + 2);
    param_1[0x14] = *param_2;
    if (((param_1 == (ushort *)0x40012c00) || (param_1 == (ushort *)0x40014000)) ||
       ((param_1 == (ushort *)0x40014400 || (param_1 == (ushort *)0x40014800)))) {
      param_1[0x18] = (ushort)(byte)param_2[5];
    }
  }
  param_1[10] = 1;
  return;
}


