/* ==== FUN_0800745C @ 0x800745c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800745C(void)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40014000,1);
  if (iVar1 != 0) {
    if (DAT_2000051a == '\x02') {
      FUN_0800AFBC(0x40014000,0);
      DAT_2000051a = 3;
      FUN_0800B38C(0x40014000,1,0);
      FUN_0800af90(0x40014000,0);
      FUN_0800b3c0(0x40014000,1);
      FUN_0800b3c0(0x40014000,2);
      FUN_0800AFA0(0x40014000,0);
      FUN_0800B2C0(0x40014000,&DAT_20000510);
      FUN_0800af54(0x40014000,0xffff);
      FUN_0800B38C(0x40014000,2,1);
      FUN_0800AFBC(0x40014000,0);
      FUN_0800AFA0(0x40014000,1);
      FUN_0800A4DC(&DAT_20000528);
      DAT_2000051a = '\x04';
    }
    FUN_0800b3c0(0x40014000,1);
  }
  iVar1 = FUN_0800B3A8(0x40014000,2);
  if (iVar1 != 0) {
    if (DAT_2000051a == '\x04') {
      FUN_0800AFA0(0x40014000,0);
      _DAT_40014020 = _DAT_40014020 & 0xfffd;
      FUN_0800AFA0(0x40014000,1);
      DAT_2000051a = '\x05';
    }
    else if (DAT_2000051a == '\x05') {
      FUN_0800AFA0(0x40014000,0);
      FUN_0800B38C(0x40014000,2,0);
      FUN_0800B38C(0x40014000,1,0);
      if (_DAT_2000051c == 0) {
        DAT_2000051a = '\x06';
      }
      else {
        DAT_2000051a = '\0';
      }
    }
    else if (DAT_2000051a == '\a') {
      FUN_0800AFBC(0x40014000,0);
      FUN_0800B38C(0x40014000,1,0);
      FUN_0800AFA0(0x40014000,0);
      DAT_2000051a = '\f';
    }
    else if (DAT_2000051a == '\r') {
      FUN_0800AFBC(0x40014000,0);
      FUN_0800b1ec(0x40014000,0x18);
      DAT_2000051a = '\x0e';
    }
    else if (DAT_2000051a == '\x0e') {
      if (DAT_20000589 == 0) {
        DAT_2000051a = '\x10';
        FUN_0800AFBC(0x40014000,0);
        FUN_0800AFA0(0x40014000,0);
      }
      if (DAT_20000589 < 0x80) {
        iVar1 = FUN_0800aabc(0x48000400,0x4000);
        if (iVar1 == 0) {
          (&DAT_2000058a)[_DAT_20000594] =
               (&DAT_2000058a)[_DAT_20000594] & ~(byte)(1 << (_DAT_20000596 & 0xff));
        }
        else {
          (&DAT_2000058a)[_DAT_20000594] =
               (byte)(1 << (_DAT_20000596 & 0xff)) | (&DAT_2000058a)[_DAT_20000594];
        }
        _DAT_20000596 = _DAT_20000596 + 1;
        if (_DAT_20000596 == 8) {
          if ((int)(char)DAT_20000589 == _DAT_20000594 + 1) {
            FUN_0800AFBC(0x40014000,0);
            FUN_0800AFA0(0x40014000,0);
            FUN_0800B38C(0x40014000,2,0);
            DAT_2000051a = '\x10';
          }
          else {
            _DAT_20000594 = _DAT_20000594 + 1;
            _DAT_20000596 = 0;
          }
        }
        if (DAT_2000051a != '\x10') {
          FUN_0800b1ec(0x40014000,6);
          FUN_0800AFBC(0x40014000,1);
          DAT_2000051a = '\r';
        }
      }
      else {
        iVar1 = FUN_0800aabc(0x48000400,0x4000);
        if (iVar1 == 0) {
          DAT_2000058a = 0;
          FUN_0800b1ec(0x40014000,6);
          FUN_0800AFBC(0x40014000,1);
          DAT_2000051a = '\r';
        }
        else {
          DAT_2000058a = 1;
          DAT_2000051a = '\x10';
          FUN_0800AFBC(0x40014000,0);
          FUN_0800AFA0(0x40014000,0);
          FUN_0800B38C(0x40014000,2,0);
        }
      }
    }
    FUN_0800b3c0(0x40014000,2);
  }
  return;
}


