/* ==== FUN_08008574 @ 0x8008574 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008574(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint local_10;
  int local_c;
  
  FUN_0800A4DC(&DAT_20000fa8);
  if (_DAT_200006ec != 0) {
    FUN_0800A558(0x20000fb0,&local_10);
    if ((local_c != 0) || (*(ushort *)((uint)_DAT_200006e0 * 6 + 0x20000652) < local_10)) {
      FUN_08008514(0);
      _DAT_200006e0 = _DAT_200006e0 + 1;
      if (_DAT_200006e2 == _DAT_200006e0) {
        _DAT_200006e0 = 0;
        _DAT_200006e2 = 0;
      }
      _DAT_200006ec = 0;
      if (_DAT_200006e8 != 0) {
        FUN_08006828();
        _DAT_200006e8 = 0;
      }
    }
    if (_DAT_200006ec != 0) {
      return;
    }
  }
  uVar3 = (uint)_DAT_200006e0;
  if (uVar3 < _DAT_200006e2) {
    iVar2 = (uVar3 + 9) * 8;
    uVar1 = *(uint *)(iVar2 + 0x2000064c);
    if ((uVar1 <= _DAT_20000fac) &&
       ((uVar1 != _DAT_20000fac || (*(uint *)(&DAT_20000648 + iVar2) <= _DAT_20000fa8)))) {
      FUN_0800849C(*(undefined2 *)(uVar3 * 6 + 0x20000650));
      FUN_08008514(*(byte *)((uint)_DAT_200006e0 * 6 + 0x20000654) >> 1);
      FUN_0800A4DC(0x20000fb0);
      _DAT_200006ec = 1;
    }
  }
  return;
}


