
#pragma once

void icache_init (void);
void dcache_init (void);
void dcache_invalidate_way (void);
void dcache_invalidate_addr(unsigned int addr, unsigned int size);
void CacheInit();
void CacheEnable();
void CacheDisable();

