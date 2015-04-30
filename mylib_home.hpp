#ifndef MYLIB_HPP
#define MYLIB_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <cstdlib>
#include <cv.h>

namespace my
{

// バージョン
const char *VERSION = "2015.4.30";
  
//----------------------------------------------------------------------------
// MyPoint2T
// ・2 次元座標値
//----------------------------------------------------------------------------

template < typename T >
struct MyPoint2T {
  T x;
  T y;
  MyPoint2T() : x( 0 ), y( 0 ){ }
  MyPoint2T( T x0, T y0 ) : x( x0 ), y( y0 ){ }
};

template < typename T >
inline
std::ostream & operator << ( std::ostream &os, const MyPoint2T<T> &p )
{
  os << p.x << "\t" << p.y;
  return os;
}

typedef MyPoint2T<int> MyPoint2i;
typedef MyPoint2T<double> MyPoint2d;
typedef MyPoint2T<short> MyPoint2s;

//----------------------------------------------------------------------------
// MyColor3T
// ・RGB の画素値
//----------------------------------------------------------------------------

template < typename T >
struct MyColor3T {
  T r;
  T g;
  T b;
  MyColor3T(){};
  MyColor3T( T R, T G, T B ) : r( R ), g( G ), b( B ) {};
  MyColor3T( const IplImage *img_in, int x, int y ){
    if( img_in != 0 ){
      assert( img_in->nChannels == 3 );
      b = *( (unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 0 ) );
      g = *( (unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 1 ) );
      r = *( (unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 2 ) );
    }
    else{
      this->r = 0;
      this->g = x;
      this->b = y;
    }
  }
  bool operator == ( const MyColor3T &rhs ) const {
    return this->r == rhs.r && this->g == rhs.g && this->b == rhs.b;
  }
  bool operator != ( const MyColor3T &rhs ) const {
    return this->r != rhs.r || this->g != rhs.g || this->b != rhs.b;
  }
  bool operator < ( const MyColor3T &rhs ) const {
    if( this->r < rhs.r ) return true;
    else if( this->r > rhs.r ) return false;
    else{
      if( this->g < rhs.g ) return true;
      else if( this->g > rhs.g ) return false;
      else return this->b < rhs.b;
    }
  }
};

template < typename T >
inline
std::ostream & operator << ( std::ostream &os, const MyColor3T<T> &cl )
{
  os << cl.r << "\t" << cl.g << "\t" << cl.b;
  return os;
}

template < typename T >
struct MyCmpColor3 {
  bool operator () ( const MyColor3T<T> &a, const MyColor3T<T> &b ) const{
    return a < b;
  }
};

typedef MyColor3T<int> MyColor3i;
typedef MyColor3T<short> MyColor3s;
typedef MyColor3T<unsigned short> MyColor3;
typedef MyColor3T<unsigned short> MyColor3us;
typedef MyColor3T<double> MyColor3d;
typedef MyCmpColor3<unsigned short> MyCmpColor3us;
typedef MyCmpColor3<double> MyCmpColor3d;

//----------------------------------------------------------------------------
// MyAbs
//----------------------------------------------------------------------------

template < class T >
inline
T
MyAbs( T a ){
  return (a > 0) ? a : -a;
}

//----------------------------------------------------------------------------
// MyAbsDiff
//----------------------------------------------------------------------------

template < class T >
inline
T
MyAbsDiff( T a, T b ){
  return (a > b) ? a - b : b - a;
}

//----------------------------------------------------------------------------
// MyMin
//----------------------------------------------------------------------------

template < class T >
inline
T
MyMin( T a, T b ){
  return (a < b) ? a : b;
}

template < class T >
inline
T
MyMin( T a, T b, T c ){
  if( a < b && a < c ) return a;
  else return MyMin< T >( b, c );
}

//----------------------------------------------------------------------------
// MyMax
//----------------------------------------------------------------------------

template < class T >
inline
T
MyMax( T a, T b ){
  return (a > b) ? a : b;
}

template < class T >
inline
T
MyMax( T a, T b, T c ){
  if( a > b && a > c ) return a;
  else return MyMax< T >( b, c );
}

//----------------------------------------------------------------------------
// MySgn
// ・符号を返す
// ・引数が正の場合は、１
// ・引数がゼロの場合は、０
// ・引数が負の場合は、−１
//----------------------------------------------------------------------------
template < typename T >
inline
T MySgn( T x ){
  return x == 0 ? 0 : ( x > 0 ? 1 : -1 );
}

//----------------------------------------------------------------------------
// MySplit
// ・文字列分割
//----------------------------------------------------------------------------

std::vector<std::string> 
MySplit( const std::string &str 
         ){
  using namespace std;
  istringstream iss(str); vector<string> res;
  copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(res));
  return res;
}

//----------------------------------------------------------------------------
// MyEndsWith
// ・指定した文字列で終わっているかどうか
//----------------------------------------------------------------------------

bool 
inline
MyEndsWith( std::string const& str, 
            std::string const& ext // 拡張子など
            ){
  return ext.size() <= str.size() && str.find(ext, str.size() - ext.size()) != str.npos;
}
  
bool 
inline
MyEndsWith( const char *str, 
            const char *ext // 拡張子など
            ){
  return MyEndsWith( std::string( str ), std::string( ext ) );
}

//----------------------------------------------------------------------------
// MyBeginsWith
// ・指定した文字列で始まっているかどうか
//----------------------------------------------------------------------------

bool
inline
MyBeginWith( const std::string &str,
             const std::string &start ){
  return str.find( start ) == 0;
}

bool
inline
MyBeginsWith( const char * str,
             const char * start ){
  return std::string( str ).find( std::string( start ) ) == 0;
}

//----------------------------------------------------------------------------
// MyFindOption
// ・引数配列の中から指定した文字列と一致する要素のインデックスを返す。
//----------------------------------------------------------------------------

int
MyFindOption( int argc, 
              char *argv[], 
              const char *option_str // 検索文字列
              ){
  for( int i = 0; i < argc; i++ ) 
    if( std::string( argv[ i ] ) == std::string( option_str ) ) return i;
  return 0;
}

//----------------------------------------------------------------------------
// MyGetFileName
// ・ファイルパスの文字列の中からパスの部分を省いたファイル名部分だけを返す。
//----------------------------------------------------------------------------

std::string 
inline
MyGetFileName( const std::string &filepath ){
  size_t idx = filepath.find_last_of( '/' );
  if (idx == std::string::npos) return filepath;
  else return filepath.substr( idx + 1, filepath.size() );
}
std::string
inline
MyGetFileName( const char *filepath ){
  return MyGetFileName( std::string( filepath ) );
}

//----------------------------------------------------------------------------
// MyGetFileNameBase
// ・ファイル名の中から拡張子を除いた部分を返す。
//----------------------------------------------------------------------------

std::string
inline
MyGetFileNameBase( const std::string &filepath ){
  size_t idx = filepath.find_last_of( '.' );
  if (idx == std::string::npos) return filepath;
  else return filepath.substr( 0, idx );
}
std::string
inline
MyGetFileNameBase( const char *filepath ){
  return MyGetFileNameBase( std::string( filepath ) );
}

//----------------------------------------------------------------------------
// MyReadText
// ・各行について空白文字で区切られた複数の値が並ぶテキストファイルを読み込む
//----------------------------------------------------------------------------

