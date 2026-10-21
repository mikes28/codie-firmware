/* ==== FUN_0800A16C @ 0x800a16c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A16C(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  code *pcVar1;
  
  pcVar1 = _DAT_20000a10;
  FUN_0800a160();
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(param_2);
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(param_3);
  if (param_4 != 0) {
    (*_DAT_20000a40)(0x20);
    (*_DAT_20000a10)(param_4);
  }
  (*_DAT_20000a40)(10);
  return 1;
}


