      subroutine zscale (im, z1, z2, contrt, optime, lenste)
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
      integer im
      real z1
      real z2
      real contrt
      integer optime
      integer lenste
      integer npix
      integer minpix
      integer ngoodx
      integer centel
      integer ngrow
      real zmin
      real zmax
      real median
      real zstart
      real zslope
      integer sample
      integer left
      integer zscsae
      integer zscfie
      save
         npix = zscsae (im, sample, optime, lenste)
         centel = max (1, (npix + 1) / 2)
         call asrtr (memr(sample), memr(sample), npix)
         zmin = memr(sample)
         zmax = memr(sample+npix-1)
         left = sample + centel - 1
         if (.not.(mod (npix, 2) .eq. 1 .or. centel .ge. npix)) goto 110
            median = memr(left)
            goto 111
110      continue
            median = (memr(left) + memr(left+1)) / 2
111      continue
         minpix = max (5 , int (npix * 0.5 ))
         ngrow = max (1, nint (npix * .01))
         ngoodx = zscfie (memr(sample), npix, zstart, zslope, 2.5 , 
     *   ngrow, 5 )
         if (.not.(ngoodx .lt. minpix)) goto 120
            z1 = zmin
            z2 = zmax
            goto 121
120      continue
            if (.not.(contrt .gt. 0)) goto 130
               zslope = zslope / contrt
130         continue
            z1 = max (zmin, median - (centel - 1) * zslope)
            z2 = min (zmax, median + (npix - centel) * zslope)
121      continue
         call xmfree(sample, 6)
100      return
      end
      integer function zscsae (im, sample, optime, lenste)
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
      integer im
      integer sample
      integer optime
      integer lenste
      integer ncols
      integer nlines
      integer colstp
      integer linesp
      integer maxpix
      integer line
      integer optnpe
      integer npixpe
      integer optnle
      integer minnle
      integer maxnle
      integer op
      integer imgl2r
      save
         ncols = meml(im+200 +1+6-1)
         nlines = meml(im+200 +2+6-1)
         optnpe = max (1, min (ncols, lenste))
         colstp = max (1, (ncols + optnpe-1) / optnpe)
         npixpe = max (1, (ncols + colstp-1) / colstp)
         minnle = max (1, optime / lenste)
         optnle = max(minnle, min(nlines, (optime + npixpe-1) / npixpe))
         linesp = max (1, nlines / (optnle))
         maxnle = (nlines + linesp-1) / linesp
         maxpix = npixpe * maxnle
         call xmallc(sample, maxpix, 6)
         op = sample
         do 110 line = (linesp + 1) / 2, nlines, linesp 
            call zscsue (memr(imgl2r(im,line)), memr(op), npixpe, colstp
     *      )
            op = op + npixpe
            if (.not.(op - sample + npixpe .gt. maxpix)) goto 120
               goto 111
120         continue
110      continue
111      continue
         zscsae = (op - sample)
         goto 100
100      return
      end
      subroutine zscsue (a, b, npix, step)
      integer npix
      integer step
      real a(*)
      real b(npix)
      integer ip
      integer i
      save
         if (.not.(step .le. 1)) goto 110
            call amovr (a, b, npix)
            goto 111
110      continue
            ip = 1
            do 120 i = 1, npix 
               b(i) = a(ip)
               ip = ip + step
120         continue
121         continue
111      continue
100      return
      end
      integer function zscfie (data, npix, zstart, zslope, krej, ngrow, 
     *maxitr)
      integer npix
      real zstart
      real zslope
      real krej
      integer ngrow
      integer maxitr
      real data(npix)
      integer i
      integer ngoodx
      integer lastnx
      integer minpix
      integer niter
      real xscale
      real z0
      real dz
      real x
      real z
      real mean
      real sigma
      real thresd
      double precision sumxsr
      double precision sumxz
      double precision sumz
      double precision sumx
      double precision rowrat
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
      integer flat
      integer badpix
      integer normx
      integer zscres
      integer zsccoa
      save
         call smark (sp)
         if (.not.(npix .le. 0)) goto 110
            zscfie = (0)
            goto 100
110      continue
         if (.not.(npix .eq. 1)) goto 120
            zstart = data(1)
            zslope = 0.0
            zscfie = (1)
            goto 100
120      continue
            xscale = 2.0 / (npix - 1)
121      continue
111      continue
         call salloc (flat, npix, 6)
         call salloc (normx, npix, 6)
         call salloc (badpix, npix, 3)
         call aclrs (mems(badpix), npix)
         do 130 i = 0, npix - 1
            memr(normx+i) = i * xscale - 1.0
130      continue
131      continue
         sumxsr = 0
         sumxz = 0
         sumx = 0
         sumz = 0
         do 140 i = 1, npix 
            x = memr(normx+i-1)
            z = data(i)
            sumxsr = sumxsr + (x ** 2)
            sumxz = sumxz + z * x
            sumz = sumz + z
140      continue
141      continue
         z0 = sumz / npix
         dz = sumxz / sumxsr
         ngoodx = npix
         minpix = max (5 , int (npix * 0.5 ))
         niter=1
