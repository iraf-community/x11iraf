      subroutine timbre ()
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
      integer ip
      integer op
      integer*2 direcy(127 +1)
      integer*2 buf(161 +1)
      integer*2 cmd(63 +1)
      integer*2 args(161 +1)
      integer*2 device(63 +1)
      integer*2 uifnae(127 +1)
      integer*2 temple(161 +1)
      integer*2 sectin(161 +1)
      integer*2 strval(161 +1)
      integer*2 curdir(127 +1)
      logical streq
      integer gopeni
      integer clgcur
      integer clgeti
      integer imbisy
      integer sw0001
      integer*2 st0001(7)
      integer*2 st0002(10)
      integer*2 st0003(7)
      integer*2 st0004(8)
      integer*2 st0005(9)
      integer*2 st0006(8)
      integer*2 st0007(9)
      integer*2 st0008(8)
      integer*2 st0009(2)
      integer*2 st0010(7)
      integer*2 st0011(6)
      integer*2 st0012(9)
      integer*2 st0013(2)
      integer*2 st0014(8)
      integer*2 st0015(7)
      integer*2 st0016(8)
      integer*2 st0017(28)
      integer*2 st0018(9)
      integer*2 st0019(49)
      integer*2 st0020(9)
      save
      integer iyy
      data st0001 / 36,110, 97,114,103,115, 0/
      data (st0002(iyy),iyy= 1, 8) /100,105,114,101, 99,116,111,114/
      data (st0002(iyy),iyy= 9,10) /121, 0/
      data st0003 /100,101,118,105, 99,101, 0/
      data st0004 /117,105,102,110, 97,109,101, 0/
      data (st0005(iyy),iyy= 1, 8) /116,101,109,112,108, 97,116,101/
      data (st0005(iyy),iyy= 9, 9) / 0/
      data st0006 /115,101, 99,116,105,111,110, 0/
      data (st0007(iyy),iyy= 1, 8) /116,101,109,112,108, 97,116,101/
      data (st0007(iyy),iyy= 9, 9) / 0/
      data st0008 /115,101, 99,116,105,111,110, 0/
      data st0009 / 46, 0/
      data st0010 / 99,111,111,114,100,115, 0/
      data st0011 / 99,104,100,105,114, 0/
      data (st0012(iyy),iyy= 1, 8) /116,101,109,112,108, 97,116,101/
      data (st0012(iyy),iyy= 9, 9) / 0/
      data st0013 / 46, 0/
      data st0014 /115,101, 99,116,105,111,110, 0/
      data st0015 /104,101, 97,100,101,114, 0/
      data st0016 /100,105,115,112,108, 97,121, 0/
      data (st0017(iyy),iyy= 1, 8) /117,110,114,101, 99,111,103,110/
      data (st0017(iyy),iyy= 9,16) /105,122,101,100, 32, 99,111,109/
      data (st0017(iyy),iyy=17,24) /109, 97,110,100, 58, 32, 96, 37/
      data (st0017(iyy),iyy=25,28) /115, 39, 10, 0/
      data (st0018(iyy),iyy= 1, 8) /101,114,114,111,114,109,115,103/
      data (st0018(iyy),iyy= 9, 9) / 0/
      data (st0019(iyy),iyy= 1, 8) /117,110,114,101, 99,111,103,110/
      data (st0019(iyy),iyy= 9,16) /105,122,101,100, 32, 99,117,114/
      data (st0019(iyy),iyy=17,24) /115,111,114, 32, 99,111,109,109/
      data (st0019(iyy),iyy=25,32) / 97,110,100, 58, 32,107,101,121/
      data (st0019(iyy),iyy=33,40) / 61, 37, 99, 32,115,116,114,118/
      data (st0019(iyy),iyy=41,48) / 97,108, 61, 96, 37,115, 39, 10/
      data (st0019(iyy),iyy=49,49) / 0/
      data (st0020(iyy),iyy= 1, 8) /101,114,114,111,114,109,115,103/
      data (st0020(iyy),iyy= 9, 9) / 0/
         if (.not.(clgeti (st0001) .gt. 0)) goto 110
            call clgstr (st0002, direcy, 127 )
            if (.not.(imbisy (direcy, curdir, 127 ) .gt. 0)) goto 120
               call xstrcy(curdir, direcy, 127 )
               goto 121
120         continue
               direcy(1) = 0
121         continue
            goto 111
