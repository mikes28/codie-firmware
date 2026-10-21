/* ==== FUN_080039D8 @ 0x80039d8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080039D8(void)

{
  int iVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  uint uVar5;
  uint extraout_r1;
  uint extraout_r1_00;
  uint uVar6;
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
  
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 100;
    _DAT_20000364 = 0;
    _DAT_20000368 = 0;
  }
  iVar1 = _DAT_20000364;
  bVar2 = FUN_08000128(_DAT_20000364 * 0xc,100);
  _DAT_2000035c = 100 - (short)(_DAT_20000368 >> 1);
  cVar3 = FUN_08000128(iVar1 * -0x16 + 0x898,100);
  cVar4 = FUN_08000128(iVar1 * 0x14,100);
  FUN_080001b0(_DAT_20000358,0xc);
  uVar5 = extraout_r1 & 0xff;
  if (bVar2 == 0) {
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_00) {
      if (uVar5 == 0) {
        iVar1 = (extraout_r1 & 0xff) * 3;
        (&DAT_20000387)[iVar1] = 6;
        (&DAT_20000385)[iVar1] = 6;
        (&DAT_20000386)[iVar1] = 6;
      }
      else {
        uVar6 = extraout_r1 & 0xff;
        if (uVar6 == 0xffffffff) {
          DAT_20000387 = 0xc;
          DAT_20000385 = '\f';
          DAT_20000386 = '\f';
        }
        else if (uVar6 == 0xfffffffe) {
          DAT_20000387 = 7;
          DAT_20000385 = '\a';
          DAT_20000386 = '\a';
        }
        else if (uVar6 == 0xfffffffd) {
          DAT_20000387 = 5;
          DAT_20000385 = '\x05';
          DAT_20000386 = '\x05';
        }
        else {
          DAT_20000387 = 4;
          DAT_20000385 = '\x04';
          DAT_20000386 = '\x04';
        }
      }
      goto LAB_08003b08;
    }
    DAT_20000386 = cVar3 + '\x02';
    DAT_20000385 = cVar4 + '\x02';
    DAT_20000387 = 6;
LAB_08003b0e:
    if (uVar5 == 1) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 0) {
        DAT_2000038a = 0xc;
        DAT_20000388 = '\f';
        DAT_20000389 = '\f';
      }
      else if (uVar6 == 0xffffffff) {
        DAT_2000038a = 7;
        DAT_20000388 = '\a';
        DAT_20000389 = '\a';
      }
      else if (uVar6 == 0xfffffffe) {
        DAT_2000038a = 5;
        DAT_20000388 = '\x05';
        DAT_20000389 = '\x05';
      }
      else {
        DAT_2000038a = 4;
        DAT_20000388 = '\x04';
        DAT_20000389 = '\x04';
      }
    }
LAB_08003bd0:
    if (bVar2 != 2) {
      if (bVar2 < 3) goto LAB_08003bd6;
      DAT_2000038d = 4;
      DAT_2000038b = cVar4;
      DAT_2000038c = cVar3;
      goto LAB_08003c9a;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_02) goto LAB_08003bd6;
    DAT_2000038c = cVar3 + '\x02';
    DAT_2000038b = cVar4 + '\x02';
    DAT_2000038d = 6;
LAB_08003ca0:
    if (uVar5 == 3) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 2) {
        DAT_20000390 = 0xc;
        DAT_2000038e = '\f';
        DAT_2000038f = '\f';
      }
      else if (uVar6 == 1) {
        DAT_20000390 = 7;
        DAT_2000038e = '\a';
        DAT_2000038f = '\a';
      }
      else if (uVar6 == 0) {
        DAT_20000390 = 5;
        DAT_2000038e = '\x05';
        DAT_2000038f = '\x05';
      }
      else {
        DAT_20000390 = 4;
        DAT_2000038e = '\x04';
        DAT_2000038f = '\x04';
      }
    }
LAB_08003d6c:
    if (bVar2 != 4) {
      if (bVar2 < 5) goto LAB_08003d72;
      DAT_20000393 = 4;
      DAT_20000391 = cVar4;
      DAT_20000392 = cVar3;
      goto LAB_08003e36;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_04) goto LAB_08003d72;
    DAT_20000392 = cVar3 + '\x02';
    DAT_20000391 = cVar4 + '\x02';
    DAT_20000393 = 6;
LAB_08003e3c:
    if (uVar5 == 5) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 4) {
        DAT_20000396 = 0xc;
        DAT_20000394 = '\f';
        DAT_20000395 = '\f';
      }
      else if (uVar6 == 3) {
        DAT_20000396 = 7;
        DAT_20000394 = '\a';
        DAT_20000395 = '\a';
      }
      else if (uVar6 == 2) {
        DAT_20000396 = 5;
        DAT_20000394 = '\x05';
        DAT_20000395 = '\x05';
      }
      else {
        DAT_20000396 = 4;
        DAT_20000394 = '\x04';
        DAT_20000395 = '\x04';
      }
    }
LAB_08003f00:
    if (bVar2 != 6) {
      if (bVar2 < 7) goto LAB_08003f06;
      DAT_20000399 = 4;
      DAT_20000397 = cVar4;
      DAT_20000398 = cVar3;
      goto LAB_08003fca;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_06) goto LAB_08003f06;
    DAT_20000398 = cVar3 + '\x02';
    DAT_20000397 = cVar4 + '\x02';
    DAT_20000399 = 6;
LAB_08003fd0:
    if (uVar5 == 7) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 6) {
        DAT_2000039c = 0xc;
        DAT_2000039a = '\f';
        DAT_2000039b = '\f';
      }
      else if (uVar6 == 5) {
        DAT_2000039c = 7;
        DAT_2000039a = '\a';
        DAT_2000039b = '\a';
      }
      else if (uVar6 == 4) {
        DAT_2000039c = 5;
        DAT_2000039a = '\x05';
        DAT_2000039b = '\x05';
      }
      else {
        DAT_2000039c = 4;
        DAT_2000039a = '\x04';
        DAT_2000039b = '\x04';
      }
    }
LAB_0800409a:
    if (bVar2 != 8) {
      if (bVar2 < 9) goto LAB_080040a0;
      DAT_2000039f = 4;
      DAT_2000039d = cVar4;
      DAT_2000039e = cVar3;
      goto LAB_08004164;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_08) goto LAB_080040a0;
    DAT_2000039e = cVar3 + '\x02';
    DAT_2000039d = cVar4 + '\x02';
    DAT_2000039f = 6;
LAB_0800416a:
    if (uVar5 == 9) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 8) {
        DAT_200003a2 = 0xc;
        DAT_200003a0 = '\f';
        DAT_200003a1 = '\f';
      }
      else if (uVar6 == 7) {
        DAT_200003a2 = 7;
        DAT_200003a0 = '\a';
        DAT_200003a1 = '\a';
      }
      else if (uVar6 == 6) {
        DAT_200003a2 = 5;
        DAT_200003a0 = '\x05';
        DAT_200003a1 = '\x05';
      }
      else {
        DAT_200003a2 = 4;
        DAT_200003a0 = '\x04';
        DAT_200003a1 = '\x04';
      }
    }
LAB_0800422e:
    if (bVar2 == 10) {
      FUN_080001b0(_DAT_20000358,10);
      if (extraout_r1_10 < 5) {
        DAT_200003a4 = cVar3 + '\x02';
        DAT_200003a3 = cVar4 + '\x02';
        DAT_200003a5 = 6;
        goto LAB_08004344;
      }
      goto LAB_08004234;
    }
    if (bVar2 < 0xb) goto LAB_08004234;
    DAT_200003a5 = 4;
    DAT_200003a3 = cVar4;
    DAT_200003a4 = cVar3;
  }
  else {
    DAT_20000387 = 4;
    DAT_20000385 = cVar4;
    DAT_20000386 = cVar3;
LAB_08003b08:
    if (bVar2 != 1) {
      if (bVar2 < 2) goto LAB_08003b0e;
      DAT_2000038a = 4;
      DAT_20000388 = cVar4;
      DAT_20000389 = cVar3;
      goto LAB_08003bd0;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_01) goto LAB_08003b0e;
    DAT_20000389 = cVar3 + '\x02';
    DAT_20000388 = cVar4 + '\x02';
    DAT_2000038a = 6;
LAB_08003bd6:
    if (uVar5 == 2) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 1) {
        DAT_2000038d = 0xc;
        DAT_2000038b = '\f';
        DAT_2000038c = '\f';
      }
      else if (uVar6 == 0) {
        DAT_2000038d = 7;
        DAT_2000038b = '\a';
        DAT_2000038c = '\a';
      }
      else if (uVar6 == 0xffffffff) {
        DAT_2000038d = 5;
        DAT_2000038b = '\x05';
        DAT_2000038c = '\x05';
      }
      else {
        DAT_2000038d = 4;
        DAT_2000038b = '\x04';
        DAT_2000038c = '\x04';
      }
    }
LAB_08003c9a:
    if (bVar2 != 3) {
      if (bVar2 < 4) goto LAB_08003ca0;
      DAT_20000390 = 4;
      DAT_2000038e = cVar4;
      DAT_2000038f = cVar3;
      goto LAB_08003d6c;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_03) goto LAB_08003ca0;
    DAT_2000038f = cVar3 + '\x02';
    DAT_2000038e = cVar4 + '\x02';
    DAT_20000390 = 6;
LAB_08003d72:
    if (uVar5 == 4) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 3) {
        DAT_20000393 = 0xc;
        DAT_20000391 = '\f';
        DAT_20000392 = '\f';
      }
      else if (uVar6 == 2) {
        DAT_20000393 = 7;
        DAT_20000391 = '\a';
        DAT_20000392 = '\a';
      }
      else if (uVar6 == 1) {
        DAT_20000393 = 5;
        DAT_20000391 = '\x05';
        DAT_20000392 = '\x05';
      }
      else {
        DAT_20000393 = 4;
        DAT_20000391 = '\x04';
        DAT_20000392 = '\x04';
      }
    }
LAB_08003e36:
    if (bVar2 != 5) {
      if (bVar2 < 6) goto LAB_08003e3c;
      DAT_20000396 = 4;
      DAT_20000394 = cVar4;
      DAT_20000395 = cVar3;
      goto LAB_08003f00;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_05) goto LAB_08003e3c;
    DAT_20000395 = cVar3 + '\x02';
    DAT_20000394 = cVar4 + '\x02';
    DAT_20000396 = 6;
LAB_08003f06:
    if (uVar5 == 6) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 5) {
        DAT_20000399 = 0xc;
        DAT_20000397 = '\f';
        DAT_20000398 = '\f';
      }
      else if (uVar6 == 4) {
        DAT_20000399 = 7;
        DAT_20000397 = '\a';
        DAT_20000398 = '\a';
      }
      else if (uVar6 == 3) {
        DAT_20000399 = 5;
        DAT_20000397 = '\x05';
        DAT_20000398 = '\x05';
      }
      else {
        DAT_20000399 = 4;
        DAT_20000397 = '\x04';
        DAT_20000398 = '\x04';
      }
    }
LAB_08003fca:
    if (bVar2 != 7) {
      if (bVar2 < 8) goto LAB_08003fd0;
      DAT_2000039c = 4;
      DAT_2000039a = cVar4;
      DAT_2000039b = cVar3;
      goto LAB_0800409a;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_07) goto LAB_08003fd0;
    DAT_2000039b = cVar3 + '\x02';
    DAT_2000039a = cVar4 + '\x02';
    DAT_2000039c = 6;
LAB_080040a0:
    if (uVar5 == 8) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 7) {
        DAT_2000039f = 0xc;
        DAT_2000039d = '\f';
        DAT_2000039e = '\f';
      }
      else if (uVar6 == 6) {
        DAT_2000039f = 7;
        DAT_2000039d = '\a';
        DAT_2000039e = '\a';
      }
      else if (uVar6 == 5) {
        DAT_2000039f = 5;
        DAT_2000039d = '\x05';
        DAT_2000039e = '\x05';
      }
      else {
        DAT_2000039f = 4;
        DAT_2000039d = '\x04';
        DAT_2000039e = '\x04';
      }
    }
LAB_08004164:
    if (bVar2 != 9) {
      if (bVar2 < 10) goto LAB_0800416a;
      DAT_200003a2 = 4;
      DAT_200003a0 = cVar4;
      DAT_200003a1 = cVar3;
      goto LAB_0800422e;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_09) goto LAB_0800416a;
    DAT_200003a1 = cVar3 + '\x02';
    DAT_200003a0 = cVar4 + '\x02';
    DAT_200003a2 = 6;
LAB_08004234:
    if (uVar5 == 10) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 9) {
        DAT_200003a5 = 0xc;
        DAT_200003a3 = '\f';
        DAT_200003a4 = '\f';
      }
      else if (uVar6 == 8) {
        DAT_200003a5 = 7;
        DAT_200003a3 = '\a';
        DAT_200003a4 = '\a';
      }
      else if (uVar6 == 7) {
        DAT_200003a5 = 5;
        DAT_200003a3 = '\x05';
        DAT_200003a4 = '\x05';
      }
      else {
        DAT_200003a5 = 4;
        DAT_200003a3 = '\x04';
        DAT_200003a4 = '\x04';
      }
    }
  }
  if (bVar2 == 0xb) {
    FUN_080001b0(_DAT_20000358,10);
    if (extraout_r1_11 < 5) {
      DAT_200003a6 = cVar4 + '\x02';
      DAT_200003a7 = cVar3 + '\x02';
      DAT_200003a8 = 6;
      return 1;
    }
  }
  else if (0xb < bVar2) {
    DAT_200003a6 = cVar4;
    DAT_200003a7 = cVar3;
    DAT_200003a8 = 4;
    return 1;
  }
LAB_08004344:
  if (uVar5 == 0xb) {
    iVar1 = (extraout_r1 & 0xff) * 3;
    (&DAT_20000387)[iVar1] = 6;
    (&DAT_20000385)[iVar1] = 6;
    (&DAT_20000386)[iVar1] = 6;
  }
  else {
    uVar5 = extraout_r1 & 0xff;
    if (uVar5 == 10) {
      DAT_200003a8 = 0xc;
      DAT_200003a6 = 0xc;
      DAT_200003a7 = 0xc;
    }
    else if (uVar5 == 9) {
      DAT_200003a8 = 7;
      DAT_200003a6 = 7;
      DAT_200003a7 = 7;
    }
    else if (uVar5 == 8) {
      DAT_200003a8 = 5;
      DAT_200003a6 = 5;
      DAT_200003a7 = 5;
    }
    else {
      DAT_200003a8 = 4;
      DAT_200003a6 = 4;
      DAT_200003a7 = 4;
    }
  }
  return 1;
}


