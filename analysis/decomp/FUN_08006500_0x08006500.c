/* ==== FUN_08006500 @ 0x8006500 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006500(uint param_1,undefined4 param_2)

{
  undefined4 uVar1;
  byte *pbVar2;
  uint uVar3;
  
  uVar3 = (uint)(byte)(DAT_2000102c - 1U);
  pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
  if (*pbVar2 == param_1) {
LAB_0800652c:
    uVar1 = (**(code **)(pbVar2 + 4))(param_2);
  }
  else {
    if (uVar3 != 0) {
      if ((DAT_2000102c - 1U & 1) != 0) {
        uVar3 = uVar3 - 1 & 0xff;
        pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
        if (*pbVar2 == param_1) goto LAB_0800652c;
        if (uVar3 == 0) goto LAB_08006534;
      }
      do {
        uVar3 = uVar3 - 1 & 0xff;
        pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
        if (*pbVar2 == param_1) goto LAB_0800652c;
        uVar3 = uVar3 - 1 & 0xff;
        pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
        if (*pbVar2 == param_1) goto LAB_0800652c;
      } while (uVar3 != 0);
    }
LAB_08006534:
    uVar1 = 0x7f;
  }
  return uVar1;
}


