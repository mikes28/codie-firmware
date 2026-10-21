/* ==== FUN_08006668 @ 0x8006668 ==== */

void FUN_08006668(undefined1 *param_1)

{
  short sVar1;
  undefined4 uVar2;
  uint local_c;
  
  if (*(short *)(param_1 + 2) == 0x1e) {
    if (param_1[1] == '\x02') {
      FUN_08002ac0(0x1b,*param_1,0);
    }
    sVar1 = *(short *)(param_1 + 9);
    local_c = (uint)*(ushort *)(param_1 + 7);
    if (sVar1 == 1) {
      uVar2 = FUN_0800656C(3,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 2) {
      uVar2 = FUN_0800656C(4,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 3) {
      uVar2 = FUN_0800656C(5,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 4) {
      uVar2 = FUN_0800656C(6,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 5) {
      uVar2 = FUN_0800656C(7,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 6) {
      uVar2 = FUN_0800656C(8,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 7) {
      uVar2 = FUN_0800656C(9,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 8) {
      uVar2 = FUN_0800656C(10,&local_c,param_1 + 0xb);
    }
    else {
      uVar2 = 0;
      if (sVar1 == 9) {
        uVar2 = FUN_0800656C(0xb,&local_c,param_1 + 0xb);
      }
    }
    if (local_c != 0) {
      FUN_08006828(local_c,uVar2);
    }
  }
  else if (*(short *)(param_1 + 2) == 0x28) {
    if (param_1[1] == '\x02') {
      FUN_08002ac0(0x1b,*param_1,0);
    }
    sVar1 = *(short *)(param_1 + 9);
    local_c = (uint)*(ushort *)(param_1 + 7);
    if (sVar1 == 1) {
      FUN_0800656C(0xc,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 2) {
      FUN_0800656C(0xd,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 3) {
      FUN_0800656C(0xe,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 4) {
      FUN_0800656C(0xf,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 5) {
      FUN_0800656C(0x10,&local_c,param_1 + 0xb);
    }
  }
  else if (param_1[1] == '\x02') {
    FUN_08002ac0(0x1b,*param_1,1);
  }
  return;
}


