/* ==== FUN_080096EC @ 0x80096ec ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080096EC(void)

{
  if (DAT_200008c9 == '\x01') {
    FUN_0800A558(0x200008d0,&DAT_20000fd8);
    if ((DAT_200008ca == '\0') && ((_DAT_20000fdc != 0 || (800 < _DAT_20000fd8)))) {
      DAT_200008ca = '\0';
      DAT_200008c9 = '\0';
      (*_DAT_200008ec)();
    }
  }
  else {
    DAT_200008c9 = '\0';
  }
  if (DAT_200008ca == '\x01') {
    if (DAT_200008c9 == '\x01') {
      (*_DAT_200008f0)();
    }
    DAT_200008c9 = '\0';
    DAT_200008ca = '\0';
  }
  return;
}