110      continue
            direcy(1) = 0
111      continue
         call clgstr (st0003, device, 63 )
         call clgstr (st0004, uifnae, 63 )
         call clgstr (st0005, temple, 161 )
         call clgstr (st0006, sectin, 161 )
         gp = gopeni (device, 5, uifnae, 6)
         call gmsg (gp, st0007, temple)
         call gmsg (gp, st0008, sectin)
         call fpathe (direcy, curdir, 127 )
         call imbser (gp, curdir, st0009, temple)
130      if (.not.(clgcur (st0010, x, y, wcs, key, strval, 161 ) .ne. -2
     *   )) goto 131
            sw0001=(key)
            goto 140
150         continue
               goto 131
160         continue
               ip=1
170            if (.not.((strval(ip).eq.32.or.strval(ip).eq.9))) goto 
     *         172
171               ip=ip+1
                  goto 170
172            continue
               op = 1
180            if (.not.(strval(ip) .ne. 0 .and. .not.(strval(ip).eq.32.
     *         or.strval(ip).eq.9))) goto 181
                  cmd(op) = strval(ip)
                  op = op + 1
                  ip = ip + 1
                  goto 180
181            continue
               cmd(op) = 0
190            if (.not.((strval(ip).eq.32.or.strval(ip).eq.9))) goto 
     *         192
191               ip=ip+1
                  goto 190
192            continue
               call xstrcy(strval(ip), args, 161 )
               if (.not.(streq (cmd, st0011))) goto 200
                  call imbser (gp, curdir, args, temple)
                  goto 201
200            continue
               if (.not.(streq (cmd, st0012))) goto 210
                  call xstrcy(args, temple, 161 )
                  call imbser (gp, curdir, st0013, temple)
                  goto 211
210            continue
               if (.not.(streq (cmd, st0014))) goto 220
                  call xstrcy(args, sectin, 161 )
                  goto 221
220            continue
               if (.not.(streq (cmd, st0015))) goto 230
                  call imbphr (gp, curdir, args, sectin)
                  goto 231
230            continue
               if (.not.(streq (cmd, st0016))) goto 240
                  call imbdiy (gp, curdir, args, sectin)
                  goto 241
240            continue
                  call sprinf (buf, 161 , st0017)
                  call pargsr (strval)
                  call gmsg (gp, st0018, buf)
241            continue
231            continue
221            continue
211            continue
201            continue
            goto 141
250         continue
               call sprinf (buf, 161 , st0019)
               call pargi (key)
               call pargsr (strval)
               call gmsg (gp, st0020, buf)
               goto 141
140         continue
               if (sw0001.eq.58) goto 160
               if (sw0001.eq.81) goto 150
               if (sw0001.eq.113) goto 150
               goto 250
141         continue
            goto 130
131      continue
         call gclose (gp)
100      return
      end
      subroutine imbser (gp, curdir, newdir, temple)
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
      integer*2 curdir(*)
      integer*2 newdir(*)
      integer*2 temple(*)
      integer sop
      integer fop
      integer pt
      integer fd
      integer errcoe
      integer nchars
      integer sp
      integer lbuf
      integer fname
      integer subdis
      integer files
      integer dirpah
      integer ftemp
      integer ptcome
      integer errget
      integer gstrcy
      integer imbisy
      integer imbisr
      integer diropn
      integer nowhie
      integer getlie
      integer xfaccs
      integer ptmath
      logical xerpop
      logical xerflg
      common /xercom/ xerflg
      integer*2 st0001(10)
      integer*2 st0002(3)
      integer*2 st0003(8)
      integer*2 st0004(6)
      integer*2 st0005(9)
      save
      integer iyy
      data (st0001(iyy),iyy= 1, 8) /100,105,114,101, 99,116,111,114/
      data (st0001(iyy),iyy= 9,10) /121, 0/
      data st0002 / 47, 10, 0/
      data st0003 /115,117, 98,100,105,114,115, 0/
      data st0004 /102,105,108,101,115, 0/
      data (st0005(iyy),iyy= 1, 8) /101,114,114,111,114,109,115,103/
      data (st0005(iyy),iyy= 9, 9) / 0/
         call smark (sp)
         call salloc (lbuf, 161 , 2)
         call salloc (fname, 127 , 2)
         call salloc (ftemp, 127 , 2)
         call salloc (subdis, 1024 , 2)
         call salloc (files, 16384, 2)
         call salloc (dirpah, 127 , 2)
         sop = subdis
         fop = files
         call xerpsh
         if (.not.(imbisr (curdir, newdir) .eq. 1)) goto 120
            call xstrcy(curdir, memc(fname), 127 )
            call zfsubd (memc(fname), 127 , newdir, nchars)
            goto 121
