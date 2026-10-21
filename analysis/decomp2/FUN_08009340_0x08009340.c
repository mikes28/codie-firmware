/* ==== FUN_08009340 @ 0x8009340 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009340(void)

{
  int iVar1;
  char cVar2;
  char cVar3;
  uint local_18;
  int local_14;
  
  local_18 = 0;
  local_14 = 0;
  FUN_0800A558(0x20000fc8,&local_18);
  iVar1 = _DAT_20000864;
  if ((local_14 != 0) || (_DAT_2000086a < local_18)) {
    _DAT_20000864 = FUN_080071DC(4,0,0);
    _DAT_20000864 = _DAT_20000864 + iVar1;
    cVar3 = DAT_2000086c + '\x01';
    cVar2 = DAT_2000086c + '\x01';
    DAT_2000086c = cVar3;
    if (DAT_20000868 == cVar2) {
      _DAT_2000086e = FUN_08000128();
      _DAT_20000864 = 0;
      DAT_2000086c = '\0';
    }
    FUN_0800A4DC(0x20000fc8);
  }
  return;
}


