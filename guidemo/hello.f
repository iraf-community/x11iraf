      subroutine thello ()
      logical Memb(1)
      integer*2 Memc(1)
      integer*2 Mems(1)
      integer Memi(1)
      integer*4 Meml(1)
      real Memr(1)
      double precision Memd(1)
      complex Memx(1)
      equivalence (Memb, Memc, Mems, Memi, Meml, Memr, Memd, Memx)
      common /Mem/ Memd
      integer gp
      real x
      real y
      integer wcs
      integer key
      integer*2 strval(161 +1)
      integer*2 device(63 +1)
      integer*2 guifie(63 +1)
      integer clgcur
      integer gopeni
      integer*2 st0001(7)
      integer*2 st0002(4)
      integer*2 st0003(7)
      integer*2 st0004(16)
      integer*2 st0005(13)
      save
      integer iyy
      data st0001 /100,101,118,105, 99,101, 0/
      data st0002 /103,117,105, 0/
      data st0003 / 99,111,111,114,100,115, 0/
      data (st0004(iyy),iyy= 1, 8) / 37,103, 32, 37,103, 32, 37,100/
      data (st0004(iyy),iyy= 9,16) / 32, 37, 99, 32, 37,115, 10, 0/
      data (st0005(iyy),iyy= 1, 8) / 37,103, 32, 37,103, 32, 37,100/
      data (st0005(iyy),iyy= 9,13) / 32, 37, 99, 10, 0/
         call clgstr (st0001, device, 63 )
         call clgstr (st0002, guifie, 63 )
         gp = gopeni (device, 5, guifie, 6)
110      if (.not.(clgcur (st0003, x, y, wcs, key, strval, 161 ) .ne. -2
     *   )) goto 111
            if (.not.(key .eq. 113 .or. key .eq. 81)) goto 120
               goto 111
120         continue
               if (.not.(key .eq. 58)) goto 130
                  call xprinf(st0004)
                  call pargr (x)
                  call pargr (y)
                  call pargi (wcs)
                  call pargi (key)
                  call pargsr (strval)
                  goto 131
130            continue
                  call xprinf(st0005)
                  call pargr (x)
                  call pargr (y)
                  call pargi (wcs)
                  call pargi (key)
131            continue
121         continue
            goto 110
111      continue
         call gclose (gp)
100      return
      end
c     guifie  guifile
c     thello  t_hello
c     gopeni  gopenui
c     pargsr  pargstr