120      continue
         if (.not.(imbisy(newdir,memc(dirpah),127 ) .gt. 0)) goto 130
            call fpathe (memc(dirpah), memc(fname), 127 )
            if (xerflg) goto 112
            goto 131
130      continue
            call fpathe (newdir, memc(fname), 127 )
            if (xerflg) goto 112
131      continue
121      continue
         fd = diropn (memc(fname), 1)
         if (xerflg) goto 112
         call xstrcy(memc(fname), curdir, 127 )
         call gmsg (gp, st0001, memc(fname))
         if (xerflg) goto 112
         pt = ptcome (temple)
         if (xerflg) goto 112
         sop = sop + gstrcy (st0002, memc(sop), 999999999)
140      if (.not.(getlie (fd, memc(lbuf)) .ne. -2).and.(.not.xerflg)) 
     *   goto 141
         if (xerflg) goto 112
            if (.not.(nowhie (memc(lbuf), memc(ftemp), 127 ) .le. 0)) 
     *      goto 150
               goto 140
150         continue
            call imbmke (memc(ftemp), curdir, memc(fname), 127 )
            if (.not.(xfaccs(memc(fname), 0, 13) .eq. 1).and.(.not.
     *      xerflg)) goto 160
            if (xerflg) goto 112
               sop = sop + gstrcy (memc(lbuf), memc(sop), 999999999)
160         continue
            if (.not.(ptmath (pt, memc(fname)) .eq. 1)) goto 170
               fop = fop + gstrcy (memc(lbuf), memc(fop), 999999999)
170         continue
            goto 140
141      continue
         call ptfree (pt)
         call xfcloe(fd)
112      if (.not.xerpop()) goto 110
            goto 91
110      continue
         memc(sop) = 0
         memc(fop) = 0
         call gmsg (gp, st0003, memc(subdis))
         if (xerflg) goto 100
         call gmsg (gp, st0004, memc(files))
         if (xerflg) goto 100
         call sfree (sp)
         goto 100
91       errcoe = errget (memc(lbuf), 161 )
         call gmsg (gp, st0005, memc(lbuf))
         if (xerflg) goto 100
         call sfree (sp)
100      return
      end
      subroutine imbphr (gp, curdir, image, sectin)
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
      integer*2 curdir(*)
      integer*2 image(*)
      integer*2 sectin(*)
      integer in
      integer minlea
      integer sp
      integer lbuf
      integer hbuf
      integer ip
      integer op
      integer im
      integer fname
      integer stropn
      integer getlie
      integer gstrcy
      integer immap
      logical xerpop
      logical xerflg
      common /xercom/ xerflg
      integer*2 st0001(23)
      integer*2 st0002(9)
      integer*2 st0003(12)
      integer*2 st0004(7)
      save
      integer iyy
      data (st0001(iyy),iyy= 1, 8) / 99, 97,110,110,111,116, 32,111/
      data (st0001(iyy),iyy= 9,16) /112,101,110, 32,105,109, 97,103/
      data (st0001(iyy),iyy=17,23) /101, 32, 96, 37,115, 39, 0/
      data (st0002(iyy),iyy= 1, 8) /101,114,114,111,114,109,115,103/
      data (st0002(iyy),iyy= 9, 9) / 0/
      data (st0003(iyy),iyy= 1, 8) /105,109, 97,103,101, 95,116,105/
      data (st0003(iyy),iyy= 9,12) /116,108,101, 0/
      data st0004 /104,101, 97,100,101,114, 0/
         call smark (sp)
         call salloc (lbuf, 161 , 2)
         call salloc (fname, 127 , 2)
         call imbmke (image, curdir, memc(fname), 127 )
         call xerpsh
         im = immap (memc(fname), 1 , 0)
         if (.not.xerpop()) goto 110
            call sprinf (memc(lbuf), 161 , st0001)
            call pargsr (image)
            call gmsg (gp, st0002, memc(lbuf))
            call sfree (sp)
            goto 100
