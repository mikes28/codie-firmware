/* ==== FUN_08005910 @ 0x8005910 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005910(void)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1c;
  undefined1 local_1b;
  byte local_1a;
  
  bVar1 = DAT_20000366;
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 0x19;
  }
  local_1c = DAT_20000364;
  local_1b = DAT_20000365;
  local_1a = DAT_20000366;
  uVar2 = FUN_080012F0(_DAT_20000358 << 1);
  uVar2 = FUN_08000BFC(uVar2,0x40490fdb);
  uVar2 = FUN_08000818(uVar2,0x42200000);
  FUN_08000E5C(uVar2,0x3fc90fdb);
  uVar2 = FUN_08002F48();
  uVar3 = FUN_08001248(bVar1 >> 1);
  uVar2 = FUN_08000BFC(uVar3,uVar2);
  uVar3 = FUN_08001248(DAT_20000366 >> 1);
  FUN_08000474(uVar2,uVar3);
  local_1a = FUN_08000380();
  FUN_08005848(&local_1c,&local_20);
  DAT_20000386 = local_1f;
  DAT_20000385 = local_20;
  DAT_20000387 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000389 = local_1f;
  DAT_20000388 = local_20;
  DAT_2000038a = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000038c = local_1f;
  DAT_2000038b = local_20;
  DAT_2000038d = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000038f = local_1f;
  DAT_2000038e = local_20;
  DAT_20000390 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000392 = local_1f;
  DAT_20000391 = local_20;
  DAT_20000393 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000395 = local_1f;
  DAT_20000394 = local_20;
  DAT_20000396 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000398 = local_1f;
  DAT_20000397 = local_20;
  DAT_20000399 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000039b = local_1f;
  DAT_2000039a = local_20;
  DAT_2000039c = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000039e = local_1f;
  DAT_2000039d = local_20;
  DAT_2000039f = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_200003a1 = local_1f;
  DAT_200003a0 = local_20;
  DAT_200003a2 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_200003a4 = local_1f;
  DAT_200003a3 = local_20;
  DAT_200003a5 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_200003a7 = local_1f;
  DAT_200003a6 = local_20;
  DAT_200003a8 = local_1e;
  return _DAT_20000358 < (uint)(_DAT_20000368 * 0x28);
}


