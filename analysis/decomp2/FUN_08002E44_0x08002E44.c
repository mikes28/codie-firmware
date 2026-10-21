/* ==== FUN_08002E44 @ 0x8002e44 ==== */

undefined1 FUN_08002E44(int *param_1)

{
  undefined1 uVar1;
  undefined1 *puVar2;
  
  disableIRQinterrupts();
  uVar1 = 0;
  if (*(short *)((int)param_1 + 6) != 0) {
    enableIRQinterrupts();
    uVar1 = *(undefined1 *)param_1[2];
    puVar2 = (undefined1 *)param_1[2] + 1;
    param_1[2] = (int)puVar2;
    if (puVar2 == (undefined1 *)(*param_1 + (uint)*(ushort *)(param_1 + 1))) {
      param_1[2] = *param_1;
    }
    disableIRQinterrupts();
    *(short *)((int)param_1 + 6) = *(short *)((int)param_1 + 6) + -1;
    enableIRQinterrupts();
  }
  return uVar1;
}


