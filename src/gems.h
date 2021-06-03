/* Graphics Gems volume 1 header */

/*********************/
/* 2d geometry types */
/*********************/

typedef struct Point2Struct {
  float x, y;
} Point2;
typedef Point2 Vector2;

typedef struct IntPoint2Struct {
  int x, y;
} IntPoint2;

typedef struct Matrix3Struct {
  float element[3][3];
} Matrix3;

typedef struct Box2dStruct {
  Point2 min, max;
} Box2;

/*********************/
/* 3d geometry types */
/*********************/

typedef struct Point3Struct {
  float x, y, z;
} Point3;
typedef Point3 Vector3;

typedef struct IntPoint3Struct {
  int x, y, z;
} IntPoint3;

typedef struct Matrix4Struct {
  float element[4][4];
} Matrix4;

typedef struct Box3dStruct {
  Point3 min, max;
} Box3;

/***********************/
/* one-argument macros */
/***********************/

/* absolute value of a */
#define ABS(a)        (((a) < 0) ? -(a) : (a))
#define FLOOR(a)      ((a) > 0 ? (int)(a) : -(int)(-a))
#define CEILING(a)    ((a)==(int)(a) ? (a) : (a)>0 ? 1+(int)(a) : -(1+(int)(-a)))
#define ROUND(a)      ((a)>0 ? (int)((a)+0.5f) : -(int)(0.5f-(a)))
#define ZSGN(a)       (((a)<0) ? -1 : (a)>0 ? 1 : 0))
#define SGN(a)        (((a)<0) ? -1 : 0)
#define ASSERT(x)     if (!(x)) Serial.println("Assertion failed: x");
#define SQR(a)        ((a) * (a))


/***********************/
/* two-argument macros */
/***********************/

#define MIN(a, b)     (((a) < (b)) ? (a) : (b))
#define MAX(a, b)     (((a) > (b)) ? (a) : (b))
#define SWAP(a, b)    { a^=b; b^=a; a^=b; }
#define LERP(a, l, h) ((l) + (((h) - (l)) * (a)))
#define CLAMP(v, l, h) ((v) < (l) ? (l) : (v) > (h) ? (h): (v))

/****************************/
/* memory allocation macros */
/****************************/

#define NEWSTRUCT(x)  (struct x *)(malloc((unsigned)sizeof(struct x)))
#define NEWTYPE(x)    (x *)(malloc((unsigned)sizeof(x)))
