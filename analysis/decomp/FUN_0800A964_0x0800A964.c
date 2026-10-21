/* ==== FUN_0800A964 @ 0x800a964 ==== */

void FUN_0800A964(uint *param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar1 = 0;
  uVar2 = 0;
  do {
    uVar5 = 1 << (uVar2 & 0xff);
    if ((*param_2 & uVar5) == uVar5) {
      if ((byte)param_2[1] - 1 < 2) {
        param_1[2] = param_1[2] & ~(3 << (uVar1 & 0xff));
        param_1[2] = (uint)*(byte *)((int)param_2 + 5) << (uVar1 & 0xff) | param_1[2];
        *(ushort *)(param_1 + 1) = (ushort)param_1[1] & ~(ushort)(1 << (uVar2 & 0xff));
        *(ushort *)(param_1 + 1) =
             (ushort)*(byte *)((int)param_2 + 6) << (uVar2 & 0xff) | (ushort)param_1[1];
      }
      *param_1 = *param_1 & ~(3 << (uVar1 & 0xff));
      *param_1 = (uint)(byte)param_2[1] << (uVar1 & 0xff) | *param_1;
      param_1[3] = param_1[3] & ~(3 << ((uVar2 & 0x7f) << 1));
      param_1[3] = (uint)*(byte *)((int)param_2 + 7) << (uVar1 & 0xff) | param_1[3];
    }
    uVar3 = uVar2 + 1;
    uVar5 = uVar1 + 2;
    uVar4 = 1 << (uVar3 & 0xff);
    if ((*param_2 & uVar4) == uVar4) {
      if ((byte)param_2[1] - 1 < 2) {
        param_1[2] = param_1[2] & ~(3 << (uVar5 & 0xff));
        param_1[2] = (uint)*(byte *)((int)param_2 + 5) << (uVar5 & 0xff) | param_1[2];
        *(ushort *)(param_1 + 1) = (ushort)param_1[1] & ~(ushort)(1 << (uVar3 & 0xff));
        *(ushort *)(param_1 + 1) =
             (ushort)*(byte *)((int)param_2 + 6) << (uVar3 & 0xff) | (ushort)param_1[1];
      }
      *param_1 = *param_1 & ~(3 << (uVar5 & 0xff));
      *param_1 = (uint)(byte)param_2[1] << (uVar5 & 0xff) | *param_1;
      param_1[3] = param_1[3] & ~(3 << ((uVar3 & 0x7f) << 1));
      param_1[3] = (uint)*(byte *)((int)param_2 + 7) << (uVar5 & 0xff) | param_1[3];
    }
    uVar2 = uVar2 + 2;
    uVar1 = uVar1 + 4;
  } while (uVar2 != 0x10);
  return;
}


