/* ==== FUN_08005F94 @ 0x8005f94 ==== */

bool FUN_08005F94(undefined2 param_1)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_2000037e;
  if (uVar1 < 3) {
    *(undefined2 *)(&DAT_20000358 + (uVar1 + 0x10) * 2) = param_1;
    DAT_2000037e = DAT_2000037e + 1;
  }
  return uVar1 < 3;
}


