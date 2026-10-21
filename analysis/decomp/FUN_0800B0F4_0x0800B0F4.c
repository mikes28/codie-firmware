/* ==== FUN_0800B0F4 @ 0x800b0f4 ==== */

void FUN_0800B0F4(int param_1,ushort *param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfeff;
  uVar2 = *(ushort *)(param_1 + 4);
  uVar1 = *param_2;
  uVar3 = *(ushort *)(param_1 + 0x20) & 0xfdff | param_2[6] << 8 | param_2[1] << 8;
  if (param_1 == 0x40012c00) {
    uVar3 = (uVar3 & 0xf7ff | param_2[7] << 8) & 0xfbff | param_2[2] << 8;
    uVar2 = uVar2 & 0xcfff | param_2[8] << 4 | param_2[9] << 4;
  }
  *(ushort *)(param_1 + 4) = uVar2;
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xff8c | uVar1;
  *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = uVar3;
  return;
}


