#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image &im) {

    float n = 1.0 / im.w * im.h;
	for (int i = 0; i < im.w; i++)
	{
		for (int j = 0; j < im.h; j++)
		{
			for(int k = 0; k < im.c; k++)
			{
				im(i, j, k) = im(i, j, k) * n;
			}
		}
	}
}

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w) {
    assert(w % 2); // w needs to be odd
	Image im = Image(w, w, 1);
	l1_normalize(im);
    return (im);
}

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image

//Completate Image convolve_image(const Image& im, const Image& filter, bool preserve) . Per questa funzine ci sono due possibilità. Con le convoluzioni normali facciamo una somma pesata su un'area dell'immagine. Con più canali teniamo conto di più possibilità:
// Se il parametro preserve è impostato a true, la funzioe dovrebbe produrre un immagine con lo stesso numero di canali dell'input. Questo è utile per esempio, se vogliamo applicare il box-filter ad un'immagine RGB per ottenere un'altra immagine RGB. Questo significa che ogni canale verrà filtrato separatamente dallo stesso kernel.
// Se preserve è a false dovremmo ritornare un'immagine con un solo canale prodotto applicndo il filtro and ogni canale e poi summandoli insieme in un unico canale.
// Naturalmente, filter dovrebbe avere 1 canale. C'è un assert nel codice che verifica questo.
//meglio usare clamped_pixel per evitare problemi con i bordi

Image convolve_image(const Image &im, const Image &filter, bool preserve) {
    assert(filter.c == 1);
    Image ret = Image(im.w, im.h, preserve ? im.c : 1);
	float q = 0;
	int scalex = filter.w / 2;
	int scaley = filter.w / 2;

    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            for (int k = 0; k < im.c; k++)
            {
                q = 0;
				for (int x = 0; x < filter.w ; x++){
					for (int y = 0; y < filter.h; y++)
						q+= filter(x, y, 0) * im.clamped_pixel(i - scalex + x, j - scaley + y, k);
				}
				ret(i, j, preserve ? k : 0) = q;
            }
        }
    }
    return ret;
}
// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter() {
    Image filter = make_box_filter(3);
    filter(0, 0, 0) = 0;
    filter(0, 1, 0) = -1;
    filter(0, 2, 0) = 0;
    filter(1, 0, 0) = -1;
    filter(1, 1, 0) = 4;
    filter(1, 2, 0) = -1;
    filter(2, 0, 0) = 0;
    filter(2, 1, 0) = -1;
    filter(2, 2, 0) = 0;
    return filter;

}

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter() {
    Image filter = make_box_filter(3);
    filter(0, 0, 0) = 0;
    filter(0, 1, 0) = -1;
    filter(0, 2, 0) = 0;
    filter(1, 0, 0) = -1;
    filter(1, 1, 0) = 5;
    filter(1, 2, 0) = -1;
    filter(2, 0, 0) = 0;
    filter(2, 1, 0) = -1;
    filter(2, 2, 0) = 0;
    return filter;

}

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter() {
    Image filter = make_box_filter(3);
    filter(0, 0, 0) = -2;
    filter(0, 1, 0) = -1;
    filter(0, 2, 0) = 0;
    filter(1, 0, 0) = -1;
    filter(1, 1, 0) = 1;
    filter(1, 2, 0) = 1;
    filter(2, 0, 0) = 0;
    filter(2, 1, 0) = 1;
    filter(2, 2, 0) = 2;
    return filter;
}

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter

Image make_gaussian_filter(float sigma) {
    // TODO: Implement the filter
    int d = 6 * sigma;
    if (d % 2 == 0) d++;
    Image filter = make_box_filter(d);
    float g;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; i< d; j++)
        {
            g = exp(-((i - d / 2) * (i - d / 2) + (j - d / 2) * (j - d / 2)) / (2 * sigma * sigma));
            filter(i, j, 0) = g;
        }
    }
    return filter;

}


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size

    Image ret = Image(a.w, a.h, a.c);
    for (int i = 0; i< a.w; i++)
    {
        for (int j = 0; j < a.h; j++)
        {
            for (int k = 0; k < a.c; k++)
            {
                ret(i, j, k) = a(i, j, k) + b(i, j, k);
            }
        }
    }

    return ret;
}

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size

    Image ret = Image(a.w, a.h, a.c);
    for (int i = 0; i< a.w; i++)
    {
        for (int j = 0; j < a.h; j++)
        {
            for (int k = 0; k < a.c; k++)
            {
                ret(i, j, k) = a(i, j, k) - b(i, j, k);
            }
        }
    }

    return ret;
}

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter() {
    Image filter = make_box_filter(3);
    filter(0, 0, 0) = -1;
    filter(0, 1, 0) = 0;
    filter(0, 2, 0) = 1;
    filter(1, 0, 0) = -2;
    filter(1, 1, 0) = 0;
    filter(1, 2, 0) = 2;
    filter(2, 0, 0) = -1;
    filter(2, 1, 0) = 0;
    filter(2, 2, 0) = 1;
    return filter;

}

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter() {
    Image filter = make_box_filter(3);
    filter(0, 0, 0) = -1;
    filter(0, 1, 0) = -2;
    filter(0, 2, 0) = -1;
    filter(1, 0, 0) = 0;
    filter(1, 1, 0) = 0;
    filter(1, 2, 0) = 0;
    filter(2, 0, 0) = 1;
    filter(2, 1, 0) = 2;
    filter(2, 2, 0) = 1;
    return filter;
}

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image &im) {
    assert(im.w * im.h); // assure we have non-empty image
    float min = 0;
    float max = 0;
    for (int i = 0; i< im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            for (int k = 0; k < im.c; k++)
            {
                if (im(i, j, k) < min) min = im(i, j, k);
                if (im(i, j, k) > max) max = im(i, j, k);
            }
        }
    }
    for (int i = 0; i< im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            for (int k = 0; k < im.c; k++)
            {
                im(i, j, k) = (im(i, j, k) - min) / (max - min);
            }
        }
    }
}