template < typename T >
int
MyReadText( const char *filepath, 
            std::vector< std::vector< T > > &data_buf, // 読み込んだ結果
            int col_num = 0 // 列数を必要であれば指定
            ){
  using namespace std;
  ifstream fin( filepath );
  if( !fin ) return -1;
  while (fin.good()) {
    string line;
    getline( fin, line );
    vector<string> la = MySplit( line );
    if( ( col_num == 0 && la.size() > 0 ) ||
        ( col_num != 0 && la.size() == col_num ) ){
      vector< T > tmp;
      for (int i = 0; i < la.size(); i++) tmp.push_back( atof( la[ i ].c_str() ) );
      data_buf.push_back( tmp );
    }
  }
  fin.close();
  return 0;
}

//----------------------------------------------------------------------------
// MyReadTextString
// ・各行について空白文字で区切られた複数のデータを文字列として読み込む
//----------------------------------------------------------------------------

int
MyReadTextString( const char *filepath, 
                  std::vector< std::vector< std::string > > &data_buf, // 読み込んだ結果
                  int col_num = 0 // 列数を必要であれば指定
            ){
  using namespace std;
  ifstream fin( filepath );
  if( !fin ) return -1;
  while (fin.good()) {
    string line;
    getline( fin, line );
    vector<string> la = my::MySplit( line );
    if( ( col_num == 0 && la.size() > 0 ) ||
        ( col_num != 0 && la.size() == col_num ) ){
      data_buf.push_back( la );
    }
  }
  fin.close();
  return 0;
}

//----------------------------------------------------------------------------
// MyReadTextMap
// ・各行について key と value の書かれたテキストファイルを読み込む
// ・どちらも文字列
// ・３列以降は無視
//----------------------------------------------------------------------------
int
MyReadTextMap( const char *filepath, 
               std::map< std::string, std::string > &data_buf // 読み込んだ結果
            ){
  using namespace std;
  ifstream fin( filepath );
  if( !fin ) return -1;
  while (fin.good()) {
    string line;
    getline( fin, line );
    vector<string> la = my::MySplit( line );
    if( la.size() >= 2 ) data_buf[ la[ 0 ] ] = la[ 1 ];
  }
  fin.close();
  return 0;
}

//----------------------------------------------------------------------------
// MyCalcHist
// ・vector の要素のヒストグラムを計算
//----------------------------------------------------------------------------

template < typename T >
void
MyCalcHist( const std::vector< T > &data,
            std::map< T, int > &hist
            ){
  using namespace std;
  hist.clear();
  for( typename vector< T >::const_iterator it = data.begin(); it != data.end(); it++ ){
    if( hist.find( *it ) == hist.end() ) hist[ *it ] = 1;
    else hist[ *it ] = hist[ *it ] + 1;
  }
}

//----------------------------------------------------------------------------
// MyGetMostFrequentElement
// ・vector の要素の中から最頻の要素を抽出
//----------------------------------------------------------------------------

template < typename T >
T
MyGetMostFrequentElement( const std::vector< T > &data,
                          int *count = NULL // 最頻値の数が入る
                          ){
  using namespace std;
  map< T, int > hist;
  MyCalcHist( data, hist );
  int max_val = 0;
  int key_at_max = 0;
  for( typename map< T, int >::const_iterator it = hist.begin(); it != hist.end(); it++ ){
    if( it->second > max_val ){
      max_val = it->second;
      key_at_max = it->first;
    }
  }
  if( count != NULL ) *count = max_val;
  return key_at_max;
}

//----------------------------------------------------------------------------
// MyGetPixel
// ・画素値の取得
//----------------------------------------------------------------------------

unsigned short
inline
MyGetPixel( const IplImage *img_in,
            int x,
            int y,
            int plane
            ){
  if( img_in->depth == 8 ){
    return *((unsigned char *)( img_in->imageData + img_in->widthStep * y +
                                ( img_in->depth / 8 ) * ( img_in->nChannels * x + plane ) ) );
  }
  else{
    return *((unsigned short *)( img_in->imageData + img_in->widthStep * y +
                                 ( img_in->depth / 8 ) * ( img_in->nChannels * x + plane ) ) );
  }
}

//----------------------------------------------------------------------------
// MySetPixel
// ・画素値のセット
//----------------------------------------------------------------------------

void
inline
MySetPixel( IplImage *img_in,
            int x,
            int y,
            int plane,
            unsigned short val ){
  if( img_in->depth == 8 ){
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y +
                         ( img_in->depth / 8 ) * ( img_in->nChannels * x + plane ) ) ) =
        (unsigned char)val;
  }
  else{
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y +
                          ( img_in->depth / 8 ) * ( img_in->nChannels * x + plane ) ) ) = val;
  }
}

void
inline
MySetPixel( IplImage *img_in,
            int x,
            int y,
            unsigned short r,
            unsigned short g,
            unsigned short b ){
  assert( img_in->nChannels == 3 );
  if( img_in->depth == 8 ){
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 0 )) = b;
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 1 )) = g;
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 2 )) = r;
  }
  else{
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y + 6 * x + 0 )) = b;
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y + 6 * x + 4 )) = g;
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y + 6 * x + 6 )) = r;
  }
}

void
inline
MySetPixel( IplImage *img_in,
            int x,
            int y,
            MyColor3 cl ){
  assert( img_in->nChannels == 3 );
  if( img_in->depth == 8 ){
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 0 )) = cl.b;
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 1 )) = cl.g;
    *((unsigned char *)( img_in->imageData + img_in->widthStep * y + 3 * x + 2 )) = cl.r;
  }
  else{
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y + 6 * x + 0 )) = cl.b;
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y + 6 * x + 4 )) = cl.g;
    *((unsigned short *)( img_in->imageData + img_in->widthStep * y + 6 * x + 6 )) = cl.r;
  }
}

//----------------------------------------------------------------------------
// MyPixelExpansion
// ・領域膨張
// ・指定された画像の指定された画素位置を起点として領域膨張を行う。
// ・隣接画素のうち、起点画素との画素値の差分が RGB 共に thres 以下の画素を同一領域として扱う。
// ・同一領域と判定された画素位置の画素値を 0 以外にセットした画像を返す。
// ・領域のサイズ（画素数）を返す
//----------------------------------------------------------------------------

