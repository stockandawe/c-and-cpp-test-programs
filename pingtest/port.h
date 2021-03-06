/*
** Various portability definitions.
**
**	@(#)port.h              e07@nikhef.nl (Eric Wassenaar) 990511
*/

#if defined(__SVR4) || defined(__svr4__)
#define SVR4
#endif

#if defined(SYSV) || defined(SVR4)
#define SYSV_MALLOC
#define SYSV_MEMSET
#define SYSV_STRCHR
#define SYSV_SETVBUF
#endif

#if defined(__hpux) || defined(hpux)
#define SYSV_MALLOC
#define SYSV_SETVBUF
#endif

#if defined(sgi)
#define SYSV_MALLOC
#endif

#if defined(linux)
#define SYSV_MALLOC
#endif

#if defined(bsdi) || defined(__bsdi__)
#define SYSV_MALLOC
#endif

#if defined(NeXT)
#define SYSV_MALLOC
#endif

/*
** Distinguish between various BIND releases.
*/

#if defined(RES_PRF_STATS)
#define BIND_49
#else
#define BIND_48
#endif

#if defined(BIND_49) && defined(__BIND)
#define BIND_493
#endif

/*
** On some platforms a raw IP packet must be in network byte order.
** Most systems require certain fields in machine byte order,
** and perform the htons()/ntohs() conversion within the kernel.
** Some systems require everything to be in network order.
** Note that this is relevant only for little-endian machines.
*/

#if defined(sun) && defined(SVR4)
#define RAW_IP_NET_ORDER
#endif

#if defined(linux)
#define RAW_IP_NET_ORDER
#endif

/*
** Define constants for fixed sizes.
*/

#ifndef INT16SZ
#define	INT16SZ		2	/* for systems without 16-bit ints */
#endif

#ifndef INT32SZ
#define	INT32SZ		4	/* for systems without 32-bit ints */
#endif

#ifndef INADDRSZ
#define	INADDRSZ	4	/* for sizeof(struct inaddr) != 4 */
#endif

/*
** The following should depend on existing definitions.
*/

typedef int	bool;		/* boolean type */
#define TRUE	1
#define FALSE	0

#if defined(BIND_48) || defined(OLD_RES_STATE)
typedef struct state		res_state_t;
#else
typedef struct __res_state	res_state_t;
#endif

#ifndef _IPADDR_T
#if defined(__alpha) || defined(BIND_49)
typedef u_int	ipaddr_t;
#else
typedef u_long	ipaddr_t;
#endif
#endif

#if defined(apollo) || defined(_BSD_SIGNALS)
typedef int	sigtype_t;
#define sig_return(n)	return(n)
#else
typedef void	sigtype_t;
#define sig_return(n)	return
#endif

#ifdef SYSV_MALLOC
typedef void	ptr_t;		/* generic pointer type */
typedef u_int	siz_t;		/* general size type */
typedef void	free_t;
#else
typedef char	ptr_t;		/* generic pointer type */
typedef u_int	siz_t;		/* general size type */
typedef int	free_t;
#endif

#ifdef SYSV_MEMSET
#define bzero(a,n)	(void) memset(a,'\0',n)
#define bcopy(a,b,n)	(void) memcpy(b,a,n)
#define bcmp(a,b,n)	memcmp(b,a,n)
#endif

#ifdef SYSV_STRCHR
#define index		strchr
#define rindex		strrchr
#endif

#ifdef SYSV_SETVBUF
#define linebufmode(a)	(void) setvbuf(a, (char *)NULL, _IOLBF, BUFSIZ);
#else
#define linebufmode(a)	(void) setlinebuf(a);
#endif

#if defined(SVR4)
#define jmp_buf		sigjmp_buf
#define setjmp(e)	sigsetjmp(e,1)
#define longjmp(e,n)	siglongjmp(e,n)
#endif

#if defined(sun) && defined(NO_YP_LOOKUP)
#define gethostbyname	(struct hostent *)res_gethostbyname
#define gethostbyaddr	(struct hostent *)res_gethostbyaddr
#endif

/*
** No prototypes yet.
*/

#define PROTO(TYPES)	()

#if !defined(__STDC__) || defined(apollo)
#define Proto(TYPES)	()
#else
#define Proto(TYPES)	TYPES
#endif

#if !defined(__STDC__) || defined(apollo)
#define const
#endif

#if defined(__STDC__) && defined(BIND_49)
#define CONST	const
#else
#define CONST
#endif
