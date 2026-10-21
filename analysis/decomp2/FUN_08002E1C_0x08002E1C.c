/* ==== FUN_08002E1C @ 0x8002e1c ==== */

undefined4 FUN_08002E1C(int *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  
  if (param_1 == (int *)0x0) {
    uVar1 = 0;
  }
  else {
    uVar1 = 0;
    if (param_3 != 0) {
      if (param_2 == 0) {
        uVar1 = 0;
      }
      else {
        *param_1 = param_2;
        param_1[2] = param_2;
        param_1[3] = param_2;
        *(short *)(param_1 + 1) = (short)param_3;
        *(undefined2 *)((int)param_1 + 6) = 0;
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}


