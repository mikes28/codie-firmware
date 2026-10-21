/* ==== FUN_0800B2C0 @ 0x800b2c0 ==== */

void FUN_0800B2C0(int param_1,short *param_2,undefined4 param_3,undefined4 param_4)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  
  sVar1 = *param_2;
  if (sVar1 == 0) {
    FUN_0800AE5C(param_1,param_2[1],param_2[2],param_2[4]);
    FUN_0800b270(param_1,param_2[3]);
  }
  else if (sVar1 == 4) {
    FUN_0800AE88(param_1,param_2[1],param_2[2],param_2[4]);
    FUN_0800b280(param_1,param_2[3]);
  }
  else if (sVar1 == 8) {
    sVar1 = param_2[1];
    uVar2 = param_2[2];
    sVar3 = param_2[4];
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfeff;
    *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xff0c | sVar3 << 4 | uVar2;
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xf5ff | sVar1 << 8 | 0x100;
    FUN_0800b298(param_1,param_2[3]);
  }
  else {
    uVar2 = param_2[1];
    sVar1 = param_2[2];
    sVar3 = param_2[4];
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xefff;
    uVar4 = *(ushort *)(param_1 + 0x1c) & 0xcff | sVar1 << 8 | sVar3 << 0xc;
    *(ushort *)(param_1 + 0x1c) = uVar4;
    *(ushort *)(param_1 + 0x20) =
         *(ushort *)(param_1 + 0x20) & 0x5fff | (ushort)((uint)uVar2 << 0xc) | 0x1000;
    FUN_0800b2a8(param_1,param_2[3],uVar4,(uint)uVar2 << 0xc,param_4);
  }
  return;
}


