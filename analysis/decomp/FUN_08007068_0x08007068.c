/* ==== FUN_08007068 @ 0x8007068 ==== */

void FUN_08007068(void)

{
  undefined4 local_64;
  undefined1 local_60;
  undefined4 local_5c;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200004b0 << 0x1e)) {
    FUN_0800AD5C(0x20000,1);
    FUN_0800aaa4(&local_20);
    local_1c = 3;
    local_19 = 0;
    local_20 = 0x66;
    FUN_0800A964(0x48000000,&local_20);
    FUN_0800A67C(0x40012400);
    FUN_0800AD7C(0x200,1);
    FUN_0800ac0c(0x1004000);
    FUN_0800AD5C(1,1);
    local_4c = 0x40012440;
    local_48 = 0x200004b6;
    local_44 = 0;
    local_40 = 0x20;
    local_3c = 0;
    local_38 = 0x80;
    local_34 = 0x100;
    local_30 = 0x400;
    local_2c = 0x20;
    local_28 = 0x1000;
    local_24 = 0;
    FUN_0800A840(0x4002001c,&local_4c);
    FUN_0800AD7C(1,1);
    FUN_0800AE14(0x100,1);
    FUN_0800a6cc(&local_64);
    local_60 = 1;
    local_54 = 0;
    local_5c = 0;
    local_64 = 0;
    local_50 = 0;
    FUN_0800A6A0(0x40012400,&local_64);
    FUN_0800a6f8(0x40012400,DAT_200004b1,4);
    FUN_0800a6f8(0x40012400,DAT_200004b2,6);
    FUN_0800a6f8(0x40012400,DAT_200004b3,6);
    FUN_0800a6f8(0x40012400,DAT_200004b4,6);
    FUN_0800a77c(0x40012400,2);
    DAT_200004b0 = 2;
  }
  return;
}


