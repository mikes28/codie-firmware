/* ==== FUN_08008C48 @ 0x8008c48 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008C48(void)

{
  undefined2 local_38;
  undefined2 local_36;
  undefined4 local_34;
  undefined2 local_30;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  
  if (-1 < (int)((uint)DAT_20000704 << 0x1e)) {
    FUN_0800AD7C(0x800,1);
    FUN_0800AD9C(1,1);
    FUN_0800AD5C(0x20000,1);
    local_28 = 0x101;
    local_24 = 2;
    local_21 = 0;
    local_23 = 3;
    FUN_0800A964(0x48000000,&local_28);
    FUN_0800AAC8(0x48000000,0,2);
    FUN_0800AAC8(0x48000000,8,2);
    local_28 = 0x8000;
    local_24 = 1;
    local_22 = 0;
    local_21 = 0;
    local_23 = 0;
    FUN_0800A964(0x48000000,&local_28);
    _DAT_48000028 = 0x8000;
    local_2c = 0xd;
    local_2b = 2;
    local_2a = 1;
    FUN_0800AB30(&local_2c);
    local_34 = 0xffff;
    local_38 = 0;
    local_30 = 0;
    local_36 = 0;
    FUN_0800AEC4(0x40012c00,&local_38);
    FUN_0800af4c(0x40012c00,0,1);
    _DAT_40012c18 = _DAT_40012c18 & 0xff0c | 0x21;
    _DAT_40012c20 = _DAT_40012c20 & 0xfffe | 0xb;
    FUN_0800b3ec(0x40012c00,7);
    FUN_0800b3e0(0x40012c00,0x50);
    FUN_0800B38C(0x40012c00,1,1);
    local_2c = 0xf;
    local_2b = 2;
    local_2a = 1;
    FUN_0800AB30(&local_2c);
    FUN_0800AEC4(0x40000000,&local_38);
    FUN_0800af4c(0x40000000,0,1);
    _DAT_40000018 = _DAT_40000018 & 0xff0c | 0x21;
    _DAT_40000020 = _DAT_40000020 & 0xfffe | 0xb;
    FUN_0800b3ec(0x40000000,7);
    FUN_0800b3e0(0x40000000,0x50);
    FUN_0800B38C(0x40000000,1,1);
    DAT_20000704 = 2;
  }
  return;
}


