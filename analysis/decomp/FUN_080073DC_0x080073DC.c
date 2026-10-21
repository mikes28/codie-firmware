/* ==== FUN_080073DC @ 0x80073dc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080073DC(int param_1,undefined4 param_2)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  byte bVar4;
  undefined1 auStack_2c [16];
  
  FUN_08003040();
  if ((param_1 != 1) || (iVar2 = FUN_0800b740(&DAT_0800c4e8,param_2), iVar2 != 0)) {
    return 0;
  }
  bVar4 = 1;
  bVar3 = 0;
  do {
    pcVar1 = _DAT_20000a10;
    FUN_080068D4(*(undefined2 *)((uint)bVar3 * 2 + 0x200004b6),auStack_2c,10);
    (*pcVar1)();
    FUN_0800A140();
    if (bVar3 != 0) {
      if ((bVar3 & 7) == 0) {
        FUN_0800A140();
      }
      if (0x1f < bVar4) {
        return 0;
      }
    }
    bVar3 = bVar3 + 1;
    bVar4 = bVar4 + 1;
  } while( true );
}


