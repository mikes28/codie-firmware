/* ==== FUN_08006F0C @ 0x8006f0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006F0C(void)

{
  int iVar1;
  
  if ((_DAT_200004ac != (code *)0x0) && (iVar1 = (*_DAT_200004ac)(), iVar1 == 0)) {
    _DAT_200004ac = (code *)0x0;
  }
  FUN_08007B34();
  return;
}