150      if (.not.(niter .le. maxitr)) goto 152
            lastnx = ngoodx
            call zscfla (data, memr(flat), memr(normx), npix, z0, dz)
            ngoodx = zsccoa (memr(flat), mems(badpix), npix, mean, sigma
     *      )
            thresd = sigma * krej
            ngoodx = zscres (data, memr(flat), memr(normx), mems(badpix)
     *      , npix, sumxsr, sumxz, sumx, sumz, thresd, ngrow)
            if (.not.(ngoodx .gt. 0)) goto 160
               rowrat = sumx / sumxsr
               z0 = (sumz - rowrat * sumxz) / (ngoodx - rowrat * sumx)
               dz = (sumxz - z0 * sumx) / sumxsr
160         continue
            if (.not.(ngoodx .ge. lastnx .or. ngoodx .lt. minpix)) goto 
     *      170
               goto 152
170         continue
151         niter=niter+1
            goto 150
152      continue
         zstart = z0 - dz
         zslope = dz * xscale
         call sfree (sp)
         zscfie = (ngoodx)
         goto 100
100      return
      end
      subroutine zscfla (data, flat, x, npix, z0, dz)
      integer npix
      real z0
      real dz
      real data(npix)
      real flat(npix)
      real x(npix)
      integer i
      save
         do 110 i = 1, npix
            flat(i) = data(i) - (x(i) * dz + z0)
110      continue
111      continue
100      return
      end
      integer function zsccoa (a, badpix, npix, mean, sigma)
      integer npix
      real mean
      real sigma
      real a(npix)
      integer*2 badpix(npix)
      real pixval
      integer i
      integer ngoodx
      double precision sum
      double precision sumsq
      double precision temp
      integer sw0001
      save
         sum = 0
         sumsq = 0
         ngoodx = 0
         do 110 i = 1, npix
            if (.not.(badpix(i) .eq. 0 )) goto 120
               pixval = a(i)
               ngoodx = ngoodx + 1
               sum = sum + pixval
               sumsq = sumsq + pixval ** 2
120         continue
110      continue
111      continue
         sw0001=(ngoodx)
         goto 130
140      continue
            mean = 1.6e38
            sigma = 1.6e38
         goto 131
150      continue
            mean = sum
            sigma = 1.6e38
         goto 131
160      continue
            mean = sum / ngoodx
            temp = sumsq / (ngoodx - 1) - sum**2 / (ngoodx * (ngoodx - 1
     *      ))
            if (.not.(temp .lt. 0)) goto 170
               sigma = 0.0
               goto 171
170         continue
               sigma = sqrt (temp)
171         continue
            goto 131
130      continue
            if (sw0001.eq.0) goto 140
            if (sw0001.eq.1) goto 150
            goto 160
131      continue
         zsccoa = (ngoodx)
         goto 100
100      return
      end
      integer function zscres (data, flat, normx, badpix, npix, sumxsr, 
     *sumxz, sumx, sumz, thresd, ngrow)
      integer npix
      double precision sumxsr
      double precision sumxz
      double precision sumx
      double precision sumz
      real thresd
      integer ngrow
      real data(npix)
      real flat(npix)
      real normx(npix)
      integer*2 badpix(npix)
      integer ngoodx
      integer i
      integer j
      real residl
      real lcut
      real hcut
      double precision x
      double precision z
      save
         ngoodx = npix
         lcut = -thresd
         hcut = thresd
         do 110 i = 1, npix
            if (.not.(badpix(i) .eq. 1 )) goto 120
               ngoodx = ngoodx - 1
               goto 121
120         continue
               residl = flat(i)
               if (.not.(residl .lt. lcut .or. residl .gt. hcut)) goto 
     *         130
                  do 140 j = max(1,i-ngrow), min(npix,i+ngrow) 
                     if (.not.(badpix(j) .ne. 1 )) goto 150
                        if (.not.(j .le. i)) goto 160
                           x = normx(j)
                           z = data(j)
                           sumxsr = sumxsr - (x ** 2)
                           sumxz = sumxz - z * x
                           sumx = sumx - x
                           sumz = sumz - z
                           badpix(j) = 1
                           ngoodx = ngoodx - 1
                           goto 161
160                     continue
                           badpix(j) = 2
161                     continue
150                  continue
140               continue
141               continue
130            continue
121         continue
110      continue
111      continue
         zscres = (ngoodx)
         goto 100
100      return
      end
c     zscsae  zsc_sample_image
c     lenste  len_stdline
c     minnle  min_nlines_in_sample
c     zscfla  zsc_flatten_data
c     maxnle  max_nlines_in_sample
c     optime  optimal_sample_size
c     ngoodx  ngoodpix
c     npixpe  npix_per_line
c     zsccoa  zsc_compute_sigma
c     optnpe  opt_npix_per_line
c     linesp  line_step
c     contrt  contrast
c     zscres  zsc_reject_pixels
c     optnle  opt_nlines_in_sample
c     zscfie  zsc_fit_line
c     zscsue  zsc_subsample
c     lastnx  last_ngoodpix
c     thresd  threshold
c     sumxsr  sumxsqr
c     maxitr  maxiter
c     centel  center_pixel
c     colstp  col_step
c     residl  residual
