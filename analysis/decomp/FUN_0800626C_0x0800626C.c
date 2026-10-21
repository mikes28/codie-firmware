/* ==== FUN_0800626C @ 0x800626c ==== */

void FUN_0800626C(void)

{
  undefined4 in_r3;
  
  if (DAT_20000410 == '\x02') {
    DAT_20000410 = '\x03';
    FUN_08005EC4(4,0,3,0x60,in_r3);
    FUN_08006130();
  }
  return;
}


