/* ==== FUN_08009A04 @ 0x8009a04 ==== */

void FUN_08009A04(void)

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
  
  if (-1 < (int)((uint)DAT_200008f8 << 0x1e)) {
    FUN_0800AD5C(0x80000,1);
    FUN_0800AD9C(0x80000,1);
    local_1b = 3;
    local_1c = 2;
    local_1a = 0;
    local_19 = 1;
    local_20 = 0xc00;
    FUN_0800A964(0x48000800);
    FUN_0800AAC8(0x48000800,10,0);
    FUN_0800AAC8(0x48000800,0xb,0);
    local_38 = 0x70800;
    local_34 = 0;
    local_30 = 0;
    local_2c = 0;
    local_24 = 0;
    local_28 = 0xc;
    FUN_0800B3FC(0x40004c00,&local_38);
    FUN_0800AB30();
    FUN_08002E1C(&DAT_200009c4,0x200008f9,200);
    DAT_200008f8 = 2;
  }
  return;
}


