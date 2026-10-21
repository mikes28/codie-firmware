/* ==== FUN_08009EA0 @ 0x8009ea0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009EA0(void)

{
  int iVar1;
  uint local_18;
  int local_14;
  
  if (((int)(_DAT_20000a00 << 0x1f) < 0) &&
     ((FUN_0800A558(0x200009f8,&local_18), local_14 != 0 || (0x37 < local_18)))) {
    DAT_20000a08 = 3;
    _DAT_20000a00 = _DAT_20000a00 & 0xfffffffe;
    if (_DAT_20000a04 != (code *)0x0) {
      (*_DAT_20000a04)(3);
    }
    _DAT_48000018 = 0x80;
    FUN_08002DD4(8);
    _DAT_48000028 = 0x80;
  }
  if ((int)(_DAT_20000a00 << 0x1e) < 0) {
    iVar1 = FUN_0800A5C8(&DAT_200009f0,&DAT_200009e8,&local_18);
    if (iVar1 != 0) {
      _DAT_200009e4 =
           FUN_080002fc(local_18 * 10,
                        -((int)(local_18 * 5) >> 0x1f) |
                        (local_14 + (local_14 << 2 | local_18 >> 0x1e) +
                        (uint)CARRY4(local_18,local_18 * 4)) * 2,0x3a,0);
    }
    _DAT_20000a00 = _DAT_20000a00 & 0xfffffffc;
    if (_DAT_20000a04 != (code *)0x0) {
      (*_DAT_20000a04)(0);
    }
  }
  return;
}


