/* ==== FUN_08008AD0 @ 0x8008ad0 ==== */

void FUN_08008AD0(undefined4 param_1,int param_2)

{
  if ((short)-(short)param_1 < 1) {
    if ((short)-(short)param_1 < 0) {
      if (param_2 << 0x1f < 0) {
        DAT_200006fe = 0xff;
      }
      if (param_2 << 0x1e < 0) {
        DAT_200006fd = 0xff;
      }
    }
    else {
      if (param_2 << 0x1f < 0) {
        DAT_200006fe = 1;
      }
      if (param_2 << 0x1e < 0) {
        DAT_200006fd = 1;
      }
    }
  }
  else {
    if (param_2 << 0x1f < 0) {
      DAT_200006fe = 1;
    }
    if (param_2 << 0x1e < 0) {
      DAT_200006fd = 1;
    }
  }
  if (param_2 << 0x1f < 0) {
    FUN_08008A50();
  }
  if (param_2 << 0x1e < 0) {
    FUN_080089D0(param_1);
  }
  return;
}


