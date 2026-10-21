/* ==== FUN_08009F8C @ 0x8009f8c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009F8C(void)

{
  bool bVar1;
  
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setMainStackPointer(0x200014a8);
  }
  (*_DAT_1fffc804)();
  return;
}


