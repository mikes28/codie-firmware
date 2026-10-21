/* ==== FUN_08002f38 @ 0x8002f38 ==== */

void FUN_08002f38(undefined4 *param_1)

{
  disableIRQinterrupts();
  param_1[2] = *param_1;
  param_1[3] = *param_1;
  *(undefined2 *)((int)param_1 + 6) = 0;
  enableIRQinterrupts();
  return;
}


