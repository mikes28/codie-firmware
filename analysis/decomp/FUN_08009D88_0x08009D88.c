/* ==== FUN_08009D88 @ 0x8009d88 ==== */

void FUN_08009D88(void)

{
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200009e0 << 0x1e)) {
    FUN_0800AD5C(0x60000,1);
    local_1c = 0;
    local_19 = 2;
    local_1b = 3;
    local_20 = 0x20;
    FUN_0800A964(0x48000400,&local_20);
    local_1c = 1;
    local_1a = 0;
    local_19 = 0;
    local_20 = 0x80;
    FUN_0800A964(0x48000000,&local_20);
    FUN_0800AD7C(1,1);
    FUN_0800AE34(1,5);
    local_28 = 0x20;
    local_24 = 0;
    local_23 = 0x10;
    local_22 = 1;
    FUN_0800A8D4(&local_28);
    DAT_200009e0 = 2;
  }
  return;
}