110      continue
         call salloc (hbuf, memi(im+200 +3) , 2)
         op = hbuf
         minlea = (200 + memi(im+27) - 713 ) * 2 - 1
         in = stropn (memc((im+713 -1)*2 + 1), minlea, 1 )
120      if (.not.(getlie (in, memc(lbuf)) .ne. -2)) goto 121
            ip=lbuf
130         if (.not.(memc(ip) .ne. 0 .and. memc(ip) .ne. 10)) goto 132
131            ip=ip+1
               goto 130
132         continue
140         if (.not.(ip .gt. lbuf .and. memc(ip-1) .eq. 32)) goto 141
               ip = ip - 1
               goto 140
141         continue
            memc(ip) = 10
            memc(ip+1) = 0
            op = op + gstrcy (memc(lbuf), memc(op), 999999999)
            goto 120
121      continue
         call gmsg (gp, st0003, memc((((im+200 +183)-1)*2+1)) )
         call gmsg (gp, st0004, memc(hbuf))
         call xfcloe(in)
         call imunmp (im)
         call sfree (sp)
100      return
      end
      subroutine imbdiy (gp, curdir, image, sectin)
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
      integer*2 curdir(*)
      integer*2 image(*)
      integer*2 sectin(*)
      integer ncols
      integer nrows
      integer i
      integer v
      real contrt
      real z1
      real z2
      real dz1
      real dz2
      integer r(200)
      integer g(200)
      integer b(200)
      integer nsamps
      integer lenste
      integer j1
      integer j2
      integer npix
      integer sp
      integer im
      integer fname
      integer lbuf
      integer in
      integer out
      integer pkras
      integer clgeti
      real clgetr
      integer immap
      integer imgs2r
      logical xerpop
      logical xerflg
      common /xercom/ xerflg
      integer*2 st0001(25)
      integer*2 st0002(9)
      integer*2 st0003(12)
      integer*2 st0004(9)
      integer*2 st0005(14)
      save
      integer iyy
      data (st0001(iyy),iyy= 1, 8) / 99, 97,110,110,111,116, 32,111/
      data (st0001(iyy),iyy= 9,16) /112,101,110, 32,105,109, 97,103/
      data (st0001(iyy),iyy=17,24) /101, 32, 96, 37,115, 37,115, 39/
      data (st0001(iyy),iyy=25,25) / 0/
      data (st0002(iyy),iyy= 1, 8) /101,114,114,111,114,109,115,103/
      data (st0002(iyy),iyy= 9, 9) / 0/
      data (st0003(iyy),iyy= 1, 8) /105,109, 97,103,101, 95,116,105/
      data (st0003(iyy),iyy= 9,12) /116,108,101, 0/
      data (st0004(iyy),iyy= 1, 8) / 99,111,110,116,114, 97,115,116/
      data (st0004(iyy),iyy= 9, 9) / 0/
      data (st0005(iyy),iyy= 1, 8) /110,115, 97,109,112,108,101, 95/
      data (st0005(iyy),iyy= 9,14) /108,105,110,101,115, 0/
         call smark (sp)
         call salloc (lbuf, 161 , 2)
         call salloc (fname, 127 , 2)
         call imbmke (image, curdir, memc(fname), 127 )
         call xstrct(sectin, memc(fname), 127 )
         call xerpsh
         im = immap (memc(fname), 1 , 0)
         if (.not.xerpop()) goto 110
            call sprinf (memc(lbuf), 161 , st0001)
            call pargsr (image)
            call pargsr (sectin)
            call gmsg (gp, st0002, memc(lbuf))
            call sfree (sp)
            goto 100
110      continue
         ncols = meml(im+200 +1+6-1)
         nrows = meml(im+200 +2+6-1)
         call gmsg (gp, st0003, memc((((im+200 +183)-1)*2+1)) )
         call gimcrr (gp, 1, 0, ncols, nrows, 8)
         if (xerflg) goto 100
         call gseti (gp, 43 , 1)
         if (xerflg) goto 100
         call gswind (gp, 0.5, real(ncols) + 0.5, 0.5, real(nrows) + 0.5
     *   )
         if (xerflg) goto 100
         call gimseg (gp, 1, 0, 1, 0 , 0.0, 0.0, real(ncols), real(nrows
     *   ), 0, 1 , 0.0, 0.0, 1.0, 1.0)
         if (xerflg) goto 100
         do 120 i = 1, 200 
            v = 255 * (real((i - 1)) / (200 - 1))
            r(i) = v
            g(i) = v
            b(i) = v