int
MyPixelExpansion( const IplImage *img_in, // RGB 3 チャンネル画像
                  int x,
                  int y,
                  int thres, // 同一画素と判定する
                  IplImage *img_out, // 領域外：0 領域内：1
                  int pix_val = 1 // 領域内の値を1以外にしたい場合（ 2以上）
                  ){
  using namespace std;
  // 入力チェック
  assert( img_in->nChannels == 3 );
  assert( img_out->nChannels == 1 );
  assert( pix_val >= 1 );
  // 出力画像をまず黒に
  cvSet( img_out, cvScalar( 0 ) );
  // 判定待ちの画素位置リスト
  // MSB 16 bit : y 座標値
  // LSB 16 bit : x 座標値
  stack< int, vector< int > > next;
  // 領域サイズ
  int area_size = 0;
  // 検査済みフラグの値
  int checked = 2;
  if( checked == pix_val ) checked = 3;
  // まず当該画素位置について判定
  if( x >= 0 && x < img_in->width &&
      y >= 0 && y < img_in->height ){
    // 出力画像に書き込み
    MySetPixel( img_out, x, y, 0, pix_val );
    // 当該画素位置の画素値
    MyColor3 col( img_in, x, y );
    // 周辺８画素について、
    // 当該画素の画素値と画素値の差が thres 以下であれば
    // 同一領域として追加。
    for( int i = -1; i <= 1; i++ ){
      for( int j = -1; j <= 1; j++ ){
        if( i == 0 && j == 0 ) continue;
        int x2 = x + j;
        int y2 = y + i;
        if( x2 >= 0 && x2 < img_in->width &&
            y2 >= 0 && y2 < img_in->height ){
          MyColor3 col2( img_in, x2, y2 );
          if( MyAbsDiff( col.r, col2.r ) <= thres &&
              MyAbsDiff( col.g, col2.g ) <= thres &&
              MyAbsDiff( col.b, col2.b ) <= thres ){
            // 書き込み
            MySetPixel( img_out, x2, y2, 0, pix_val );
            // 更にそれらの周辺画素も判定するためにリストに追加
            next.push( y2 << 16 | x2 );
          }
          else{
            // 判定済み画素として一時的にフラグを立てる
            MySetPixel( img_out, x2, y2, 0, checked );
          }
        }
      }
    }
    // リストが空になるまで
    while( ! next.empty() ){
      // 判定画素位置
      int x1 = 0x0000ffff & next.top();
      int y1 = 0x0000ffff & ( next.top() >> 16 );
      next.pop();
      // 周辺８画素を見る
      for( int i = -1; i <= 1; i++ ){
        for( int j = -1; j <= 1; j++ ){
          if( i == 0 && j == 0 ) continue;
          int x2 = x1 + j;
          int y2 = y1 + i;
          if( x2 >= 0 && x2 < img_in->width &&
              y2 >= 0 && y2 < img_in->height ){
            // 判定済み画素でなければ
            if( MyGetPixel( img_out, x2, y2, 0 ) == 0 ){
              // 同一領域判定
              MyColor3 col2( img_in, x2, y2 );
              if( MyAbsDiff( col.r, col2.r ) <= thres &&
                  MyAbsDiff( col.g, col2.g ) <= thres &&
                  MyAbsDiff( col.b, col2.b ) <= thres ){
                // 書き込み
                MySetPixel( img_out, x2, y2, 0, pix_val );
                // 更にそれらの周辺画素も判定するためにリストに追加
                next.push( y2 << 16 | x2 );
              }
              else{
                // 判定済み画素として一時的にフラグを立てる
                MySetPixel( img_out, x2, y2, 0, checked );
              }
            }
          }
        }//j
      }//i
    }//while
    // 同一領域画素でないが一時的にフラグを立てたところを戻す
    // 領域サイズのカウント
    area_size = 0;
    for( int y1 = 0; y1 < img_out->height; y1++ ){
      for( int x1 = 0; x1 < img_out->width; x1++ ){
        if( MyGetPixel( img_out, x1, y1, 0 ) == checked ){
          MySetPixel( img_out, x1, y1, 0, 0 );
        }
        else if( MyGetPixel( img_out, x1, y1, 0 ) == pix_val ){
          area_size += 1;
        }
      }//x1
    }//y1
  }
  return area_size;
}

// alias
int MyAreaExpansion( const IplImage *img_in, int x, int y, int thres,  IplImage *img_out,
                     int pix_val = 1 ){
  return MyPixelExpansion( img_in, x, y, thres, img_out, pix_val );
}

//----------------------------------------------------------------------------
// MyInverseMat3x3
// ・3x3 の逆行列：解析的
//----------------------------------------------------------------------------

double
inline
MyInverseMat3x3( const double src[][ 3 ], double dst[][ 3 ] )
{
  double detA =
      src[0][0] * src[1][1] * src[2][2] +
      src[1][0] * src[2][1] * src[0][2] +
      src[2][0] * src[0][1] * src[1][2] -
      src[0][0] * src[2][1] * src[1][2] -
      src[2][0] * src[1][1] * src[0][2] -
      src[1][0] * src[0][1] * src[2][2];
  
  dst[0][0] = src[1][1] * src[2][2] - src[1][2] * src[2][1];
  dst[0][1] = src[0][2] * src[2][1] - src[0][1] * src[2][2];
  dst[0][2] = src[0][1] * src[1][2] - src[0][2] * src[1][1];
  dst[1][0] = src[1][2] * src[2][0] - src[1][0] * src[2][2];
  dst[1][1] = src[0][0] * src[2][2] - src[0][2] * src[2][0];
  dst[1][2] = src[0][2] * src[1][0] - src[0][0] * src[1][2];
  dst[2][0] = src[1][0] * src[2][1] - src[1][1] * src[2][0];
  dst[2][1] = src[0][1] * src[2][0] - src[0][0] * src[2][1];
  dst[2][2] = src[0][0] * src[1][1] - src[0][1] * src[1][0];
  
  for( int i = 0; i < 3; i++ ){
    for( int j = 0; j < 3; j++ ){
      dst[ i ][ j ] /= detA;
    }
  }

  return detA;
}

//----------------------------------------------------------------------------
// MyInverseMat4x4
// ・4x4 の逆行列：解析的
//----------------------------------------------------------------------------

