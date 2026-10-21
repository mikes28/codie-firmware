/* ==== FUN_0800B174 @ 0x800b174 ==== */

void FUN_0800B174(int param_1,short *param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  ushort uVar4;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xefff;
  uVar4 = *(ushort *)(param_1 + 4);
  sVar1 = *param_2;
  sVar2 = param_2[6];
  sVar3 = param_2[1];
  if (param_1 == 0x40012c00) {
    uVar4 = uVar4 & 0xbfff | param_2[8] << 6;
  }
  *(ushort *)(param_1 + 4) = uVar4;
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0x8cff | sVar1 << 8;
  *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xdfff | sVar2 << 0xc | sVar3 << 0xc;
  return;
}