120      continue
121      continue
         call gimwrp (gp, 0, 9+1, 200, r, g, b)
         if (xerflg) goto 100
         call gimwrp (gp, 1, 9+1, 200, r, g, b)
         if (xerflg) goto 100
         contrt = clgetr (st0004)
         if (xerflg) goto 100
         nsamps = clgeti (st0005)
         if (xerflg) goto 100
         lenste = 600 / nsamps
         call zscale (im, z1, z2, contrt, 600, lenste)
         if (xerflg) goto 100
         dz1 = 9 + 1
         dz2 = dz1 + 200 - 1
         call xmallc(out, ncols * 32, 6)
         if (xerflg) goto 100
         call xmallc(pkras, ncols * 32, 2)
         if (xerflg) goto 100
         do 130 j1 = 1, nrows, 32 
            j2 = min (j1 + 32 - 1, nrows)
            npix = (j2 - j1 + 1) * ncols
            in = imgs2r (im, 1, ncols, j1, j2)
            if (xerflg) goto 100
            call amapr (memr(in), memr(out), npix, z1, z2, dz1, dz2)
            call achtrb (memr(out), memc(pkras), npix)
            call gimwrs (gp, 1, memc(pkras), 8, 0, j1-1, ncols, j2-j1+1)
            if (xerflg) goto 100
130      continue
131      continue
         call xmfree(out, 6)
         call xmfree(pkras, 2)
         call imunmp (im)
         call sfree (sp)
100      return
      end
      subroutine imbmke (file, direcy, fname, maxch)
      integer maxch
      integer*2 file(*)
      integer*2 direcy(*)
      integer*2 fname(*)
      save
         call fdirne (direcy, fname, maxch)
         call xstrct(file, fname, maxch)
100      return
      end
      integer function imbisy (vfn, pathne, maxch)
      integer maxch
      integer*2 vfn(*)
      integer*2 pathne(*)
      logical isdir
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
      integer sp
      integer fname
      integer op
      integer ip
      integer fd
      integer nchars
      integer ch
      integer*4 fileio((6+16) )
      integer finfo
      integer diropn
      integer gstrcy
      integer xstrln
      logical streq
      logical xerpop
      logical xerflg
      common /xercom/ xerflg
      integer*2 st0001(2)
      integer*2 st0002(3)
      save
      data st0001 / 46, 0/
      data st0002 / 46, 46, 0/
         call smark (sp)
         call salloc (fname, 127 , 2)
         op = fname
         ip=1
110      if (.not.(vfn(ip) .ne. 0)) goto 112
            ch = vfn(ip)
            if (.not.(.not.(ch.eq.32.or.ch.eq.9))) goto 120
               memc(op) = ch
               op = op + 1
120         continue
111         ip=ip+1
            goto 110
112      continue
         memc(op) = 0
         isdir = .false.
         if (.not.(streq (vfn, st0001) .or. streq (vfn, st0002))) goto 
     *   130
            isdir = .true.
            goto 131
130      continue
         if (.not.(finfo (memc(fname), fileio) .ne. -1)) goto 140
            isdir = (fileio(1) .eq. 2 )
            if (.not.(isdir)) goto 150
               call fdirne (memc(fname), pathne, maxch)
               nchars = xstrln(pathne)
150         continue
            goto 141
140      continue
            memc(op) = 36
            memc(op+1) = 0
            call xerpsh
            fd = diropn (memc(fname), 0)
            if (xerpop()) goto 160
               call xfcloe(fd)
               isdir = .true.
160         continue
            nchars = gstrcy (memc(fname), pathne, maxch)
141      continue
131      continue
         call sfree (sp)
         if (.not.(isdir)) goto 170
            imbisy = (nchars)
            goto 100
170      continue
            pathne(1) = 0
            imbisy = (0)
            goto 100
171      continue
100      return
      end
      integer function imbisr (curdir, newdir)
      integer*2 curdir(*)
      integer*2 newdir(*)
      logical subdir
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
      integer sp
      integer fname
      integer root
      integer extn
      integer nchars
      integer*4 fileio((6+16) )
      integer finfo
      integer btoi
      logical streq
      integer*2 st0001(2)
      integer*2 st0002(3)
      save
      data st0001 / 46, 0/
      data st0002 / 46, 46, 0/
         call smark (sp)
         call salloc (fname, 127 , 2)
         subdir = .false.
         if (.not.(streq (newdir, st0001) .or. streq (newdir, st0002))) 
     *   goto 110
            subdir = .true.
            goto 111
