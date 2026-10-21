/* ==== FUN_08007F7C @ 0x8007f7c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007F7C(void)

{
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_20000598 << 0x1e)) {
    FUN_0800AD5C(0xe0000,1);
    FUN_0800AD9C(0x40000,1);
    local_1b = 3;
    local_1c = 2;
    local_1a = 0;
    local_19 = 0;
    local_20 = 0x2002;
    FUN_0800A964(0x48000400,&local_20);
    local_20 = 0x30;
    FUN_0800A964(0x48000800,&local_20);
    FUN_0800AAC8(0x48000800,4,1);
    FUN_0800AAC8(0x48000800,5,1);
    FUN_0800AAC8(0x48000400,0xd,4);
    FUN_0800AAC8(0x48000400,1,4);
    local_20 = 0x400;
    local_1c = 0;
    local_1a = 0;
    local_19 = 0;
    FUN_0800A964(0x48000000,&local_20);
    local_38 = 0x3840;
    local_34 = 0;
    local_30 = 0x2000;
    local_2c = 0;
    local_24 = 0x300;
    local_28 = 0xc;
    FUN_0800B3FC(0x40004800,&local_38);
    _DAT_20000f24 = 0x8007d89;
    FUN_08002E1C(&DAT_2000061c,0x20000599,0x80);
    DAT_20000598 = 2;
  }
  return;
}


