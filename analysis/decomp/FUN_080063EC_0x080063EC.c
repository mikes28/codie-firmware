/* ==== FUN_080063EC @ 0x80063ec ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080063EC(void)

{
  int iVar1;
  undefined8 uVar2;
  undefined1 auStack_18 [8];
  uint local_10;
  int local_c;
  
  FUN_080096EC();
  FUN_08008784();
  FUN_080071C0();
  FUN_08009EA0();
  FUN_08008130();
  FUN_08009340();
  FUN_08006000();
  FUN_080030D0();
  FUN_080094F8();
  FUN_08008574();
  if ((DAT_20000410 == '\x03') && (iVar1 = FUN_08006124(), iVar1 == 4)) {
    local_10 = 0;
    local_c = 0;
    FUN_0800A4DC(auStack_18);
    while ((((iVar1 = FUN_08009094(), iVar1 != 0 &&
             (iVar1 = FUN_0800A558(auStack_18,&local_10), iVar1 != 0)) && (local_c == 0)) &&
           (local_10 < 0xb))) {
      FUN_0800ab10();
    }
    DataSynchronizationBarrier(0xf);
    DataSynchronizationBarrier(0xf);
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  FUN_08003330();
  uVar2 = FUN_08003050();
  if (((int)((ulonglong)uVar2 >> 0x20) != 0) || (900000 < (uint)uVar2)) {
    FUN_080062F0();
  }
  if (DAT_20000400 == '\x02') {
    FUN_0800A558(0x20000408,&local_10);
    iVar1 = FUN_08005EE4();
    if (((iVar1 == 0) || (local_c != 0)) || (3000 < local_10)) {
      FUN_080097E8(1);
    }
  }
  return;
}