// Normalizes features across all channels
void feature_normalize_total(Image &im) {
    assert(im.w * im.h * im.c); // assure we have non-empty image

    int nc = im.c;
    im.c = 1;
    im.w *= nc;

    feature_normalize(im);

    im.w /= nc;
    im.c = nc;

}


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image, Image> sobel_image(const Image &im) {
    // TODO: Your code here
    Image gx = convolve_image(im, make_gx_filter(), 0);
    Image gy = convolve_image(im, make_gy_filter(), 0);
    feature_normalize(gx);
    feature_normalize(gy);

    return make_pair(gx, gy);
}


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image &im) {
    // TODO: Your code here
    Image sv = sobel_image(im).first;
    Image hue = sobel_image(im).second;
    Image ret(im.w, im.h, im.c);
    feature_normalize(sv);
    for (int i = 0; i< im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            hue(i, j, 0) = (hue(i, j, 0) / (2 * M_PI)) + 0.5;
        }
    }
    for (int i = 0; i< im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            ret(i, j, 0) = hue(i, j, 0);
            ret(i, j, 1) = sv(i, j, 0);
            ret(i, j, 2) = sv(i, j, 0);
        }
    }
    hsv_to_rgb(ret);

    //NOT_IMPLEMENTED();
    convolve_image(ret, make_gaussian_filter(4), 1);
    return ret;
}


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image &im, float sigma1, float sigma2) {
    Image bf = im;
    float W = 0;
    float w = 0;
    float ke = 6 * sigma1;
    //Image filterB()
    Image filterG1 = make_gaussian_filter(sigma1);
    for(int i = 0; i < bf.w; i++)
    {
        for(int j = 0; j < bf.h; j++)
        {
            for(int k = 0; k < bf.c; k++)
            {
                for (int k1 = -ke; k1 <= ke; k1++)
                {
                    for (int k2 = -ke; k2 <= ke; k2++)
                    {
                        w = filterG1(k1,k2) * (im(i,j,k) - im.clamped_pixel(i+1, j+1, k));

                    }
                }
            }
        }
    }


    return bf;
}


// HM #5
//
float *compute_histogram(const Image &im, int ch, int num_bins) {
    float *hist = (float *) malloc(sizeof(float) * num_bins);
    for (int i = 0; i < num_bins; ++i) {
        hist[i] = 0;
    }

    // TODO: Your histogram code
    NOT_IMPLEMENTED();

    return hist;
}

float *compute_CDF(float *hist, int num_bins) {
    float *cdf = (float *) malloc(sizeof(float) * num_bins);

    cdf[0] = hist[0];

    // TODO: Your cdf code
    NOT_IMPLEMENTED();

    return cdf;
}

Image histogram_equalization_hsv(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // TODO: Your histogram equalization code
    NOT_IMPLEMENTED();
    // convert to hsv
    // compute histograms for the luminance channel
    // compute cdf
    // equalization
    // convert back to rgb

    // delete the allocated memory!
//    delete hist;
//    delete cdf;

    return new_im;
}

Image histogram_equalization_rgb(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // compute histograms for each color channel
    for (int c = 0; c < im.c; ++c) {

        // TODO: Your equalization code
        NOT_IMPLEMENTED();

        // delete the allocated memory!
//        delete hist;
//        delete cdf;
    }

    return new_im;
}


// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }

void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }

void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image &a, const Image &b) { return sub_image(a, b); }

Image operator+(const Image &a, const Image &b) { return add_image(a, b); }
