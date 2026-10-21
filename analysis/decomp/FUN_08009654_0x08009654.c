/* ==== FUN_08009654 @ 0x8009654 ==== */

void FUN_08009654(void)

{
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  
  if (-1 < (int)((uint)DAT_200008c8 << 0x1e)) {
    FUN_0800AD5C(0x80000,1);
    local_24 = 0;
    local_21 = 2;
    local_23 = 1;
    local_28 = 0x2000;
    FUN_0800A964(0x48000800,&local_28);
    local_24 = 1;
    local_22 = 0;
    local_21 = 0;
    local_23 = 0;
    local_28 = 0xd000;
    FUN_0800A964(0x48000800,&local_28);
    FUN_0800AD7C(1,1);
    FUN_0800AE34(2,0xd);
    FUN_0800A8D4();
    FUN_0800AB30();
    DAT_200008c8 = 2;
  }
  return;
}


