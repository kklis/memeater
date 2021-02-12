#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

typedef void (far *func)();

unsigned char check_xms_driver();
func get_xms_func();
void xms_info(func fn);
void xms_malloc(func fn, unsigned int kb);

int main(int argc, char *argv[]) {
  func fn;
  long int kb;

  if (check_xms_driver() != 0x80) {
    printf("XMS memory driver not found. Do you have HIMEM.SYS installed?");
    return 2;
  }
  fn = get_xms_func();
  if (argc != 2) {
    printf("Syntax: memeater.exe kilobytes\n");
    xms_info(fn);
    return 0;
  }
  kb = atol(argv[1]);
  if ((kb = atol(argv[1])) < 0) {
    printf("Wrong argument! Syntax: memeater.exe kilobytes\n");
    return 1;
  }
  if (kb > 0xFFFF) {
    while (kb > 0xFFFF) {
      xms_malloc(fn, 0xFFFF);
      kb -= 0xFFFF;
    }
  }
  xms_malloc(fn, kb & 0xFFFF);
  xms_info(fn);
  return 0;
}

unsigned char check_xms_driver() {
  _AX = 0x4300;
  geninterrupt(0x2F);
  return _AL;
}

func get_xms_func() {
  func fn;

  _AX = 0x4310;
  geninterrupt(0x2F);
  /* Make far function pointer from segment and offset */
  fn = (void (far *)(void)) MK_FP(_ES, _BX);
  return fn;
}

void xms_info(func fn) {
  struct {
    unsigned int total;
    unsigned int free;
  } mem;

  _AH = 8;
  (*fn)();
  mem.total = _DX;
  mem.free = _AX;
  printf("Total amount of free extended memory: %u kilobytes\n", mem.total);
  printf("Largest free extended memory block: %u kilobytes\n", mem.free);
}

void xms_malloc(func fn, unsigned int kb) {
  _AH = 9;
  _DX = kb;
  (*fn)();
}