double
inline
MyInverseMat4x4( const double src[][ 4 ], double dst[][ 4 ] )
{
  double detA =
      src[0][0]*src[1][1]*src[2][2]*src[3][3] + src[0][0]*src[1][2]*src[2][3]*src[3][1] + src[0][0]*src[1][3]*src[2][1]*src[3][2] +
      src[0][1]*src[1][0]*src[2][3]*src[3][2] + src[0][1]*src[1][2]*src[2][0]*src[3][3] + src[0][1]*src[1][3]*src[2][2]*src[3][0] +
      src[0][2]*src[1][0]*src[2][1]*src[3][3] + src[0][2]*src[1][1]*src[2][3]*src[3][0] + src[0][2]*src[1][3]*src[2][0]*src[3][1] +
      src[0][3]*src[1][0]*src[2][2]*src[3][1] + src[0][3]*src[1][1]*src[2][0]*src[3][2] + src[0][3]*src[1][2]*src[2][1]*src[3][0] -
      src[0][0]*src[1][1]*src[2][3]*src[3][2] - src[0][0]*src[1][2]*src[2][1]*src[3][3] - src[0][0]*src[1][3]*src[2][2]*src[3][1] -
      src[0][1]*src[1][0]*src[2][2]*src[3][3] - src[0][1]*src[1][2]*src[2][3]*src[3][0] - src[0][1]*src[1][3]*src[2][0]*src[3][2] -
      src[0][2]*src[1][0]*src[2][3]*src[3][1] - src[0][2]*src[1][1]*src[2][0]*src[3][3] - src[0][2]*src[1][3]*src[2][1]*src[3][0] -
      src[0][3]*src[1][0]*src[2][1]*src[3][2] - src[0][3]*src[1][1]*src[2][2]*src[3][0] - src[0][3]*src[1][2]*src[2][0]*src[3][1];

  dst[0][0] = src[1][1]*src[2][2]*src[3][3]+src[1][2]*src[2][3]*src[3][1]+src[1][3]*src[2][1]*src[3][2]-src[1][1]*src[2][3]*src[3][2]-src[1][2]*src[2][1]*src[3][3]-src[1][3]*src[2][2]*src[3][1];
  dst[0][1] = src[0][1]*src[2][3]*src[3][2]+src[0][2]*src[2][1]*src[3][3]+src[0][3]*src[2][2]*src[3][1]-src[0][1]*src[2][2]*src[3][3]-src[0][2]*src[2][3]*src[3][1]-src[0][3]*src[2][1]*src[3][2];
  dst[0][2] = src[0][1]*src[1][2]*src[3][3]+src[0][2]*src[1][3]*src[3][1]+src[0][3]*src[1][1]*src[3][2]-src[0][1]*src[1][3]*src[3][2]-src[0][2]*src[1][1]*src[3][3]-src[0][3]*src[1][2]*src[3][1];
  dst[0][3] = src[0][1]*src[1][3]*src[2][2]+src[0][2]*src[1][1]*src[2][3]+src[0][3]*src[1][2]*src[2][1]-src[0][1]*src[1][2]*src[2][3]-src[0][2]*src[1][3]*src[2][1]-src[0][3]*src[1][1]*src[2][2];
  dst[1][0] = src[1][0]*src[2][3]*src[3][2]+src[1][2]*src[2][0]*src[3][3]+src[1][3]*src[2][2]*src[3][0]-src[1][0]*src[2][2]*src[3][3]-src[1][2]*src[2][3]*src[3][0]-src[1][3]*src[2][0]*src[3][2];
  dst[1][1] = src[0][0]*src[2][2]*src[3][3]+src[0][2]*src[2][3]*src[3][0]+src[0][3]*src[2][0]*src[3][2]-src[0][0]*src[2][3]*src[3][2]-src[0][2]*src[2][0]*src[3][3]-src[0][3]*src[2][2]*src[3][0];
  dst[1][2] = src[0][0]*src[1][3]*src[3][2]+src[0][2]*src[1][0]*src[3][3]+src[0][3]*src[1][2]*src[3][0]-src[0][0]*src[1][2]*src[3][3]-src[0][2]*src[1][3]*src[3][0]-src[0][3]*src[1][0]*src[3][2];
  dst[1][3] = src[0][0]*src[1][2]*src[2][3]+src[0][2]*src[1][3]*src[2][0]+src[0][3]*src[1][0]*src[2][2]-src[0][0]*src[1][3]*src[2][2]-src[0][2]*src[1][0]*src[2][3]-src[0][3]*src[1][2]*src[2][0];
  dst[2][0] = src[1][0]*src[2][1]*src[3][3]+src[1][1]*src[2][3]*src[3][0]+src[1][3]*src[2][0]*src[3][1]-src[1][0]*src[2][3]*src[3][1]-src[1][1]*src[2][0]*src[3][3]-src[1][3]*src[2][1]*src[3][0];
  dst[2][1] = src[0][0]*src[2][3]*src[3][1]+src[0][1]*src[2][0]*src[3][3]+src[0][3]*src[2][1]*src[3][0]-src[0][0]*src[2][1]*src[3][3]-src[0][1]*src[2][3]*src[3][0]-src[0][3]*src[2][0]*src[3][1];
  dst[2][2] = src[0][0]*src[1][1]*src[3][3]+src[0][1]*src[1][3]*src[3][0]+src[0][3]*src[1][0]*src[3][1]-src[0][0]*src[1][3]*src[3][1]-src[0][1]*src[1][0]*src[3][3]-src[0][3]*src[1][1]*src[3][0];
  dst[2][3] = src[0][0]*src[1][3]*src[2][1]+src[0][1]*src[1][0]*src[2][3]+src[0][3]*src[1][1]*src[2][0]-src[0][0]*src[1][1]*src[2][3]-src[0][1]*src[1][3]*src[2][0]-src[0][3]*src[1][0]*src[2][1];
  dst[3][0] = src[1][0]*src[2][2]*src[3][1]+src[1][1]*src[2][0]*src[3][2]+src[1][2]*src[2][1]*src[3][0]-src[1][0]*src[2][1]*src[3][2]-src[1][1]*src[2][2]*src[3][0]-src[1][2]*src[2][0]*src[3][1];
  dst[3][1] = src[0][0]*src[2][1]*src[3][2]+src[0][1]*src[2][2]*src[3][0]+src[0][2]*src[2][0]*src[3][1]-src[0][0]*src[2][2]*src[3][1]-src[0][1]*src[2][0]*src[3][2]-src[0][2]*src[2][1]*src[3][0];
  dst[3][2] = src[0][0]*src[1][2]*src[3][1]+src[0][1]*src[1][0]*src[3][2]+src[0][2]*src[1][1]*src[3][0]-src[0][0]*src[1][1]*src[3][2]-src[0][1]*src[1][2]*src[3][0]-src[0][2]*src[1][0]*src[3][1];
  dst[3][3] = src[0][0]*src[1][1]*src[2][2]+src[0][1]*src[1][2]*src[2][0]+src[0][2]*src[1][0]*src[2][1]-src[0][0]*src[1][2]*src[2][1]-src[0][1]*src[1][0]*src[2][2]-src[0][2]*src[1][1]*src[2][0];
  
  for( int j = 0; j < 4; j++ ){
    for( int i = 0; i < 4; i++ ){
      dst[ i ][ j ] /= detA;
    }//i
  }//j

  return detA;
}

//----------------------------------------------------------------------------
// MyFitQuadratic
// ・二次式 y = a x^2 + b x + c でのフィッティング
//----------------------------------------------------------------------------

int
MyFitQuadratic( const double *data_x, const double *data_y, int data_num,
                double *a, double *b, double *c )
{
  double n = 0.0, sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0, sum_x4 = 0.0, 
      sum_y = 0.0, sum_xy = 0.0, sum_x2y = 0.0;
  
  for( int i = 0; i < data_num; i++ ){

    n += 1;
    sum_x += data_x[ i ];
    sum_x2 += data_x[ i ] * data_x[ i ];
    sum_x3 += data_x[ i ] * data_x[ i ] * data_x[ i ];
    sum_x4 += data_x[ i ] * data_x[ i ] * data_x[ i ] * data_x[ i ];
    sum_y += data_y[ i ];
    sum_xy += data_x[ i ] * data_y[ i ];
    sum_x2y += data_x[ i ] * data_x[ i ] * data_y[ i ];

  }// i
  
  double A[ 3 ][ 3 ];
  A[ 0 ][ 0 ] = n;      A[ 0 ][ 1 ] = sum_x;  A[ 0 ][ 2 ] = sum_x2;
  A[ 1 ][ 0 ] = sum_x;  A[ 1 ][ 1 ] = sum_x2; A[ 1 ][ 2 ] = sum_x3;
  A[ 2 ][ 0 ] = sum_x2; A[ 2 ][ 1 ] = sum_x3; A[ 2 ][ 2 ] = sum_x4;

  double B[ 3 ];
  B[ 0 ] = sum_y;
  B[ 1 ] = sum_xy;
  B[ 2 ] = sum_x2y;

  double InvA[ 3 ][ 3 ];

  MyInverseMat3x3( A, InvA );

  *c = InvA[0][0] * B[0] + InvA[0][1] * B[1]  + InvA[0][2] * B[2];
  *b = InvA[1][0] * B[0] + InvA[1][1] * B[1]  + InvA[1][2] * B[2];
  *a = InvA[2][0] * B[0] + InvA[2][1] * B[1]  + InvA[2][2] * B[2];

  return 0;
}

//----------------------------------------------------------------------------
// MyFitCubic
// ・三次式 y = a x^3 + b x^2  + c x + d でのフィッティング
//----------------------------------------------------------------------------

