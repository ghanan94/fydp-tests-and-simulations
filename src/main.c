#include <stdio.h>
#include <stdlib.h>
#include <fft/fft.h>
#include <math.h>

int main(void)
{
  int N = 1 << 3; // for 2^3 point (i)fft
  int k;
  complex v[N], v1[N], scratch[N];

  /* Fill v[] with a function of known FFT: */
  for(k=0; k<N; k++) {
    v[k].Re = 0.125*cos(2*PI*k/(double)N);
    v[k].Im = 0.125*sin(2*PI*k/(double)N);
    v1[k].Re =  0.3*cos(2*PI*k/(double)N);
    v1[k].Im = -0.3*sin(2*PI*k/(double)N);
  }

  /* FFT, iFFT of v[]: */
  print_vector("Orig", v, N);
  fft( v, N, scratch );
  print_vector(" FFT", v, N);
  ifft( v, N, scratch );
  print_vector("iFFT", v, N);

  /* FFT, iFFT of v1[]: */
  print_vector("Orig", v1, N);
  fft( v1, N, scratch );
  print_vector(" FFT", v1, N);
  ifft( v1, N, scratch );
  print_vector("iFFT", v1, N);

  exit(EXIT_SUCCESS);
}
