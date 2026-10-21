/* ==== FUN_0800A2D0 @ 0x800a2d0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A2D0(undefined4 param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  
  pcVar1 = _DAT_20000a10;
  FUN_0800a160(3);
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(":proxy");
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)("message");
  (*_DAT_20000a40)(0x20);
  pcVar1 = _DAT_20000a10;
  FUN_0800a2c0(param_1);
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(param_2);
  if (param_3 != 0) {
    (*_DAT_20000a10)(param_3);
  }
  (*_DAT_20000a40)(10);
  return;
}