int
MyFitCubic( const double *data_x, const double *data_y, int data_num,
            double *a, double *b, double *c, double *d )
{
  double sum_m = 0.0, sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0;
  double sum_x4 = 0.0, sum_x5 = 0.0, sum_x6 = 0.0;
  double sum_y = 0.0, sum_yx = 0.0, sum_yx2 = 0.0, sum_yx3 = 0.0;
  
  for( int i = 0; i < data_num; i++ ){
    sum_m  += 1;
    sum_x  += data_x[i];
    sum_x2 += data_x[i]*data_x[i];
    sum_x3 += data_x[i]*data_x[i]*data_x[i];
    sum_x4 += data_x[i]*data_x[i]*data_x[i]*data_x[i];
    sum_x5 += data_x[i]*data_x[i]*data_x[i]*data_x[i]*data_x[i];
    sum_x6 += data_x[i]*data_x[i]*data_x[i]*data_x[i]*data_x[i]*data_x[i];
    sum_y  += data_y[i];
    sum_yx += data_y[i]*data_x[i];
    sum_yx2 += data_y[i]*data_x[i]*data_x[i];
    sum_yx3 += data_y[i]*data_x[i]*data_x[i]*data_x[i];
  }

  double A[ 4 ][ 4 ];
  A[0][0] = sum_m;  A[0][1] = sum_x;  A[0][2] = sum_x2; A[0][3] = sum_x3;
  A[1][0] = sum_x;  A[1][1] = sum_x2; A[1][2] = sum_x3; A[1][3] = sum_x4;
  A[2][0] = sum_x2; A[2][1] = sum_x3; A[2][2] = sum_x4; A[2][3] = sum_x5;
  A[3][0] = sum_x3; A[3][1] = sum_x4; A[3][2] = sum_x5; A[3][3] = sum_x6;
               
  double B[ 4 ];
  B[0] = sum_y;
  B[1] = sum_yx;
  B[2] = sum_yx2;
  B[3] = sum_yx3;
  
  double InvA[ 4 ][ 4 ];

  MyInverseMat4x4( A, InvA );

  *d = InvA[0][0] * B[0] + InvA[0][1] * B[1]  + InvA[0][2] * B[2] + InvA[0][3] * B[3];
  *c = InvA[1][0] * B[0] + InvA[1][1] * B[1]  + InvA[1][2] * B[2] + InvA[1][3] * B[3];
  *b = InvA[2][0] * B[0] + InvA[2][1] * B[1]  + InvA[2][2] * B[2] + InvA[2][3] * B[3];
  *a = InvA[3][0] * B[0] + InvA[3][1] * B[1]  + InvA[3][2] * B[2] + InvA[3][3] * B[3];

  return 0;
}

//----------------------------------------------------------------------------
// MyRandGauss
// ・平均 mu、分散 sg2 の正規分布で分布する乱数を発生する。
//----------------------------------------------------------------------------

double MyRandGauss( double mu, double sg2 ){
  using namespace std;
  // Box-Muller's Method
  double X = rand() / (double)RAND_MAX;
  double Y = rand() / (double)RAND_MAX;
  double Z1 = sqrt( sg2 ) * sqrt( -2 * log( X ) ) * cos( 2 * M_PI * Y ) + mu;
  //double Z2 = sqrt( sg2 ) * sqrt( -2 * log( X ) ) * sin( 2 * M_PI * Y ) + mu;
  return Z1;
}

//----------------------------------------------------------------------------
// MyRandPoisson
// ・ポアソン分布に従う乱数を発生する。
// ・ただし、lambda が大きいと exp( -lambda ) が 0 になり、返ってこない。
//----------------------------------------------------------------------------

int MyRandPoisson( double lambda ){
  using namespace std;
  assert( lambda > 0 );
  double x = rand() / (double)RAND_MAX;
  int k = 0;
  double a = exp( -lambda );
  while( x >= a ){
    x = x * rand() / (double)RAND_MAX;
    k = k + 1;
  }
  return k;
}

//----------------------------------------------------------------------------
// vector operators
//----------------------------------------------------------------------------

template < typename T >
inline
std::vector< T > operator + ( const std::vector< T > &a,
                              const std::vector< T > &b ){
  assert( a.size() == b.size() );
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = a[ i ] + b[ i ];
  return c;
}

template < typename T >
inline
std::vector< T > operator - ( const std::vector< T > &a,
                              const std::vector< T > &b ){
  assert( a.size() == b.size() );
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = a[ i ] - b[ i ];
  return c;
}

template < typename T >
inline
std::vector< T > operator * ( const T k,
                              const std::vector< T > &a ){
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = k * a[ i ];
  return c;
}

template < typename T >
inline
std::vector< T > operator / ( const std::vector< T > &a,
                              const T k ){
  assert( k != 0 );
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = a[ i ] / k;
  return c;
}

template < typename T >
inline
std::ostream & operator << ( std::ostream &os,
                             const std::vector< T > &a ){
  for( int i = 0; i < a.size(); i++ ) os << a[ i ] << "\t";
  return os;
}

//----------------------------------------------------------------------------
// MyVecNorm
// ・ベクトルのノルムを返す
//----------------------------------------------------------------------------

template < typename T >
inline
T
MyVecNorm( const std::vector< T > &a ){
  T sum = 0.0;
  for( int i = 0; i < a.size(); i++ ){
    sum += a[ i ] * a[ i ];
  }
  return sqrt( sum );
}

//----------------------------------------------------------------------------
// MyVecGrad
// ・任意の関数の勾配（ナブラ）を返す。
// ・vector< T > fx( const vector< T > &x ) な関数
// ・数値微分（中心差分）
//----------------------------------------------------------------------------

template < typename T >
inline
int
MyVecGrad( T (*fx)( const std::vector< T > & ), // 関数ポインタ
           const std::vector< T > &x, // この位置での勾配を計算
           std::vector< T > &out, // 出力値
           T h = 1E-06 // 数値微分の際の微小変化値
           ){
  using namespace std;

  // h が 0 だとゼロ割になってしまうのでダメ
  assert( h != 0 );

  // ベクトルの次元数
  int n = x.size();

  // もし出力ベクトルがからだったらリサイズ
  if( out.empty() ) out.resize( n );

  // そうでなければ、ベクトルのサイズはあらかじめ確保されているものとする
  else assert( out.size() == n );

  // 各成分ごとに
  for( int i = 0; i < n; i++ ){

    // その成分だけを少し動かすベクトル
    vector< T > dx( n, 0 );
    dx[ i ] = h;

    // 両隣の関数値の変化から微分値を計算（中心差分方式）   
    out[ i ] = ( fx( x + dx ) - fx( x - dx ) ) / ( 2 * h );
  }
  
  return 0;
}

// 勾配ベクトルを返り値として返すパターン
template < typename T >
inline
std::vector< T > MyVecGrad( T (*fx)( const std::vector< T > & ),
                            const std::vector< T > &x, T h = 1E-06 ){
  std::vector< T > out;
  MyVecGrad( fx, x, out, h );
  return out;
}

//----------------------------------------------------------------------------
// MyCalcGrad
// ・１変数関数の数値微分を計算
// ・T fx( T x ) な関数
// ・中心差分
//----------------------------------------------------------------------------
template < typename T >
inline
int
MyCalcGrad( T (*fx)( T ), // 微分計算対象の関数ポインタ
            T x, // この値での微分値を計算
            T *out, // 出力値
            T h = 1E-06 // 微分の微小変化量
            ){

  // h が 0 だとゼロ割になってしまうのでダメ
  assert( h != 0 );

  // 両隣の関数値の変化から微分値を計算（中心差分方式）   
  *out = ( fx( x + h ) - fx( x - h ) ) / ( 2 * h );

  return 0;
}

// 値を返す版
template < typename T >
inline
T MyCalcGrad( T (*fx)( T ), T x, T h = 1E-06 ){
  T out;
  MyCalcGrad( fx, x, &out, h );
  return out;
}

