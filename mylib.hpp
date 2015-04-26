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
const char *VERSION = "2015.4.26";
  
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
T
MyAbs( T a ){
  return (a > 0) ? a : -a;
}

//----------------------------------------------------------------------------
// MyAbsDiff
//----------------------------------------------------------------------------

template < class T >
T
MyAbsDiff( T a, T b ){
  return (a > b) ? a - b : b - a;
}

//----------------------------------------------------------------------------
// MyMin
//----------------------------------------------------------------------------

template < class T >
T
MyMin( T a, T b ){
  return (a < b) ? a : b;
}

template < class T >
T
MyMin( T a, T b, T c ){
  if( a < b && a < c ) return a;
  else return MyMin< T >( b, c );
}

//----------------------------------------------------------------------------
// MyMax
//----------------------------------------------------------------------------

template < class T >
T
MyMax( T a, T b ){
  return (a > b) ? a : b;
}

template < class T >
T
MyMax( T a, T b, T c ){
  if( a > b && a > c ) return a;
  else return MyMax< T >( b, c );
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
MyEndsWith( std::string const& str, 
            std::string const& ext // 拡張子など
            ){
  return ext.size() <= str.size() && str.find(ext, str.size() - ext.size()) != str.npos;
}
  
bool 
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
MyBeginWith( const std::string &str,
             const std::string &start ){
  return str.find( start ) == 0;
}

bool
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
MyGetFileName( const std::string &filepath ){
  size_t idx = filepath.find_last_of( '/' );
  if (idx == std::string::npos) return filepath;
  else return filepath.substr( idx + 1, filepath.size() );
}
std::string
MyGetFileName( const char *filepath ){
  return MyGetFileName( std::string( filepath ) );
}

//----------------------------------------------------------------------------
// MyGetFileNameBase
// ・ファイル名の中から拡張子を除いた部分を返す。
//----------------------------------------------------------------------------

std::string
MyGetFileNameBase( const std::string &filepath ){
  size_t idx = filepath.find_last_of( '.' );
  if (idx == std::string::npos) return filepath;
  else return filepath.substr( 0, idx );
}
std::string
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
    vector<string> la = my::MySplit( line );
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

}// my

#endif
