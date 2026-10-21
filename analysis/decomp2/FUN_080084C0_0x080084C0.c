/* ==== FUN_080084C0 @ 0x80084c0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080084C0(undefined2 param_1,undefined2 param_2,undefined1 param_3,int param_4)

{
  int iVar1;
  uint local_20;
  int local_1c;
  
  if (_DAT_200006e2 != 10) {
    iVar1 = (uint)_DAT_200006e2 * 6;
    *(undefined2 *)(iVar1 + 0x20000650) = param_1;
    *(undefined2 *)(iVar1 + 0x20000652) = param_2;
    *(undefined1 *)(iVar1 + 0x20000654) = param_3;
    FUN_0800A4DC(&local_20);
    iVar1 = (_DAT_200006e2 + 9) * 8;
    *(uint *)(&DAT_20000648 + iVar1) = param_4 * 1000 + local_20;
    *(uint *)(iVar1 + 0x2000064c) = local_1c + (uint)CARRY4(param_4 * 1000,local_20);
    _DAT_200006e2 = _DAT_200006e2 + 1;
  }
  return;
}


