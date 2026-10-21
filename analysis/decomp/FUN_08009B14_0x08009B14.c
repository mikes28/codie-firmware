/* ==== FUN_08009B14 @ 0x8009b14 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int FUN_08009B14(undefined1 *param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  if (DAT_200009d8 == '\0') {
    if (_DAT_200009c8 <= _DAT_200009ca) {
      FUN_08002f38(&DAT_200009c4);
    }
    goto LAB_08009bbc;
  }
  disableIRQinterrupts();
  enableIRQinterrupts();
  if (_DAT_200009ca == 0) {
LAB_08009b9a:
    DAT_200009d8 = DAT_200009d8 + -1;
LAB_08009bbc:
    if (_DAT_200009d4 << 0x1e < 0) {
      FUN_0800A2D0(4,"USART framing error",0);
      FUN_0800b548(0x40004c00,2);
    }
    if (_DAT_200009d4 << 0x1b < 0) {
      FUN_0800b548(0x40004c00,0x10);
    }
    if (_DAT_200009d4 << 0x1d < 0) {
      FUN_0800A2D0(4,"USART noise",0);
      FUN_0800b548(0x40004c00,4);
    }
    if (_DAT_200009d4 << 0x1c < 0) {
      FUN_0800A2D0(4,"USART overrun",0);
      FUN_0800b548(0x40004c00,8);
    }
    return 0;
  }
  iVar1 = FUN_08002E44(&DAT_200009c4);
  *param_1 = (char)iVar1;
  puVar3 = param_1;
  if (iVar1 == 10) {
LAB_08009b38:
    *puVar3 = 0;
    DAT_200009d8 = DAT_200009d8 + -1;
    return (int)puVar3 - (int)param_1;
  }
  puVar2 = param_1 + 1;
  if ((int)puVar2 - (int)param_1 == param_2) goto LAB_08009b60;
  if (((uint)(param_1 + param_2 + ~(uint)param_1) & 1) != 0) {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_200009ca == 0) goto LAB_08009b9a;
    iVar1 = FUN_08002E44(&DAT_200009c4);
    *puVar2 = (char)iVar1;
    puVar3 = puVar2;
    if (iVar1 == 10) goto LAB_08009b38;
    puVar2 = param_1 + 2;
    if ((int)puVar2 - (int)param_1 == param_2) goto LAB_08009b60;
  }
  do {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_200009ca == 0) goto LAB_08009b9a;
    iVar1 = FUN_08002E44(&DAT_200009c4);
    *puVar2 = (char)iVar1;
    puVar3 = puVar2;
    if (iVar1 == 10) goto LAB_08009b38;
    puVar3 = puVar2 + 1;
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_200009ca == 0) goto LAB_08009b9a;
    iVar1 = FUN_08002E44(&DAT_200009c4);
    *puVar3 = (char)iVar1;
    if (iVar1 == 10) goto LAB_08009b38;
    puVar2 = puVar2 + 2;
  } while ((int)puVar2 - (int)param_1 != param_2);
LAB_08009b60:
  *param_1 = 0;
  return param_2;
}


