/* ==== FUN_08001FD4 @ 0x8001fd4 ==== */

undefined8 FUN_08001FD4(uint param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  if (param_1 == 0) {
    uVar2 = 0;
    uVar3 = 0;
    iVar4 = 0;
  }
  else {
    iVar1 = FUN_0800216c();
    if (iVar1 < 0xb) {
      iVar4 = param_1 << (iVar1 + 0x15U & 0xff);
      uVar3 = param_1 >> (0xbU - iVar1 & 0xff) & 0xfffff;
      uVar2 = 0x41eU - iVar1 & 0x7ff;
    }
    else {
      uVar2 = 0x41eU - iVar1 & 0x7ff;
      uVar3 = param_1 << (iVar1 - 0xbU & 0xff) & 0xfffff;
      iVar4 = 0;
    }
  }
  return CONCAT44(uVar3 | uVar2 << 0x14,iVar4);
}


