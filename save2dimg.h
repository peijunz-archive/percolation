#ifndef SAVEIMG_H
#define SAVEIMG_H
#include <opencv/cv.hpp>
#include "zonebond.h"
#include "ndarray.h"
using namespace cv;

/// Default color scheme for bond---geometric progression
uint8_t geo[4] = {0, 64, 255, 128};
#ifdef MAT
/**
 * @brief Save 2D percolation with bond classification to image using openCV
 * @param b     matrix for bond type
 * @param col   Color scheme for bondtype
 * @todo Renew the code to mark bond by father ax
 */
template<unsigned int L>
void savemat(ctorus<2> &t, string fname) {
    int width = t.bonds[0].shape(0), sf = L / 2, W = width * L, x1, x2;
    Mat M;
    ndarray<uint8_t> g(2, W);
    g = 0;
    for(int i = 0; i < width - 1; i++) {
        for(int j = 0; j < width - 1; j++) {
            g(L * i + sf, L * j + sf) = col[1];
            for(uint l = 1; l < L; l++) {
                g(L * i + l + sf, L * j + sf) = col[t.bonds[0](i, j)];
                g(L * i + sf, L * j + l + sf) = col[t.bonds[1](i, j)];
            }
        }
        x1 = L * i + sf;
        x2 = L * (width - 1) + sf;
        g(x1, x2) = g(x2, x1) = 63;
        for(uint l = 1; l < L; l++) {
            g((x1 + l) % W, x2 % W) = col[t.bonds[0](i, width - 1)];
            g(x1 % W, (x2 + l) % W) = col[t.bonds[1](i, width - 1)];
            g((x2 + l) % W, x1 % W) = col[t.bonds[0](width - 1, i)];
            g(x2 % W, (x1 + l) % W) = col[t.bonds[1](width - 1, i)];
        }
    }
    M = Mat(width * L, width * L, CV_8UC1, g.head);
    imwrite(fname , M);
}
#endif

template<unsigned int L = 5, bool undirected = true>
/**
 * @brief save 2D near linked list to image
 * @param fname
 *
 * Save 2D percolation to image using openCV
 */
void savenlist(ctorus<2> &t, string fname) {
    int width = t.bonds.shape(0), ax, sf = L / 2, W = width * L, delta;
    Mat M;
    ndarray<uint8_t, 2> g(W);
    g = 0;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < width; j++) {
            g(L * i + sf, L * j + sf) += t.bonds(i, j).size * 63;
            for(int k = 0; k < t.bonds(i, j).size; k++) {
                ax = t.bonds(i, j)[k];
                if(undirected) {
                    if(ax < 0) continue;
                    for(uint l = 1; l < L; l++) {
                        g((L * i + l * (1 - ax) + sf) % W, (L * j + l * ax + sf) % W) = 63;
                    }
                } else {
                    if(ax >= 0) {
                        delta = 1;
                    } else {
                        ax += 2;
                        delta = -1;
                    }
                    for(uint l = 1; l < L; l++) {
                        g((L * i + l * (1 - ax)*delta + sf) % W, \
                          (L * j + l * ax * delta + sf) % W) = 63;
                    }
                }
            }
        }
    }
    M = Mat(width * L, width * L, CV_8UC1, g.head);
    imwrite(fname, M);
}
#endif //SAVEIMG_H
