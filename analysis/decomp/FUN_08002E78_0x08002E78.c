/* ==== FUN_08002E78 @ 0x8002e78 ==== */

int FUN_08002E78(int *param_1,undefined1 *param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  disableIRQinterrupts();
  if (*(short *)((int)param_1 + 6) == 0) {
    return 0;
  }
  if (*(ushort *)((int)param_1 + 6) < param_3) {
    param_3 = (uint)*(ushort *)((int)param_1 + 6);
  }
  enableIRQinterrupts();
  disableIRQinterrupts();
  enableIRQinterrupts();
  uVar3 = param_3;
  if ((param_3 != 0) && (*(short *)((int)param_1 + 6) != 0)) {
    uVar1 = param_3;
    if (-1 < (int)(param_3 << 0x1f)) {
      *param_2 = *(undefined1 *)param_1[2];
      param_2 = param_2 + 1;
      iVar2 = param_1[2];
      param_1[2] = iVar2 + 1;
      if (iVar2 + 1 == *param_1 + (uint)*(ushort *)(param_1 + 1)) {
        param_1[2] = *param_1;
      }
      uVar1 = param_3 - 1;
      disableIRQinterrupts();
      enableIRQinterrupts();
      uVar3 = uVar1;
      if (*(short *)((int)param_1 + 6) == 0) goto LAB_08002ed8;
    }
    do {
      *param_2 = *(undefined1 *)param_1[2];
      iVar2 = param_1[2];
      param_1[2] = iVar2 + 1;
      if (iVar2 + 1 == *param_1 + (uint)*(ushort *)(param_1 + 1)) {
        param_1[2] = *param_1;
      }
      uVar3 = uVar1 - 1;
      disableIRQinterrupts();
      enableIRQinterrupts();
      if ((uVar3 == 0) || (*(short *)((int)param_1 + 6) == 0)) break;
      param_2[1] = *(undefined1 *)param_1[2];
      param_2 = param_2 + 2;
      iVar2 = param_1[2];
      param_1[2] = iVar2 + 1;
      if (iVar2 + 1 == *param_1 + (uint)*(ushort *)(param_1 + 1)) {
        param_1[2] = *param_1;
      }
      uVar1 = uVar1 - 2;
      disableIRQinterrupts();
      enableIRQinterrupts();
      uVar3 = uVar1;
    } while (*(short *)((int)param_1 + 6) != 0);
  }
LAB_08002ed8:
  disableIRQinterrupts();
  *(short *)((int)param_1 + 6) = *(short *)((int)param_1 + 6) - (short)(param_3 - uVar3);
  enableIRQinterrupts();
  return param_3 - uVar3;
}


