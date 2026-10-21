/* ==== FUN_08006000 @ 0x8006000 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006000(void)

{
  int iVar1;
  uint local_18;
  int local_14;
  
  FUN_0800A558(0x20000f70,&local_18);
  if ((((local_14 == 0) && (local_18 <= _DAT_2000035c)) || (_DAT_2000035e == 0)) &&
     ((local_14 == 0 && (local_18 < 0x321)))) {
    if (_DAT_20000374 == 0) {
      return;
    }
  }
  else if (_DAT_20000374 == 0) goto LAB_0800604c;
  _DAT_20000374 = 0;
LAB_0800604c:
  if ((_DAT_2000035e != 0) && (_DAT_20000360 != (code *)0x0)) {
    if ((_DAT_20000358 == 0) && (_DAT_2000035c == 0)) {
      _DAT_2000035c = 0xa0;
    }
    iVar1 = (*_DAT_20000360)(_DAT_20000358);
    if (iVar1 == 0) {
      _DAT_20000360 = (code *)0x0;
      _DAT_2000035e = 0;
      FUN_08006828(_DAT_200003ac,0);
      _DAT_200003ac = 0;
      if (DAT_2000037e != 0) {
        DAT_2000037e = DAT_2000037e - 1;
        FUN_08005FBC(*(undefined2 *)(&DAT_20000358 + (DAT_2000037e + 0x10) * 2));
      }
    }
    else {
      _DAT_20000358 = _DAT_20000358 + 1;
    }
    FUN_080090A8(&DAT_20000385);
  }
  iVar1 = FUN_08009268();
  if (iVar1 == 0) {
    FUN_080092D4();
  }
  FUN_0800A4DC(0x20000f70);
  return;
}


