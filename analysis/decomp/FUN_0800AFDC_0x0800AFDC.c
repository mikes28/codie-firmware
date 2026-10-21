/* ==== FUN_0800AFDC @ 0x800afdc ==== */

void FUN_0800AFDC(int param_1,ushort *param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfffe;
  uVar2 = *(ushort *)(param_1 + 4);
  uVar1 = *param_2;
  uVar3 = param_2[6] | param_2[1] | *(ushort *)(param_1 + 0x20) & 0xfffd;
  if ((((param_1 == 0x40012c00) || (param_1 == 0x40014000)) || (param_1 == 0x40014400)) ||
     (param_1 == 0x40014800)) {
    uVar3 = (uVar3 & 0xfff7 | param_2[7]) & 0xfffb | param_2[2];
    uVar2 = uVar2 & 0xfcff | param_2[8] | param_2[9];
  }
  *(ushort *)(param_1 + 4) = uVar2;
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xff8c | uVar1;
  *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = uVar3;
  return;
}


