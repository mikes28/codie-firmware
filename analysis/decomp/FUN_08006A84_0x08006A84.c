/* ==== FUN_08006A84 @ 0x8006a84 ==== */

void FUN_08006A84(undefined4 *param_1,undefined4 *param_2)

{
  if (param_2 != (undefined4 *)0x0) {
    *param_1 = *param_2;
    param_1[1] = param_2[1];
    param_1[2] = param_2[2];
    param_1[3] = param_2[3];
    param_1[6] = param_2[4];
    param_1[7] = param_2[5];
    param_1[8] = param_2[6];
    param_1[10] = param_2[7];
  }
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[9] = 0;
  return;
}


