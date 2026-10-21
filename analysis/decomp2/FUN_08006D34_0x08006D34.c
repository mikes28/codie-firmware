/* ==== FUN_08006D34 @ 0x8006d34 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006D34(void)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = FUN_08007980();
  uVar3 = 1;
  if (iVar2 == 0) {
    if (DAT_200004ab == '\x01') {
      FUN_08006D18();
      DAT_200004ab = '\x02';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x02') {
      FUN_08006C60(0);
      DAT_200004ab = '\x03';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x03') {
      FUN_08006CDC(0);
      DAT_200004ab = '\x04';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x04') {
      DAT_200004ab = 5;
      iVar2 = FUN_08007d7c();
      _DAT_200004a2 = (short)*(char *)(iVar2 + 2);
      iVar2 = FUN_08007d7c();
      bVar1 = *(byte *)(iVar2 + 4);
      iVar2 = FUN_08007d7c();
      _DAT_200004a4 = ((bVar1 & 3) * 0x100 + (ushort)*(byte *)(iVar2 + 3)) * 10;
      iVar2 = FUN_08007d7c();
      bVar1 = *(byte *)(iVar2 + 6);
      iVar2 = FUN_08007d7c();
      iVar2 = (short)((ushort)*(byte *)(iVar2 + 5) + (ushort)bVar1 * 0x100) * 0x271;
      _DAT_200004a6 = (undefined2)((int)(((uint)(iVar2 >> 0x1f) >> 0x1a) + iVar2) >> 6);
      FUN_08006C60(1);
      DAT_200004ab = '\x06';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x06') {
      FUN_08006CDC(1);
      DAT_200004ab = '\a';
      uVar3 = 1;
    }
    else {
      uVar3 = 1;
      if (DAT_200004ab == '\a') {
        DAT_200004ab = 8;
        iVar2 = FUN_08007d7c();
        _DAT_200004a8 = (short)((int)((uint)*(byte *)(iVar2 + 4) * 0x271) >> 5) + -1000;
        DAT_200004ab = '\0';
        uVar3 = 0;
      }
    }
  }
  return uVar3;
}