//----------------------------------------------------------------------------
// MyGoldenSection
// ・１変数の最小値検索
// ・黄金分割法
// ・a < x < b の範囲における f(x) の値を最小にする x の値を返す。
//----------------------------------------------------------------------------
int
MyGoldenSection( double (*fx)( double ), // 評価関数
                 double a, // x の検索範囲（の初期値）
                 double b, // x の検索範囲（の初期値）
                 double *out, // 出力値。評価関数の値を最小にする x の値
                 double thres = 1E-06, // 終了条件： |b-a| がこの値以下になったら計算終了。 
                 int max_itr_num = 100, // 終了条件： 繰り返し計算回数の最大値。
                 std::ostream *dout = 0 // デバッグ出力
                 ){
  using namespace std;

  // 内分比（黄金比）
  const double tau = ( sqrt( 5 ) - 1 ) / 2.0;

  // 初期範囲
  double t0 = a;
  double t3 = b;

  if( dout ){
    *dout << "--- MyGoldenSection() ---" << endl;
    *dout << "a:\t" << a << endl;
    *dout << "b:\t" << b << endl;
  }

  // 探索処理
  for( int i = 0; i < max_itr_num; i++ ){

    // [t0, t3] をそれぞれ ( tau / ( tau + 1 ) ), ( 1 / ( tau + 1 ) ) に内分する点を求める
    double t1 = t0 + ( t3 - t0 ) * tau / ( 1 + tau );
    double t2 = t0 + ( t3 - t0 ) * 1 / ( 1 + tau );

    // それぞれの位置での関数値を計算
    double f1 = fx( t1 );
    double f2 = fx( t2 );
    
    if( dout ){
      *dout << "[" << i << "]\t";
      *dout << "f(" << t1 << ") = " << f1 << ",\t";
      *dout << "f(" << t2 << ") = " << f2 << endl;
    }

    // 関数値の大小に応じて、探索範囲を狭めていく
    if( f1 > f2 ) t0 = t1;
    else t3 = t2;

    // 探索範囲が閾値以下になったら終了
    if( abs( t3 - t0 ) < thres ) break;
  }

  // 探索範囲の中点を出力する
  *out = (t3 + t0) / 2.0;
  
  return 0;
}

//----------------------------------------------------------------------------
// MyGradientBasedSearch
// ・１変数の最小値検索
// ・勾配利用。勾配は数値微分。
//----------------------------------------------------------------------------

int
MyGradientBasedSearch( double (*fx)( double ), // 評価関数
                       double x_init, // 初期値
                       double *out, // 出力値。評価関数の値を最小にする x の値
                       double thres = 1E-06, // 終了条件： 勾配の絶対値がこの値以下になったら計算終了。 
                       int max_itr_num = 100, // 終了条件： 繰り返し計算回数の最大値。
                       std::ostream *dout = 0 // デバッグ出力
                       ){
  using namespace std;
  using namespace my;

  // 固定パラメータ
  const double INIT_STEP_PCT = 1; // 最初の移動量。初期値に対する % で指定。
  const double INIT_STEP_WHEN_ZERO = 0.00025; // 初期値がゼロだった場合の最初の移動量。
  const double MAX_ITR_NUM2 = 25; // 内部の while 文での最大繰り返し数。

  // 初期値のセット
  double x = x_init;
  double h = x_init * INIT_STEP_PCT / 100.0;
  if( h == 0 ) h = INIT_STEP_WHEN_ZERO;

  // 諸変数
  int itr_cnt = 0;
  double gx_x, X, Xd, fx_X, fx_Xd;

  // 初期位置での勾配
  gx_x = MyCalcGrad( fx, x );

  if( dout ){
    *dout << "--- MyGradientBasedSearch() ---" << endl;
    *dout << "[" << itr_cnt << "]";
    *dout << "\t f'(" << x << ") = " << gx_x << endl;
  }

  // 探索処理（勾配が閾値以下（≒０）になるまで）
  while( MyAbs( gx_x ) > thres ){

    // 勾配の正負から検索方向を決める。
    h = - MySgn( gx_x ) * MyAbs( h );

    // 現在位置から h 離れた位置の関数値を求める
    X = x;
    Xd = x + h;
    fx_X = fx( X );
    fx_Xd = fx( Xd );
    
    if( dout ){
      *dout << "\t f(" << x << ") = " << fx_X << ",\t";
      *dout << "f(" << x << " + " << h << ") = " << fx_Xd << endl;
    }

    // 関数値が小さくなっていれば
    if( fx_X > fx_Xd ){

      // 関数値がもっと小さくなるようにさらに h を大きくする。
      int cnt = 0;
      while( fx_X > fx_Xd ){ // 関数値が逆に大きくなるまで

        // h を倍々に
        h *= 2;
        X = Xd;
        Xd = X + h;

        // 関数値の計算
        fx_X = fx( X );
        fx_Xd = fx( Xd );
        
        if( dout ){
          *dout << "\t f(" << X << ") = " << fx_X << ",\t";
          *dout << "f(" << X << " + " << h << ") = " << fx_Xd << endl;
        }

        // 安全のため、閾値以上繰り返したらループを抜ける。
        if( ++cnt > MAX_ITR_NUM2 ) break;
      }

      // 位置の更新
      x = X;
      h *= .5;
      
    }// if
    
    // 逆に、関数値が大きくなっていたら
    else{

      // 大きすぎた h を小さくする
      int cnt = 0;
      while( fx_X < fx_Xd ){

        // h を半分に
        h *= .5;
        Xd = Xd - h;

        // 関数値の計算
        fx_Xd = fx( Xd );
        
        if( dout ){
          *dout << "\t f(" << X << ") = " << fx_X << ",\t";
          *dout << "f(" << X << " + " << h << ") = " << fx_Xd << endl;
        }
        
        // 安全のため、閾値以上繰り返したらループを抜ける。
        if( ++cnt > MAX_ITR_NUM2 ) break;
      }

      // 位置の更新
      x = Xd;
      h *= 2;
      
    }// else

    // 繰り返し回数のカウント
    itr_cnt += 1;

    // 最大回数を超えたら計算終了
    if( itr_cnt > max_itr_num ){
      break;
    }

    // 勾配の値を更新
    gx_x = MyCalcGrad( fx, x );
    
    if( dout ){
      *dout << "[" << itr_cnt << "]";
      *dout << "\t f'(" << x << ") = " << gx_x << endl;
    }
    
  }// while

  // 出力値のセット
  *out = x;
  
  return 0;
}

//----------------------------------------------------------------------------
// MyDownhillSimplex
// ・関数の最小値を求める関数
// ・Downhill Simplex 法 (Nelder-Mead)
//----------------------------------------------------------------------------

