/* ==== FUN_0800A084 @ 0x800a084 ==== */

undefined4 FUN_0800A084(undefined4 param_1)

{
  int iVar1;
  undefined *puVar2;
  
  puVar2 = &DAT_0800c6ec;
  do {
    iVar1 = FUN_0800b740(param_1,puVar2);
    if (iVar1 == 0) {
      return 1;
    }
    puVar2 = puVar2 + 5;
  } while (puVar2 < &UNK_0800c6fb);
  return 0;
}


