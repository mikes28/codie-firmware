/* ==== FUN_08007B34 @ 0x8007b34 ==== */

// WARNING: Removing unreachable block (ram,0x08007b54)
// WARNING: Removing unreachable block (ram,0x08007ba8)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007B34(void)

{
  undefined4 in_r3;
  
  if (DAT_2000051a == '\x04') {
    FUN_0800A500(&DAT_20000528,&DAT_20000f80);
    if ((_DAT_20000f84 != 0) || (0x46 < _DAT_20000f80)) {
      FUN_0800B38C(0x40014000,2,0);
      FUN_0800AFA0(0x40014000,0);
      DAT_20000520 = 0;
      DAT_2000051a = '\x01';
      (*_DAT_20000a10)("PPRT\n");
    }
  }
  else if (DAT_2000051a == '\x05') {
    FUN_0800A500(&DAT_20000528,&DAT_20000f80);
    if ((_DAT_20000f84 != 0) || (0x13b < _DAT_20000f80)) {
      FUN_0800B38C(0x40014000,2,0);
      FUN_0800AFA0(0x40014000,0);
      DAT_20000520 = 1;
      DAT_2000051a = '\x01';
      (*_DAT_20000a10)("PPRET\n");
    }
  }
  else if (DAT_2000051a == '\x06') {
    if ((_DAT_40014034 < 0x5a) || (0x187 < _DAT_40014034)) {
      DAT_20000520 = 2;
      DAT_2000051a = '\x01';
    }
    else {
      FUN_0800af54(0x40014000,0x66,&DAT_200004f8,0x187,in_r3);
      _DAT_200004fc = 0x60;
      _DAT_2000050c = 0x100;
      _DAT_200004fe = 1;
      _DAT_20000508 = 2;
      _DAT_20000504 = 0;
      FUN_0800AFDC(0x40014000,&DAT_200004fc);
      FUN_0800B3C8(0x40014000,0x100,1);
      FUN_0800a89c(0x40020058,(uint)DAT_20000588 << 3);
      FUN_0800b1ec(0x40014000,0xffff);
      FUN_0800A87C(0x40020058,1);
      _DAT_40014024 = 0;
      FUN_0800AFA0(0x40014000,1);
      FUN_0800b3a4(0x40014000,1);
      FUN_0800AFBC(0x40014000,1);
      DAT_2000051a = '\a';
    }
  }
  else if (DAT_2000051a == '\f') {
    if (DAT_20000589 == '\0') {
      FUN_080077A0(1);
    }
    else {
      _DAT_20000594 = 0;
      _DAT_20000596 = 0;
      FUN_0800791C();
    }
  }
  else if (DAT_2000051a == '\x10') {
    FUN_080077A0(1);
  }
  return;
}


