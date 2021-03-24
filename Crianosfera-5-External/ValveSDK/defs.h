
#ifndef DEFS_H
#define DEFS_H

#ifndef VectorSubtract
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#endif

#ifndef VectorAdd
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#endif

#ifndef VectorCopy
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}
#endif

#ifndef VectorClear
#define VectorClear(a) { a[0]=0.0;a[1]=0.0;a[2]=0.0;}
#endif

#ifndef DotProd
#define DotProd(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#endif

#ifndef VectorScale
#define VectorScale(a,b,c) {(c)[0]=(b)*(a)[0];(c)[1]=(b)*(a)[1];(c)[2]=(b)*(a)[2];}
#endif

inline float VectorLength(const float *v)
{
	return (float)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

#endif
