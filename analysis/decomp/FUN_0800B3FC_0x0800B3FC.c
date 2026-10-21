/* ==== FUN_0800B3FC @ 0x800b3fc ==== */

void FUN_0800B3FC(uint *param_1,uint *param_2)

{
  int iVar1;
  uint extraout_r1;
  uint extraout_r1_00;
  int iVar2;
  uint uVar3;
  ushort uVar4;
  undefined1 auStack_40 [8];
  int local_38;
  int local_28;
  
  *param_1 = *param_1 & 0xfffffffe;
  param_1[1] = param_1[1] & 0xffffcfff | param_2[2];
  *param_1 = param_2[3] | param_2[1] | param_2[4] | *param_1 & 0xffffe9f3;
  param_1[2] = param_1[2] & 0xfffffcff | param_2[5];
  FUN_0800AC40(auStack_40);
  iVar2 = local_38;
  if (param_1 == (uint *)0x40013800) {
    iVar2 = local_28;
  }
  if ((int)(*param_1 << 0x10) < 0) {
    uVar3 = *param_2;
    iVar1 = FUN_08000128(iVar2 * 2,uVar3);
    FUN_080001b0(iVar2 * 2,uVar3);
    uVar3 = extraout_r1;
  }
  else {
    uVar3 = *param_2;
    iVar1 = FUN_08000128(iVar2);
    FUN_080001b0(iVar2,uVar3);
    uVar3 = extraout_r1_00;
  }
  uVar3 = iVar1 + (uint)(*param_2 >> 1 <= uVar3);
  uVar4 = (ushort)uVar3;
  if ((int)(*param_1 << 0x10) < 0) {
    uVar4 = uVar4 & 0xfff0 | (ushort)((uVar3 & 0xf) >> 1);
  }
  *(ushort *)(param_1 + 3) = uVar4;
  return;
}


