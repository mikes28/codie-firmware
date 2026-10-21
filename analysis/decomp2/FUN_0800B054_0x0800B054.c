/* ==== FUN_0800B054 @ 0x800b054 ==== */

void FUN_0800B054(int param_1,short *param_2)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xffef;
  uVar3 = *(ushort *)(param_1 + 4);
  sVar1 = *param_2;
  uVar2 = *(ushort *)(param_1 + 0x20) & 0xffdf | param_2[6] << 4 | param_2[1] << 4;
  if (param_1 == 0x40012c00) {
    uVar2 = (uVar2 & 0xff7f | param_2[7] << 4) & 0xffbf | param_2[2] << 4;
    uVar3 = (uVar3 & 0xfbff | param_2[8] << 2) & 0xf7ff | param_2[9] << 2;
  }
  else if (param_1 == 0x40014000) {
    uVar3 = uVar3 & 0xfbff | param_2[8] << 2;
  }
  *(ushort *)(param_1 + 4) = uVar3;
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0x8cff | sVar1 << 8;
  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = uVar2;
  return;
}


