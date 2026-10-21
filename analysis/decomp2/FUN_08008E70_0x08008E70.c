/* ==== FUN_08008E70 @ 0x8008e70 ==== */

// WARNING: Removing unreachable block (ram,0x08008e82)

undefined4 FUN_08008E70(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  ulonglong uVar2;
  
  uVar2 = FUN_08008E00(param_3);
  if (CONCAT44(param_2,param_1) < uVar2) {
    uVar1 = FUN_08000128(((int)uVar2 - param_1) * 0x30,10);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


