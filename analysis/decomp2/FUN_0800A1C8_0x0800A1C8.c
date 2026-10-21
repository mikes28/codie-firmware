/* ==== FUN_0800A1C8 @ 0x800a1c8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A1C8(int *param_1,int param_2)

{
  code *pcVar1;
  uint uVar2;
  int *piVar3;
  
  pcVar1 = _DAT_20000a10;
  FUN_0800a160(3);
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(":proxy");
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)("greeting");
  if (param_1 != (int *)0x0) {
    (*_DAT_20000a40)(0x20);
    uVar2 = 0;
    piVar3 = param_1;
    if (*param_1 != 0) {
      while( true ) {
        (*_DAT_20000a40)(0x22);
        (*_DAT_20000a10)(*piVar3);
        (*_DAT_20000a40)(0x3a);
        if (param_1[uVar2 + 1] == 0) break;
        uVar2 = uVar2 + 2 & 0xff;
        (*_DAT_20000a10)();
        (*_DAT_20000a40)(0x22);
        piVar3 = param_1 + uVar2;
        if (*piVar3 == 0) goto LAB_0800a270;
        if (uVar2 != 0) {
          (*_DAT_20000a40)(0x20);
        }
      }
      (*_DAT_20000a40)(0x22);
    }
  }
LAB_0800a270:
  if (param_2 != 0) {
    (*_DAT_20000a40)(0x20);
    (*_DAT_20000a40)(0x22);
    (*_DAT_20000a10)(&DAT_0800c6b8);
    (*_DAT_20000a10)(param_2);
    (*_DAT_20000a40)(0x22);
  }
  (*_DAT_20000a40)(10);
  return;
}


