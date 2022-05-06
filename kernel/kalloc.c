// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  // 物理地址最大是 PHYSTOP
  int refcount[PA2PID(PHYSTOP) + 1]; 
} pagerefcount;
int getrefcount(int id)
{
  acquire(&pagerefcount.lock);
  int t = pagerefcount.refcount[id];
  release(&pagerefcount.lock);
  return t;
}
int P(int id) 
{
  acquire(&pagerefcount.lock);
  if(id < 0 || id > PA2PID(PHYSTOP)) panic("P: id");
  int t = --pagerefcount.refcount[id];
  release(&pagerefcount.lock);
  return t;
}

int V(int id)
{
  acquire(&pagerefcount.lock);
  if(id < 0 || id > PA2PID(PHYSTOP)) panic("V: id");
  int t = ++pagerefcount.refcount[id];
  release(&pagerefcount.lock);
  return t;
}


void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&pagerefcount.lock, "cow");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(int i = 0; i < sizeof pagerefcount.refcount / sizeof(int); ++ i)
    pagerefcount.refcount[i] = 1;
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE) 
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");
  // 注意，此时 kfree 的意义已经变成：减少 pa 的引用计数refc，当 refc 是 0 的时候才释放
  int refcount = P(PA2PID(pa));
  if(refcount < 0) {
    printf("refcount : %d\n", refcount);
    panic("kfree");
  }
  if(refcount > 0) return; 
  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r) {
    kmem.freelist = r->next;
    // int t = V(PA2PID(r));
    // if(t != 1) {
    //   printf("refcount : %d\n", t);
    //   panic("kalloc V t");
    // }
    // 这里可以不用上锁，做一步优化，因为这个新内存必然是只有一个进程使用
    pagerefcount.refcount[PA2PID(r)] = 1;
  }
  release(&kmem.lock);
  
  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
