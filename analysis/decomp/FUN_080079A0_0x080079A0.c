/* ==== FUN_080079A0 @ 0x80079a0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080079A0(void)

{
  undefined1 *local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined1 local_40;
  undefined1 local_3f;
  undefined1 local_3e;
  undefined2 local_3c;
  undefined2 local_3a;
  undefined4 local_38;
  undefined2 local_34;
  undefined1 local_32;
  undefined4 local_30;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  
  if (-1 < (int)((uint)DAT_200004f8 << 0x1e)) {
    FUN_0800AD5C(0x40000,1);
    local_2c = 2;
    local_2a = 1;
    local_29 = 0;
    local_2b = 3;
    local_30 = 0x4000;
    FUN_0800A964(0x48000400,&local_30);
    local_2c = 0;
    local_29 = 0;
    local_30 = 0x1000;
    FUN_0800A964(0x48000400,&local_30);
    local_29 = 1;
    local_30 = 0x8000;
    FUN_0800A964(0x48000400,&local_30);
    FUN_0800AD7C(0x10000,1);
    FUN_0800AAC8(0x48000400,0xe,1);
    local_3a = 0;
    local_3c = 0x1d;
    local_38 = 0;
    local_32 = 0;
    local_34 = 0;
    FUN_0800AEC4();
    local_40 = 0x14;
    local_3e = 1;
    local_3f = 3;
    FUN_0800AB30(&local_40);
    FUN_0800b1fc(0x40014000,0);
    FUN_0800AF74(0x40014000,0);
    FUN_0800b1d8(&DAT_200004fc);
    _DAT_200004fc = 0x70;
    _DAT_2000050c = 0x100;
    _DAT_200004fe = 1;
    _DAT_20000508 = 2;
    _DAT_20000504 = 0;
    FUN_0800AFDC(0x40014000,&DAT_200004fc);
    FUN_0800AD5C(1,1);
    FUN_0800A7B0();
    local_6c = &DAT_40014034;
    local_68 = 0x20000530;
    local_64 = 0x10;
    local_60 = 1;
    local_5c = 0;
    local_58 = 0x80;
    local_54 = 0x100;
    local_50 = 0;
    local_4c = 0;
    local_48 = 0x3000;
    local_44 = 0;
    FUN_0800A840(0x40020058,&local_6c);
    local_40 = 0xb;
    local_3e = 1;
    local_3f = 4;
    FUN_0800AB30(&local_40);
    FUN_0800A8A0(0x40020058,2,1);
    DAT_200004f8 = 2;
  }
  return;
}


