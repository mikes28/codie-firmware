/* ==== FUN_08002958 @ 0x8002958 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002958(void)

{
  uint uVar1;
  uint uVar2;
  int local_10;
  
  _DAT_4002102c = _DAT_4002102c & 0xfffffff0;
  _DAT_40021030 = _DAT_40021030 & 0xfffffeac;
  _DAT_40021034 = _DAT_40021034 & 0xfffffffe;
  _DAT_40021008 = 0;
  local_10 = 0;
  uVar2 = _DAT_40021000 & 0xfef2ffff | 0x10001;
  do {
    local_10 = local_10 + 1;
    if ((_DAT_40021000 & 0x20000) != 0) break;
  } while (local_10 != 0x5000);
  uVar1 = _DAT_40021004 & 0xf8c0b80c;
  if ((int)(uVar2 << 0xe) < 0) {
    _DAT_40022000 = 0x11;
    do {
    } while ((_DAT_40021000 & 0x2000000) == 0);
    do {
      uVar2 = _DAT_40021000 & 0xfef2ffff | 0x1010001;
      uVar1 = _DAT_40021004 & 0xf8c0380c | 0x130002;
    } while ((_DAT_40021004 & 0xc) != 8);
  }
  _DAT_40021004 = uVar1;
  _DAT_40021000 = uVar2;
  return;
}