110      continue
            call zfnbrk (newdir, root, extn)
            if (.not.(root .eq. 1)) goto 120
               call xstrcy(curdir, memc(fname), 127 )
               call zfsubd (memc(fname), 127 , newdir, nchars)
               if (.not.(finfo (memc(fname), fileio) .ne. -1)) goto 130
                  subdir = (fileio(1) .eq. 2 )
130            continue
120         continue
111      continue
         call sfree (sp)
         imbisr = (btoi (subdir))
         goto 100
100      return
      end
      integer function ptcome (temple)
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
      integer*2 temple(*)
      integer sp
      integer patten
      integer pt
      integer op
      integer junk
      integer npatts
      integer ip
      integer pch
      integer ch
      integer patmae
      logical xerflg
      common /xercom/ xerflg
      save
         call smark (sp)
         call salloc (patten, 161 , 2)
         call xcallc(pt, (10 + 64 * 161 ), 10 )
         if (xerflg) goto 100
         npatts = 0
         ip=1
110      if (.not.(temple(ip) .eq. 44 .or. (temple(ip).eq.32.or.temple(
     *   ip).eq.9))) goto 112
111         ip=ip+1
            goto 110
112      continue
120      if (.not.(temple(ip) .ne. 0)) goto 121
            op = patten
            memc(op) = 94
            op = op + 1
            pch = 0
            ch = temple(ip)
130         if (.not.(ch .ne. 0 .and. ch .ne. 44 .and. .not.(ch.eq.32.or
     *      .ch.eq.9))) goto 131
               if (.not.(ch .eq. 42 .and. pch .ne. 93)) goto 140
                  memc(op) = 63
                  op = op + 1
140            continue
               memc(op) = ch
               op = op + 1
               ip = ip + 1
               pch = ch
               ch = temple(ip)
               goto 130
131         continue
            memc(op) = 36
            op = op + 1
            memc(op) = 0
            npatts = npatts + 1
            junk = patmae (memc(patten), memi(pt+10+((npatts)-1)*161 ), 
     *      161 )
150         if (.not.(temple(ip) .eq. 44 .or. (temple(ip).eq.32.or.
     *      temple(ip).eq.9))) goto 151
               ip = ip + 1
               goto 150
151         continue
            goto 120
121      continue
         memi(pt) = npatts
         call sfree (sp)
         ptcome = (pt)
         goto 100
100      return
      end
      integer function ptmath (pt, str)
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
      integer pt
      integer*2 str(*)
      integer i
      integer patmah
      save
         i=1
110      if (.not.(i .le. memi(pt))) goto 112
            if (.not.(patmah (str, memi(pt+10+((i)-1)*161 )) .gt. 0)) 
     *      goto 120
               ptmath = (1)
               goto 100
120         continue
111         i=i+1
            goto 110
112      continue
         ptmath = (0)
         goto 100
100      return
      end
      subroutine ptfree (pt)
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
      integer pt
      save
         call xmfree(pt, 10 )
100      return
      end
c     temple  template
c     sprinf  sprintf
c     fpathe  fpathname
c     npatts  npatterns
c     minlea  min_lenuserarea
c     imbdiy  imb_display
c     stropn  stropen
c     gstrcy  gstrcpy
c     timbre  t_imbrowse
c     lenste  len_stdline
c     patmae  patmake
c     errcoe  errcode
c     uifnae  uifname
c     ptfree  pt_free
c     nsamps  nsample_lines
c     getlie  getline
c     imbisy  imb_isdirectory
c     dirpah  dirpath
c     imbphr  imb_pheader
c     contrt  contrast
c     gimcrr  gim_createraster
c     fdirne  fdirname
c     diropn  diropen
c     gimwrp  gim_writecolormap
c     imbmke  imb_mkfname
c     sectin  section
c     imbser  imb_setdir
c     gopeni  gopenui
c     imunmp  imunmap
c     imbisr  imb_issubdir
c     pathne  pathname
c     gimseg  gim_setmapping
c     ptmath  pt_match
c     subdis  subdirs
c     patmah  patmatch
c     direcy  directory
c     nowhie  nowhite
c     patten  pattern
c     ptcome  pt_compile
c     fileio  file_info
c     gimwrs  gim_writepixels
c     pargsr  pargstr
