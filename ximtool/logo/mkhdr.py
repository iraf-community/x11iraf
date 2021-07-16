#!/usr/bin/env python3

from astropy.io import fits
import numpy as np

ncolors = 200
logo = fits.open('logo.fits')
logo_data = np.asarray(logo[0].data*ncolors, int)[::-1].flatten()
logo_nbytes = logo_data.shape[0]
data_per_codeline = 15
padding = data_per_codeline - (logo_nbytes % data_per_codeline)

code_rows = np.append(logo_data, [-1] * padding).reshape(-1, data_per_codeline)

hdr = f'''#define LOGO_XDIM {logo[0].data.shape[0]}
#define LOGO_YDIM {logo[0].data.shape[1]}
#define LOGO_NCOLORS {ncolors}
#define LOGO_NBYTES {logo_nbytes}

unsigned char logo_data[] = {{
  '''

footer = '''
};
'''

content = ',\n  '.join(','.join('0x{:02x}'.format(c) for c in row if c >=0)
                       for row in code_rows)

with open('logo.h', 'w') as fp:
    fp.write(hdr)
    fp.write(content)
    fp.write(footer)
