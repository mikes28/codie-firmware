/* ==== FUN_08009CCC @ 0x8009ccc ==== */

int FUN_08009CCC(char *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if ((int)((uint)DAT_200008f8 << 0x1d) < 0) {
    if (*param_1 == '\0') {
      iVar1 = 0;
    }
    else {
      do {
        FUN_08009CA0();
        iVar1 = iVar1 + 1;
      } while (param_1[iVar1] != '\0');
    }
  }
  return iVar1;
}