int
MyDownhillSimplex( double (*fx)( const std::vector< double > & ), // 評価関数
                   std::vector< double > x_init, // 初期値
                   std::vector< double > &out, // 出力値。評価関数の値を最小にする x 。
                   double thres = 1E-06, // 終了条件： シンプレックスサイズがこの値以下。 
                   int max_itr_num = 100, // 終了条件： 繰り返し計算回数の最大値。
                   std::ostream *dout = 0 // デバッグ出力
                   ){
  using namespace std;
  
  if( dout ) *dout << "--- MyDownhillSimplex() ---" << endl;

  // 固定パラメータ
  const double ALPHA = 1; // 反射の大きさ
  const double GAMMA = 2; // 拡張の大きさ
  const double RHO = 0.5; // 収縮の大きさ
  const double SIGMA = 0.5; // シンプレックス縮小の大きさ
  const double INIT_SMP_SIZE_PCT = 5; // 初期値の何％の範囲でシンプレックスを生成するか
  const double INIT_SMP_SIZE_WHEN_ZERO = 0.00025; // 上記で初期値がゼロだった場合に代わりに使う値
  
  // シンプレックス
  typedef multimap< double, vector< double > > map_type;
  map_type smp;

  // 次元
  int n = x_init.size();
  
  // 諸変数
  vector< double > x_in;
  double x_out;

  // --- 初期値の周りに乱数でシンプレックスを生成 ---

  for( int i = 0; i < n; i++ ){
    x_in.clear();
    for( int j = 0; j < n; j++ ){
      double rand_val = ( rand() / (double)RAND_MAX - 0.5 ) * 2;
      double size = MyAbs( x_init[ j ] * INIT_SMP_SIZE_PCT / 100.0 );
      double val = x_init[ j ] + rand_val * size;
      if( x_init[ j ] == 0 ) val = INIT_SMP_SIZE_WHEN_ZERO;
      x_in.push_back( val );
    }
    x_out = fx( x_in );
    smp.insert( make_pair< double, vector< double > >( x_out, x_in ) );
  }

  // 重心が初期値になるように最後の頂点位置を決定
  x_in.clear();
  for( int i = 0; i < n; i++ ){
    double sum = 0;
    for( map_type::const_iterator it = smp.begin(); it != smp.end(); it++ ){
      sum += it->second[ i ];
    }
    double val = ( n + 1 ) * x_init[ i ] - sum;
    x_in.push_back( val );
  }
  x_out = fx( x_in );
  smp.insert( make_pair< double, vector< double > >( x_out, x_in ) );

  // --- 探索処理 ---

  int itr_count = 0; 
  while( itr_count < max_itr_num ){

    // 現時点での解
    vector< double > x_min = (smp.begin())->second;

    // その位置での関数値
    double f_min = (smp.begin())->first;

    if( dout ){
      *dout << "[" << itr_count << "]\t f_min: " << f_min << endl;
      *dout << "\t x_min: " << x_min << endl;
    }

    // 繰り返し回数のカウント
    itr_count++;

    // --- 収束判定 ---

    // シンプレックスの重心を求める
    vector< double > smp_cent( n, 0 );
    for( map_type::const_iterator it = smp.begin(); it != smp.end(); it++ ){
      smp_cent = smp_cent + it->second;
    }
    smp_cent = smp_cent / (double)( smp.size() );

    // 重心点からのシンプレックスの各頂点の距離の平均値をシンプレックスサイズとする
    double smp_size = 0;
    for( map_type::const_iterator it = smp.begin(); it != smp.end(); it++ ){
      smp_size += MyVecNorm( it->second - smp_cent );
    }
    smp_size /= (double)( smp.size() );
    
    if( dout ) *dout << "\t smp_size: " << smp_size << endl;

    // シンプレックスサイズが閾値を下回れば、計算終了
    if( smp_size < thres ){
      break;
    }

    if( dout ){
      *dout << "\t simplex:" << endl;
      for( map_type::const_iterator it = smp.begin(); it != smp.end(); it++ ){
        *dout << "\t\t" << it->first << "\t (" << it->second << ")" << endl;
      }
    }
    
    // --- Step.1：反射 ---

    // シンプレックスの各頂点のうち、関数値が最大となる頂点を除いた残りの頂点の重心を求める
    vector< double > x_c( n, 0 );
    map_type::iterator it = smp.begin();
    for( int j = 0; j < smp.size() - 1; j++ ){
      x_c = x_c + it->second;
      it++;
    }
    x_c = x_c / (double)n;

    // その重心位置に対象に、関数値が最大となっている頂点位置を移動
    vector< double > x_max = (smp.rbegin())->second;

    // 移動量は乱数で決める。場合によって ”振動” してデッドロックになることを防ぐため。
    double alpha = ALPHA + ( rand() / (double)RAND_MAX );
    // double alpha = ALPHA; // 乱数を使わない場合は、この行を使う。（使わない実装も一般的）
    vector< double > x_ref = x_c + alpha * ( x_c - x_max );

    // 移動した先での関数値を求める。
    double f_ref, f_max;
    f_ref = fx( x_ref );

    // 現在の（移動前の）関数の最大値
    f_max = fx( x_max );
    
    if( dout ){
      *dout << "\t Step.1:" << endl;
      *dout << "\t\t x_c: " << x_c << endl;
      *dout << "\t\t x_max: " << x_max << endl;
      *dout << "\t\t x_ref: " << x_ref << endl;
      *dout << "\t\t f_ref: " << f_ref << "\t f_max: " << f_max << endl;
      if( f_ref <= f_max ) *dout << "\t\t f_ref(" << f_ref << ") <= f_max(" << f_max << ")" << endl;
      else *dout << "\t\t f_ref(" << f_ref << ") > f_max(" << f_max << ")" << endl;
    }

    // もし移動によって、関数値が減少していたら...
    if( f_ref <= f_max ){
      
      // --- Step.2：拡張 ---

      // さらに関数値が減少することを狙って、同じ方向にさらに移動する。
      // また乱数で移動量を決めることにする。
      //vector< double > x_exp = x_c + GAMMA * ( x_c - x_max );
      double gamma = GAMMA + ( rand() / (double)RAND_MAX );
      // double gamma = GAMMA; // 乱数を使わない場合はこっち。
      vector< double > x_exp = x_c + gamma * ( x_c - x_max );

      // 移動先での関数値を求める
      double f_exp;
      f_exp = fx( x_exp );

      // シンプレックスから関数値が最大となっていた頂点を削除
      it = smp.begin();
      for( int i = 0; i < smp.size() - 1; i++ ) it++;
      smp.erase( it );

      // 拡張した先での関数値がさらに小さくなっていたら、拡張先の位置で頂点を入れ替え
      if( f_ref <= f_exp ){
        smp.insert( make_pair< double, vector< double > >( f_ref, x_ref ) );
      }
      // そうでなかった場合は、拡張前の位置（反射点）で頂点を入れ替え
      else{
        smp.insert( make_pair< double, vector< double > >( f_exp, x_exp ) );
      }

      if( dout ){
        *dout << "\t Step.2:" << endl;
        *dout << "\t\t x_exp: " << x_exp << endl;
        *dout << "\t\t f_ref: " << f_ref << "\t f_exp: " << f_exp << endl;
        if( f_ref <= f_exp ){
          *dout << "\t\t f_ref(" << f_ref << ") <= f_exp(" << f_exp << ")" << endl;
          *dout << "\t\t x_max(" << x_max << ") ---> x_ref(" << x_ref << ")" << endl;
        }
        else{
          *dout << "\t\t f_ref(" << f_ref << ") > f_exp(" << f_exp << ")" << endl;
          *dout << "\t\t x_max(" << x_max << ") ---> x_exp(" << x_exp << ")" << endl;
        }
      }

      // 次のステップへ
      continue;
    }

    // 関数値が逆に増加していたら...
    else{
      
      // --- Step.3：収縮 ---

      // 行き過ぎたので、移動量を縮める。
      // ここでも移動量は乱数で決める。
      //vector< double > x_con = x_c + RHO * ( x_c - x_max );
      double rho = RHO + rand() / (double)RAND_MAX;
      // double rho = RHO; // 乱数を使わない場合はこっち
      vector< double > x_con = x_c + rho * ( x_c - x_max );

      // 縮小先での関数値
      double f_con;
      f_con = fx( x_con );

      if( dout ){
        *dout << "\t Step.3:" << endl;
        *dout << "\t\t x_con: " << x_con << endl;
        *dout << "\t\t f_con: " << f_con << endl;
        *dout << "\t\t f_max: " << f_max << endl;
        if( f_con <= f_max ){
          *dout << "\t\t f_con(" << f_con << ") <= f_max(" << f_max << ")" << endl;
        }
        else{
          *dout << "\t\t f_con(" << f_con << ") > f_max(" << f_max << ")" << endl;
        }
      }

      // 縮小によって無事関数値が減少していたら...
      if( f_con <= f_max ){

        // シンプレックスの頂点のうち、関数値が最大となっているものを削除
        it = smp.begin();
        for( int i = 0; i < smp.size() - 1; i++ ) it++;
        smp.erase( it );

        // 縮小先の位置を頂点に加える
        smp.insert( make_pair< double, vector< double > >( f_con, x_con ) );
        
        if( dout ){
          *dout << "\t\t x_max(" << x_max << ") ---> x_con(" << x_con << ")" << endl;
        }

        // 次のステップへ
        continue;
      }

      // それでも関数値が減少しなかったら
      else{
        
        // --- Step.4：シンプレックス収縮 ---

        // あらたにシンプレックスを用意
        map_type smp_new;

        // 現時点の最小値を与える頂点だけはそのまま利用
        smp_new.insert( make_pair< double, vector< double > >( f_min, x_min ) );
        it = smp.begin();
        it++;

        // その他の頂点は、現在の頂点位置からそれぞれ縮小した位置に頂点を移動
        for( int i = 1; i < smp.size(); i++ ){
          vector< double > x_old = it->second;
          vector< double > x_new = x_min + SIGMA * ( x_old - x_min );
          double f_new;
          f_new = fx( x_new );
          smp_new.insert( make_pair< double, vector< double > >( f_new, x_new ) );
          it++;
        }

        // 新たに作ったシンプレックスと現在のシンプレックスを入れ替え
        smp.swap( smp_new );

        if( dout ){
          *dout << "\t Step.4" << endl;
          map_type::const_iterator it1 = smp_new.begin();
          map_type::const_iterator it2 = smp.begin();
          for( int i = 0; i < smp.size(); i++ ){
            *dout << "\t\t" << it1->second << " ---> " << it2->second << endl;
            it1++;
            it2++;
          }
        }

        // 次のステップへ
        
      }// else
    }// else

  }// while 
  
  // 出力値にセット
  out = (smp.begin())->second;

  return 0;
}

