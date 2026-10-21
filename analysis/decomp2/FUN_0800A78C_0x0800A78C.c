/* ==== FUN_0800A78C @ 0x800a78c ==== */

bool FUN_0800A78C(uint *param_1,uint param_2)

{
  uint uVar1;
  
  if ((int)(param_2 << 7) < 0) {
    uVar1 = param_1[2] & 0xfeffffff;
  }
  else {
    uVar1 = *param_1;
  }
  return (param_2 & uVar1) != 0;
}


