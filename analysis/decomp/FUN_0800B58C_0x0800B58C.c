/* ==== FUN_0800B58C @ 0x800b58c ==== */

// WARNING: Removing unreachable block (ram,0x0800b5c0)
// WARNING: Removing unreachable block (ram,0x0800b5cc)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800B58C(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  
  if (_DAT_20000c20 == 0) {
    _DAT_20000c20 = 0x20000c24;
  }
  iVar1 = _DAT_20000c20;
  if (*(int *)(_DAT_20000c20 + 4) < 0x20) {
    uVar4 = *(uint *)(_DAT_20000c20 + 4);
    if (param_1 != 0) {
      *(undefined4 *)(_DAT_20000c20 + uVar4 * 4 + 0x88) = param_3;
      uVar3 = 1 << (uVar4 & 0xff);
      *(uint *)(iVar1 + 0x188) = *(uint *)(iVar1 + 0x188) | uVar3;
      *(undefined4 *)(iVar1 + (uVar4 + 0x20) * 4 + 0x88) = param_4;
      if (param_1 == 2) {
        *(uint *)(iVar1 + 0x18c) = uVar3 | *(uint *)(iVar1 + 0x18c);
      }
    }
    *(undefined4 *)((uVar4 + 2) * 4 + iVar1) = param_2;
    *(uint *)(iVar1 + 4) = uVar4 + 1;
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}