//----------------------------------------------------------------------------
// MyLineSearch
// ・fx( x + t * dx ) を最小にする t の値を返す
// ・x, dx は、ベクトル
//----------------------------------------------------------------------------

// 内部で使う１変数の最小値検索アルゴリズムの種類
typedef enum {
  GoldenSection = 0,
  GradientBased,
  DownhillSimplex
} MyLineSearch_MethodType;

// 内部で使う関数
const std::vector< double > * g_MyLineSearch_x;
const std::vector< double > * g_MyLineSearch_dx;
double (*g_MyLineSearch_fx)( const std::vector< double > & );
double MyLineSearch_Ft( double t ){
  return g_MyLineSearch_fx( (*g_MyLineSearch_x) + t * (*g_MyLineSearch_dx) );
}
double MyLineSearch_Ft2( const std::vector< double > &t ){
  assert( t.size() > 0 );
  return g_MyLineSearch_fx( (*g_MyLineSearch_x) + t[0] * (*g_MyLineSearch_dx) );
}

int
MyLineSearch( double (*fx)( const std::vector< double > &), // 評価関数
              const std::vector< double > &x, // 初期位置
              const std::vector< double > &dx, // 検索方向
              double *out, // 出力値
              MyLineSearch_MethodType type = GoldenSection, // 探索アルゴリズム
              double thres = 1E-06, // 検索終了のしきい値
              int max_itr_num = 100, // 繰り返し計算の最大回数
              std::ostream *dout = 0 // デバッグ用出力
              ){
  using namespace std;

  if( dout ) *dout << "--- MyLineSearch() ---" << endl;

  // F(t) = fx( x + t * dx ) のセットアップ
  g_MyLineSearch_x = &x;
  g_MyLineSearch_dx = &dx;
  g_MyLineSearch_fx = fx;
  double (*ft)( double ) = MyLineSearch_Ft;
  double (*ft2)( const std::vector< double > & ) = MyLineSearch_Ft2;
  
  switch( type ){
    case GradientBased:
      {
        double t_init = 0.0; // t の初期値
        assert( ! MyGradientBasedSearch( ft, t_init, out, thres, max_itr_num, dout ) );
      }
      break;
    case GoldenSection:
      {
        // --- 検索範囲を決める ---
        const double STEP = 0.00025; // いくつか適当かわからない。値が大きいほど局所解を巻き込みやすい。
        const int MAX_ITR = 25;
        double a = -STEP;
        double b = STEP;
        double fa = ft( a );
        double ff = ft( 0 );
        double fb = ft( b );
        if( dout ){
          *dout << "[init]\t";
          *dout << "f(" << a << ") = " << fa << ",\t";
          *dout << "f(0) = " << ff << ",\t";
          *dout << "f(" << b << ") = " << fb << endl;
        }
        if( fa <= ff && ff < fb ){
          for( int i = 0; i < 25; i++ ){
            a *= 2;
            fa = ft( a );
            if( fa > ff ) break;
          }
          assert( fa > ff );
        }
        else if( fa > ff && ff >= fb ){
          for( int i = 0; i < MAX_ITR; i++ ){
            b *= 2;
            fb = ft( b );
            if( fb > ff ) break;
          }
          assert( fb > ff );
        }
        else if( fa <= ff && ff >= fb ){
          for( int i = 0; i < MAX_ITR; i++ ){
            a *= 2;
            fa = ft( a );
            if( fa > ff ) break;
          }
          assert( fa > ff );
          for( int i = 0; i < MAX_ITR; i++ ){
            b *= 2;
            fb = ft( b );
            if( fb > ff ) break;
          }
          assert( fb > ff );
        }
        assert( fa > ff && ff < fb );
        if( dout ){
          *dout << "[start]\t";
          *dout << "f(" << a << ") = " << fa << ",\t";
          *dout << "f(0) = " << ff << ",\t";
          *dout << "f(" << b << ") = " << fb << endl;
        }
        // --- 探索実行 ---
        assert( ! MyGoldenSection( ft, a, b, out, thres, max_itr_num, dout ) );
      }
      break;
    case DownhillSimplex:
      {
        // １次元の Downhill Simplex
        vector< double > t_init( 1, 0 );
        vector< double > t_out( 1 );
        assert( ! MyDownhillSimplex( ft2, t_init, t_out, thres, max_itr_num, dout ) );
        *out = t_out[ 0 ];
      }
      break;
  }

  return 0;
}

//----------------------------------------------------------------------------
// MySteepestDescent
// ・最急降下法
// ・関数 double f( const vector< double > &x ) の値を最小にする入力 x を求める
//----------------------------------------------------------------------------

int
MySteepestDescent( double (*fx)( const std::vector< double > &), // 評価関数
                   std::vector< double > &x, // 出力値。最初は初期値を入れておく。
                   MyLineSearch_MethodType type = GoldenSection, // 内部で使う直線探索のアルゴリズム
                   double thres = 1E-06, // 検索終了のしきい値
                   int max_itr_num = 100, // 繰り返し計算の最大回数
                   std::ostream *dout = 0 // デバッグ用出力
                   ){
  using namespace std;

  // 繰り返し数のカウンター
  int itr_count = 0;

  // 探索処理
  while( itr_count++ < max_itr_num ){

    // 現在位置での勾配
    vector< double > x_grad;
    assert( ! MyVecGrad( fx, x, x_grad ) );

    // 勾配方向に直線検索
    double t;
    assert( ! MyLineSearch( fx, x, x_grad, &t, type ) );

    // 位置の更新
    vector< double > dx = t * x_grad;
    x = x + dx;

    if( dout ){
      if( itr_count == 1 ) *dout << "--- MySteepestDescent() ---" << endl;
      *dout << "[" << itr_count << "] ";
      *dout << "x: " << (x - dx) << " ";
      *dout << "x_grad: " << x_grad << " ";
      *dout << "t: " << t << " ";
      *dout << "dx: " << dx << " ";
      *dout << "norm(dx): " << MyVecNorm( dx ) << " ";
      *dout << "x updated: " << x << endl;
    }
    
    // 終了判定
    if( MyVecNorm( dx ) < thres ) break;
    
  } // while

  return 0;
}

}// my

#endif
