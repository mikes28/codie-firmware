/* ==== FUN_08005B0C @ 0x8005b0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005B0C(void)

{
  byte bVar1;
  int iVar2;
  byte bVar3;
  uint extraout_r1;
  uint extraout_r1_00;
  uint extraout_r1_01;
  uint extraout_r1_02;
  uint extraout_r1_03;
  uint extraout_r1_04;
  uint extraout_r1_05;
  uint extraout_r1_06;
  uint extraout_r1_07;
  uint extraout_r1_08;
  uint extraout_r1_09;
  uint extraout_r1_10;
  uint extraout_r1_11;
  uint uVar4;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1c;
  undefined1 local_1b;
  byte local_1a;
  
  bVar3 = DAT_20000366;
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 0x50;
  }
  local_1c = DAT_20000364;
  local_1b = DAT_20000365;
  local_1a = DAT_20000366;
  bVar1 = DAT_20000366 >> 2;
  FUN_080001b0(_DAT_20000358,0xc);
  uVar4 = extraout_r1 & 0xff;
  FUN_08000270(uVar4,0xc);
  if (bVar3 < (byte)(bVar1 * '\x04')) {
    local_1a = '\0';
  }
  else {
    local_1a = bVar3 + bVar1 * -4;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_00 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 1,0xc);
  if (DAT_20000366 < (byte)(bVar1 * '\x03')) {
    local_1a = '\0';
  }
  else {
    local_1a = DAT_20000366 + bVar1 * -3;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_01 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 2,0xc);
  if (DAT_20000366 < (byte)(bVar1 * '\x02')) {
    local_1a = '\0';
  }
  else {
    local_1a = DAT_20000366 + bVar1 * -2;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_02 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 3,0xc);
  if (DAT_20000366 < bVar1) {
    local_1a = '\0';
  }
  else {
    local_1a = DAT_20000366 - bVar1;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_03 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  local_1a = DAT_20000366;
  FUN_08005848(&local_1c,&local_20);
  FUN_08000270(uVar4 + 4,0xc);
  iVar2 = (extraout_r1_04 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 5,0xc);
  iVar2 = (extraout_r1_05 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 6,0xc);
  iVar2 = (extraout_r1_06 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 7,0xc);
  iVar2 = (extraout_r1_07 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 8,0xc);
  iVar2 = (extraout_r1_08 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 9,0xc);
  iVar2 = (extraout_r1_09 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 10,0xc);
  iVar2 = (extraout_r1_10 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 0xb,0xc);
  iVar2 = (extraout_r1_11 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  uVar4 = FUN_08000128(_DAT_20000358,0xc);
  return uVar4 < _DAT_20000368;
}


