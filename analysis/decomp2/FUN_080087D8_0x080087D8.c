/* ==== FUN_080087D8 @ 0x80087d8 ==== */

void FUN_080087D8(void)

{
  undefined2 local_40;
  undefined2 local_3e;
  undefined2 local_3c;
  undefined4 local_38;
  undefined2 local_34;
  undefined2 local_32;
  undefined2 local_30;
  undefined2 local_2e;
  undefined2 local_2c;
  undefined2 local_2a;
  undefined4 local_28;
  undefined2 local_24;
  undefined1 local_22;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200006fc << 0x1e)) {
    FUN_0800AD5C(0x80000,1);
    FUN_0800aaa4(&local_20);
    local_1c = 2;
    local_1a = 0;
    local_19 = 0;
    local_1b = 3;
    local_20 = 0x3c0;
    FUN_0800A964(0x48000800,&local_20);
    local_1c = 1;
    local_19 = 0;
    local_1b = 0;
    local_20 = 4;
    FUN_0800A964(0x48000800,&local_20);
    local_1c = 0;
    local_19 = 1;
    local_20 = 2;
    FUN_0800A964(0x48000800,&local_20);
    FUN_0800AAC8(0x48000800,6,0);
    FUN_0800AAC8(0x48000800,7,0);
    FUN_0800AAC8(0x48000800,8,0);
    FUN_0800AAC8(0x48000800,9,0);
    FUN_0800AD9C(2,1);
    FUN_0800b1fc(0x40000400,8);
    FUN_0800b208(0x40000400,8);
    FUN_0800b220(0x40000400,8);
    FUN_0800b22c(0x40000400,8);
    FUN_0800AF74(0x40000400,0);
    local_2a = 0x20;
    local_2c = 0;
    local_28 = 1000;
    local_22 = 0;
    local_24 = 0;
    FUN_0800AEC4(0x40000400);
    local_40 = 0x60;
    local_30 = 0x100;
    local_2e = 0x200;
    local_34 = 0;
    local_32 = 8;
    local_3e = 1;
    local_3c = 0;
    local_38 = 0;
    FUN_0800AFDC(0x40000400,&local_40);
    FUN_0800B054(0x40000400,&local_40);
    FUN_0800B0F4(0x40000400,&local_40);
    FUN_0800B174(0x40000400,&local_40);
    DAT_200006fc = 2;
  }
  return;
}


