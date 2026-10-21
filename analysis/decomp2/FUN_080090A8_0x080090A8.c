/* ==== FUN_080090A8 @ 0x80090a8 ==== */

void FUN_080090A8(int param_1)

{
  int extraout_r1;
  byte *pbVar1;
  undefined1 *puVar2;
  int iVar3;
  
  puVar2 = &DAT_20000710;
  iVar3 = 0;
  do {
    FUN_08000270(iVar3 + 0xb,0xc);
    pbVar1 = (byte *)(extraout_r1 * 3 + param_1);
    if ((*pbVar1 & 0x80) == 0) {
      puVar2[1] = 0xe;
    }
    else {
      puVar2[1] = 0x2e;
    }
    if ((*pbVar1 & 0x40) == 0) {
      puVar2[2] = 0xe;
    }
    else {
      puVar2[2] = 0x2e;
    }
    if ((*pbVar1 & 0x20) == 0) {
      puVar2[3] = 0xe;
    }
    else {
      puVar2[3] = 0x2e;
    }
    if ((*pbVar1 & 0x10) == 0) {
      puVar2[4] = 0xe;
    }
    else {
      puVar2[4] = 0x2e;
    }
    if ((*pbVar1 & 8) == 0) {
      puVar2[5] = 0xe;
    }
    else {
      puVar2[5] = 0x2e;
    }
    if ((*pbVar1 & 4) == 0) {
      puVar2[6] = 0xe;
    }
    else {
      puVar2[6] = 0x2e;
    }
    if ((*pbVar1 & 2) == 0) {
      puVar2[7] = 0xe;
    }
    else {
      puVar2[7] = 0x2e;
    }
    if ((*pbVar1 & 1) == 0) {
      puVar2[8] = 0xe;
    }
    else {
      puVar2[8] = 0x2e;
    }
    if ((pbVar1[1] & 0x80) == 0) {
      puVar2[9] = 0xe;
    }
    else {
      puVar2[9] = 0x2e;
    }
    if ((pbVar1[1] & 0x40) == 0) {
      puVar2[10] = 0xe;
    }
    else {
      puVar2[10] = 0x2e;
    }
    if ((pbVar1[1] & 0x20) == 0) {
      puVar2[0xb] = 0xe;
    }
    else {
      puVar2[0xb] = 0x2e;
    }
    if ((pbVar1[1] & 0x10) == 0) {
      puVar2[0xc] = 0xe;
    }
    else {
      puVar2[0xc] = 0x2e;
    }
    if ((pbVar1[1] & 8) == 0) {
      puVar2[0xd] = 0xe;
    }
    else {
      puVar2[0xd] = 0x2e;
    }
    if ((pbVar1[1] & 4) == 0) {
      puVar2[0xe] = 0xe;
    }
    else {
      puVar2[0xe] = 0x2e;
    }
    if ((pbVar1[1] & 2) == 0) {
      puVar2[0xf] = 0xe;
    }
    else {
      puVar2[0xf] = 0x2e;
    }
    if ((pbVar1[1] & 1) == 0) {
      puVar2[0x10] = 0xe;
    }
    else {
      puVar2[0x10] = 0x2e;
    }
    if ((pbVar1[2] & 0x80) == 0) {
      puVar2[0x11] = 0xe;
    }
    else {
      puVar2[0x11] = 0x2e;
    }
    if ((pbVar1[2] & 0x40) == 0) {
      puVar2[0x12] = 0xe;
    }
    else {
      puVar2[0x12] = 0x2e;
    }
    if ((pbVar1[2] & 0x20) == 0) {
      puVar2[0x13] = 0xe;
    }
    else {
      puVar2[0x13] = 0x2e;
    }
    if ((pbVar1[2] & 0x10) == 0) {
      puVar2[0x14] = 0xe;
    }
    else {
      puVar2[0x14] = 0x2e;
    }
    if ((pbVar1[2] & 8) == 0) {
      puVar2[0x15] = 0xe;
    }
    else {
      puVar2[0x15] = 0x2e;
    }
    if ((pbVar1[2] & 4) == 0) {
      puVar2[0x16] = 0xe;
    }
    else {
      puVar2[0x16] = 0x2e;
    }
    if ((pbVar1[2] & 2) == 0) {
      puVar2[0x17] = 0xe;
    }
    else {
      puVar2[0x17] = 0x2e;
    }
    if ((pbVar1[2] & 1) == 0) {
      puVar2[0x18] = 0xe;
    }
    else {
      puVar2[0x18] = 0x2e;
    }
    iVar3 = iVar3 + 1;
    puVar2 = puVar2 + 0x18;
  } while (iVar3 != 0xc);
  puVar2 = &DAT_20000831;
  do {
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2 = puVar2 + 2;
  } while (puVar2 != (undefined1 *)0x2000084f);
  return;
}


