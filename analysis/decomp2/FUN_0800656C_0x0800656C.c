/* ==== FUN_0800656C @ 0x800656c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800656C(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  uint uVar2;
  byte *pbVar3;
  
  uVar2 = (uint)(byte)(DAT_2000102c - 1U);
  pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
  if (*pbVar3 == param_1) {
LAB_08006598:
    uVar1 = (**(code **)(pbVar3 + 4))(param_2,param_3,param_3,uVar2,param_4);
  }
  else {
    if (uVar2 != 0) {
      if ((DAT_2000102c - 1U & 1) != 0) {
        uVar2 = uVar2 - 1 & 0xff;
        pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
        if (*pbVar3 == param_1) goto LAB_08006598;
        if (uVar2 == 0) goto LAB_080065a2;
      }
      do {
        uVar2 = uVar2 - 1 & 0xff;
        pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
        if (*pbVar3 == param_1) goto LAB_08006598;
        uVar2 = uVar2 - 1 & 0xff;
        pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
        if (*pbVar3 == param_1) goto LAB_08006598;
      } while (uVar2 != 0);
    }
LAB_080065a2:
    uVar1 = 0x7f;
  }
  return uVar1;
}


