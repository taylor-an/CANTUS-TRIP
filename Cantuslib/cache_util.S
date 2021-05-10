
/* void icache_invalidate_way(void)*/
	.section .text
    .global _icache_invalidate_way
	.type _icache_invalidate_way, @function
_icache_invalidate_way:	
/*cache disable*/
	push	%r0
	ldi		0x0,%r0
	mvtc 	0,%r9
/*waybase invalidation	writeback*/
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync
	
/*cache on	*/
	ldi		0x33,%r0
	mvtc 	0,%r9
	sync
	
	pop		%r0
	jplr
.size _icache_invalidate_way, .-_icache_invalidate_way	


/* void dcache_invalidate_way(void)*/
	.section .text
    .global _dcache_invalidate_way
	.type _dcache_invalidate_way, @function
_dcache_invalidate_way:	
/*cache disable*/
	push	%r0
	ldi		0x0,%r0
	mvtc 	0,%r9
/*waybase invalidation	writeback*/
	ldi		0xd,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1d,%r0
	mvtc	0,%R11
	sync
	
/*cache on	*/
	ldi		0x33,%r0
	mvtc 	0,%r9
	sync
	
	pop		%r0
	jplr
.size _dcache_invalidate_way, .-_dcache_invalidate_way	
	
/*	void CacheDisable (void)*/
	.global _CacheDisable
	.type _CacheDisable, @function
_CacheDisable:
/*cache disable*/
	push	%r0
	ldi		0x0,%r0
	mvtc 	0,%r9
/*i-cache waybase invalidation	 writeback*/
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync

/*d-cache waybase invalidation	writeback*/
	ldi		0xd,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1d,%r0
	mvtc	0,%R11
	sync
	
	pop		%r0
	jplr

.size _CacheDisable, .-_CacheDisable	

/*	void CacheEnable (void)*/
	.global _CacheEnable
	.type _CacheEnable, @function
_CacheEnable:
	push	%r0
	ldi		0x33,%r0
	mvtc 	0,%r9
	sync
	
	pop		%r0
	jplr

.size _CacheEnable, .-_CacheEnable	


/* void CacheInit(void)*/
	.section .text
    .global _CacheInit
	.type _CacheInit, @function
_CacheInit:	

	push %r0
/* if cache on */
	mvfc	0,%r9
	cmp		0,%r0
	jnz		._idinvalidation_copyback_:

._idinvalidation_:
/*i-cache waybase invalidation	*/	
	ldi		0x8,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x18,%r0
	mvtc	0,%R11
	sync
/*d-cache waybase invalidation*/	
	ldi		0x9,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x19,%r0
	mvtc	0,%R11
	sync

/*cache on	*/
	ldi		0x33,%r0
	mvtc 	0,%r9
	sync
	
	pop		%r0
	jplr

._idinvalidation_copyback_	:
/* cache off */
	ldi		0x0,%r0
	mvtc 	0,%r9
	sync
/*i-cache waybase invalidation	writeback*/	
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync
/*d-cache waybase invalidation	writeback*/	
	ldi		0xd,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1d,%r0
	mvtc	0,%R11
	sync
	
/*cache on	*/
	ldi		0x33,%r0
	mvtc 	0,%r9
	sync
	
	pop		%r0
	jplr
.size _CacheInit, .-_CacheInit	

