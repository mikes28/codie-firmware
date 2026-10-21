/* ==== FUN_080043D4 @ 0x80043d4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080043D4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = _DAT_20000364;
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 10;
    _DAT_20000364 = 0;
    cVar1 = '\0';
    FUN_08000128(0,100,param_3,0,param_4);
    iVar2 = -1;
LAB_0800441e:
    DAT_20000387 = '\0';
LAB_0800445e:
    DAT_20000385 = DAT_20000387;
    DAT_20000386 = DAT_20000387;
    if (iVar2 != 1) {
      if (iVar2 < 2) goto LAB_08004464;
      DAT_2000038a = cVar1 + '\b';
      goto LAB_080044a4;
    }
    DAT_20000389 = cVar1 + '\f';
    DAT_20000388 = cVar1 + '\x03';
    DAT_2000038a = cVar1 + '\x02';
LAB_080044aa:
    DAT_2000038d = '\0';
LAB_080044ea:
    DAT_2000038b = DAT_2000038d;
    DAT_2000038c = DAT_2000038d;
    if (iVar2 != 3) {
      if (iVar2 < 4) goto LAB_080044f0;
      DAT_20000390 = cVar1 + '\b';
      goto LAB_08004530;
    }
    DAT_2000038f = cVar1 + '\f';
    DAT_2000038e = cVar1 + '\x03';
    DAT_20000390 = cVar1 + '\x02';
LAB_08004536:
    DAT_20000393 = '\0';
LAB_08004576:
    DAT_20000391 = DAT_20000393;
    DAT_20000392 = DAT_20000393;
    if (iVar2 != 5) {
      if (iVar2 < 6) goto LAB_0800457c;
      DAT_20000396 = cVar1 + '\b';
      goto LAB_080045bc;
    }
    DAT_20000395 = cVar1 + '\f';
    DAT_20000394 = cVar1 + '\x03';
    DAT_20000396 = cVar1 + '\x02';
LAB_080045c2:
    DAT_20000399 = '\0';
LAB_08004602:
    DAT_20000397 = DAT_20000399;
    DAT_20000398 = DAT_20000399;
    if (iVar2 != 7) {
      if (iVar2 < 8) goto LAB_08004608;
      DAT_2000039c = cVar1 + '\b';
      goto LAB_08004648;
    }
    DAT_2000039b = cVar1 + '\f';
    DAT_2000039a = cVar1 + '\x03';
    DAT_2000039c = cVar1 + '\x02';
LAB_0800464e:
    DAT_2000039f = '\0';
LAB_0800468e:
    DAT_2000039d = DAT_2000039f;
    DAT_2000039e = DAT_2000039f;
    if (iVar2 != 9) {
      if (iVar2 < 10) goto LAB_08004694;
      DAT_200003a2 = cVar1 + '\b';
      goto LAB_080046d4;
    }
    DAT_200003a1 = cVar1 + '\f';
    DAT_200003a0 = cVar1 + '\x03';
    DAT_200003a2 = cVar1 + '\x02';
  }
  else {
    cVar1 = FUN_08000128(_DAT_20000364 * 0xf,100,_DAT_20000364 * 0x10,&DAT_20000358,param_4);
    uVar3 = FUN_08000128(iVar2 * 0xc,100);
    iVar2 = (uVar3 & 0xff) - 1;
    if (iVar2 != 0) {
      if (iVar2 < 1) goto LAB_0800441e;
      DAT_20000387 = cVar1 + '\b';
      goto LAB_0800445e;
    }
    DAT_20000386 = cVar1 + '\f';
    DAT_20000385 = cVar1 + '\x03';
    DAT_20000387 = cVar1 + '\x02';
LAB_08004464:
    DAT_2000038a = '\0';
LAB_080044a4:
    DAT_20000388 = DAT_2000038a;
    DAT_20000389 = DAT_2000038a;
    if (iVar2 != 2) {
      if (iVar2 < 3) goto LAB_080044aa;
      DAT_2000038d = cVar1 + '\b';
      goto LAB_080044ea;
    }
    DAT_2000038c = cVar1 + '\f';
    DAT_2000038b = cVar1 + '\x03';
    DAT_2000038d = cVar1 + '\x02';
LAB_080044f0:
    DAT_20000390 = '\0';
LAB_08004530:
    DAT_2000038e = DAT_20000390;
    DAT_2000038f = DAT_20000390;
    if (iVar2 != 4) {
      if (iVar2 < 5) goto LAB_08004536;
      DAT_20000393 = cVar1 + '\b';
      goto LAB_08004576;
    }
    DAT_20000392 = cVar1 + '\f';
    DAT_20000391 = cVar1 + '\x03';
    DAT_20000393 = cVar1 + '\x02';
LAB_0800457c:
    DAT_20000396 = '\0';
LAB_080045bc:
    DAT_20000394 = DAT_20000396;
    DAT_20000395 = DAT_20000396;
    if (iVar2 != 6) {
      if (iVar2 < 7) goto LAB_080045c2;
      DAT_20000399 = cVar1 + '\b';
      goto LAB_08004602;
    }
    DAT_20000398 = cVar1 + '\f';
    DAT_20000397 = cVar1 + '\x03';
    DAT_20000399 = cVar1 + '\x02';
LAB_08004608:
    DAT_2000039c = '\0';
LAB_08004648:
    DAT_2000039a = DAT_2000039c;
    DAT_2000039b = DAT_2000039c;
    if (iVar2 != 8) {
      if (iVar2 < 9) goto LAB_0800464e;
      DAT_2000039f = cVar1 + '\b';
      goto LAB_0800468e;
    }
    DAT_2000039e = cVar1 + '\f';
    DAT_2000039d = cVar1 + '\x03';
    DAT_2000039f = cVar1 + '\x02';
LAB_08004694:
    DAT_200003a2 = '\0';
LAB_080046d4:
    if (iVar2 == 10) {
      DAT_200003a0 = DAT_200003a2;
      DAT_200003a1 = DAT_200003a2;
      DAT_200003a3 = cVar1 + '\x03';
      DAT_200003a4 = cVar1 + '\f';
      DAT_200003a5 = cVar1 + '\x02';
      DAT_200003a6 = 0;
      DAT_200003a7 = 0;
      DAT_200003a8 = 0;
      return 1;
    }
    DAT_200003a0 = DAT_200003a2;
    DAT_200003a1 = DAT_200003a2;
    if (10 < iVar2) {
      DAT_200003a5 = cVar1 + '\b';
      goto LAB_08004720;
    }
  }
  DAT_200003a5 = '\0';
LAB_08004720:
  if (iVar2 == 0xb) {
    DAT_200003a3 = DAT_200003a5;
    DAT_200003a4 = DAT_200003a5;
    DAT_200003a6 = cVar1 + '\x03';
    DAT_200003a7 = cVar1 + '\f';
    DAT_200003a8 = cVar1 + '\x02';
    return 1;
  }
  if (iVar2 < 0xc) {
    DAT_200003a8 = 0;
    DAT_200003a7 = 0;
    DAT_200003a6 = 0;
    DAT_200003a4 = DAT_200003a5;
    DAT_200003a3 = DAT_200003a5;
    return 1;
  }
  DAT_200003a6 = cVar1 + '\b';
  DAT_200003a3 = DAT_200003a5;
  DAT_200003a4 = DAT_200003a5;
  DAT_200003a7 = DAT_200003a6;
  DAT_200003a8 = DAT_200003a6;
  return 1;
}


