/* ==== FUN_08006E94 @ 0x8006e94 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006E94(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  uVar3 = param_1 + 1000U & 0xffff;
  uVar1 = 0;
  if (uVar3 < 0x1375) {
    iVar2 = FUN_08007980(0);
    if ((iVar2 == 0) && (_DAT_200004ac == 0)) {
      local_18 = 0xcc;
      local_17 = 0x4e;
      local_16 = 1;
      local_15 = 0;
      local_14 = 0;
      local_13 = 0;
      local_12 = 0;
      local_11 = FUN_080001c4(uVar3 << 5,0x271);
      DAT_200004ab = 1;
      _DAT_200004ac = 0x8006ca1;
      uVar1 = FUN_080078E4(&local_18,8,0);
    }
    else {
      DAT_200004a1 = 2;
      uVar1 = 0;
    }
  }
  return uVar1;
}


