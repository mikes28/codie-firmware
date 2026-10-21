/* ==== FUN_08002A60 @ 0x8002a60 ==== */

undefined4 FUN_08002A60(uint param_1)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = param_1 >> 0x10 & 0xff;
  uVar2 = param_1 >> 0x18;
  if ((param_1 & 0xff) < 0x80) {
    if (uVar3 < 10) {
      if (uVar2 < *(byte *)(uVar3 * 8 + 0x20000198)) {
        uVar1 = *(undefined4 *)(uVar2 * 4 + *(int *)(uVar3 * 8 + 0x20000194));
      }
      else {
        uVar1 = 0;
      }
    }
    else {
      uVar1 = 0;
    }
  }
  else if (uVar3 < 10) {
    if (uVar2 < *(byte *)(uVar3 * 8 + 0x20000118)) {
      uVar1 = *(undefined4 *)(uVar2 * 4 + *(int *)(uVar3 * 8 + 0x20000114));
    }
    else {
      uVar1 = 0;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


