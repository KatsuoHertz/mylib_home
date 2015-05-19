/**
 * @file mylib_home.hpp
 * @brief 各種便利関数、クラス
 * @version 2015.5.16
 */

#ifndef MYLIB_HOME_HPP
#define MYLIB_HOME_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <cv.h>
#include <highgui.h>

namespace my
{

// --- 目次 ---
// 1. 便利クラス、便利関数
// 2. ベクトル、行列操作
// 3. 数学
// 4. 最小化
// 5. 画像操作
// 6. 画像処理

//#########################################################################################
// 便利クラス、便利関数
//#########################################################################################

/**
 * ２次元座標値
 */

template < typename T >
struct MyPoint2 {
  T x;
  T y;
  MyPoint2() : x( 0 ), y( 0 ){ }
  MyPoint2( T x0, T y0 ) : x( x0 ), y( y0 ){ }
  bool operator == ( const MyPoint2 &rhs ) const {
    return this->x == rhs.x && this->y == rhs.y;
  }
  bool operator != ( const MyPoint2 &rhs ) const {
    return this->x != rhs.x || this->y != rhs.y;
  }
};

/**
 * ２次元座標値の表示
 */
template < typename T >
inline
std::ostream & operator << ( std::ostream &os, const MyPoint2<T> &p )
{
  os << p.x << "\t" << p.y;
  return os;
}

typedef MyPoint2<int> MyPoint2i;
typedef MyPoint2<double> MyPoint2d;
typedef MyPoint2<short> MyPoint2s;

/**
 * RGB の画素値
 */

template < typename T >
struct MyColor3 {
  T r;
  T g;
  T b;
  MyColor3(){};
  MyColor3( T R, T G, T B ) : r( R ), g( G ), b( B ) {};
  MyColor3( const IplImage *img_in, int x, int y ){
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
  bool operator == ( const MyColor3 &rhs ) const {
    return this->r == rhs.r && this->g == rhs.g && this->b == rhs.b;
  }
  bool operator != ( const MyColor3 &rhs ) const {
    return this->r != rhs.r || this->g != rhs.g || this->b != rhs.b;
  }
  bool operator < ( const MyColor3 &rhs ) const {
    if( this->r < rhs.r ) return true;
    else if( this->r > rhs.r ) return false;
    else{
      if( this->g < rhs.g ) return true;
      else if( this->g > rhs.g ) return false;
      else return this->b < rhs.b;
    }
  }
};

/**
 * 構造体 MyColor3 の表示
 */

template < typename T >
inline
std::ostream & operator << ( std::ostream &os, const MyColor3<T> &cl )
{
  os << cl.r << "\t" << cl.g << "\t" << cl.b;
  return os;
}

/**
 * 画素値のソート用コンパレータ
 */
template < typename T >
struct MyCmpColor3 {
  bool operator () ( const MyColor3<T> &a, const MyColor3<T> &b ) const{
    return a < b;
  }
};

typedef MyColor3<int> MyColor3i;
typedef MyColor3<short> MyColor3s;
typedef MyColor3<unsigned short> MyColor3us;
typedef MyColor3<double> MyColor3d;
typedef MyCmpColor3<unsigned short> MyCmpColor3us;
typedef MyCmpColor3<double> MyCmpColor3d;

/**
 * 絶対値を返す
 */
template < class T >
inline
T
MyAbs( T a ){
  return (a > 0) ? a : -a;
}

/**
 * 差分の絶対値を返す
 */
template < class T >
inline
T
MyAbsDiff( T a, T b ){
  return (a > b) ? a - b : b - a;
}

/**
 * 小さい方を返す
 */
template < class T >
inline
T
MyMin( T a, T b ){
  return (a < b) ? a : b;
}

/**
 * 最小のものを返す
 */
template < class T >
inline
T
MyMin( T a, T b, T c ){
  if( a < b && a < c ) return a;
  else return MyMin< T >( b, c );
}

/**
 * 大きい方を返す
 */
template < class T >
inline
T
MyMax( T a, T b ){
  return (a > b) ? a : b;
}

/**
 * 最大のものを返す
 */
template < class T >
inline
T
MyMax( T a, T b, T c ){
  if( a > b && a > c ) return a;
  else return MyMax< T >( b, c );
}

/**
 * 符号を返す
 * - 引数が正の場合は、１
 * - 引数がゼロの場合は、０
 * - 引数が負の場合は、−１
 */
template < typename T >
inline
T MySgn( T x ){
  return x == 0 ? 0 : ( x > 0 ? 1 : -1 );
}

/**
 * 文字列分割
 * - タブ、空白で分割
 */
std::vector<std::string> 
MySplit( const std::string &str 
         ){
  using namespace std;
  istringstream iss(str); vector<string> res;
  copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(res));
  return res;
}

/**
 * 指定した文字列で終わっているかどうか
 */
bool 
inline
MyEndsWith( std::string const& str, 
            std::string const& ext // 拡張子など
            ){
  return ext.size() <= str.size() && str.find(ext, str.size() - ext.size()) != str.npos;
}
  
/**
 * 指定した文字列で終わっているかどうか
 * - const char * 版
 */
bool 
inline
MyEndsWith( const char *str, 
            const char *ext // 拡張子など
            ){
  return MyEndsWith( std::string( str ), std::string( ext ) );
}

/**
 * 指定した文字列で始まっているかどうか
 */
bool
inline
MyBeginWith( const std::string &str,
             const std::string &start ){
  return str.find( start ) == 0;
}

/**
 * 指定した文字列で始まっているかどうか
 * - const char * 版
 */
bool
inline
MyBeginsWith( const char * str,
             const char * start ){
  return std::string( str ).find( std::string( start ) ) == 0;
}

/**
 * 引数解析
 * - 引数配列の中から指定した文字列と一致する要素のインデックスを返す。
 */
int
MyFindOption( int argc, 
              char *argv[], 
              const char *option_str // 検索文字列
              ){
  for( int i = 0; i < argc; i++ ) 
    if( std::string( argv[ i ] ) == std::string( option_str ) ) return i;
  return 0;
}

/**
 * ファイル名取得
 * - ファイルパスの文字列の中からパスの部分を省いたファイル名部分だけを返す。
 */
std::string 
inline
MyGetFileName( const std::string &filepath ){
  size_t idx = filepath.find_last_of( '/' );
  if (idx == std::string::npos) return filepath;
  else return filepath.substr( idx + 1, filepath.size() );
}

/**
 * ファイル名取得
 * - const char * 版
 */
std::string
inline
MyGetFileName( const char *filepath ){
  return MyGetFileName( std::string( filepath ) );
}

/**
 * 拡張子抜きのファイル名取得
 * - ファイル名の中から拡張子を除いた部分を返す。
 */ 
std::string
inline
MyGetFileNameBase( const std::string &filepath ){
  size_t idx = filepath.find_last_of( '.' );
  if (idx == std::string::npos) return filepath;
  else return filepath.substr( 0, idx );
}

/**
 * 拡張子抜きのファイル名取得
 * - const char * 版
 */ 
std::string
inline
MyGetFileNameBase( const char *filepath ){
  return MyGetFileNameBase( std::string( filepath ) );
}

/**
 * テキストデータの読み込み
 * - 各行について空白文字で区切られた文字列の配列を読み込む。
 * @param [out] data_buf 読み取った結果が string 型の二次元配列として入る。
 * @param col_num 列数の指定。指定された列数の文字列が存在する行しか読まない。
 */
int
MyReadTextString( const char *filepath, 
                  std::vector< std::vector< std::string > > &data_buf,
                  int col_num = 0
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
      data_buf.push_back( la );
    }
  }
  fin.close();
  return 0;
}

/**
 * テキストデータの読み込み（整数版）
 * - 各行について空白文字で区切られた数値が並ぶテキストファイルを読み込む
 * - atoi() で整数変換される。
 * @param [out] data_buf 読み取った結果が int 型の二次元配列として入る。
 * @param col_num 列数の指定。指定された列数の文字列が存在する行しか読まない。
 */
int
MyReadTextInt( const char *filepath, 
               std::vector< std::vector< int > > &data_buf,
               int col_num = 0
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
      vector< int > tmp;
      for (int i = 0; i < la.size(); i++) tmp.push_back( atoi( la[ i ].c_str() ) );
      data_buf.push_back( tmp );
    }
  }
  fin.close();
  return 0;
}

/**
 * テキストデータの読み込み（実数版）
 * - 各行について空白文字で区切られた数値が並ぶテキストファイルを読み込む
 * - atof() で実数変換される。
 * @param [out] data_buf 読み取った結果が double 型の二次元配列として入る。
 * @param col_num 列数の指定。指定された列数の文字列が存在する行しか読まない。
 */
int
MyReadTextDouble( const char *filepath, 
                  std::vector< std::vector< double > > &data_buf,
                  int col_num = 0
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
      vector< double > tmp;
      for (int i = 0; i < la.size(); i++) tmp.push_back( atof( la[ i ].c_str() ) );
      data_buf.push_back( tmp );
    }
  }
  fin.close();
  return 0;
}

/**
 * テキストデータの読み込み
 * - 各行について key と value の書かれたテキストファイルを map として読み込む
 * - 各行の最初の２列だけ。３列目以降は無視。
 * - key と value のどちらも string 型として読み込まれる。
 */
int
MyReadTextMapString( const char *filepath, 
                     std::map< std::string, std::string > &data_buf
                     ){
  using namespace std;
  ifstream fin( filepath );
  if( !fin ) return -1;
  while (fin.good()) {
    string line;
    getline( fin, line );
    vector<string> la = MySplit( line );
    if( la.size() >= 2 ) data_buf[ la[ 0 ] ] = la[ 1 ];
  }
  fin.close();
  return 0;
}

//#########################################################################################
// ベクトル、行列操作
//#########################################################################################

/**
 * ベクトルの足し算
 */
template < typename T >
inline
std::vector< T > operator + ( const std::vector< T > &a,
                              const std::vector< T > &b ){
  assert( a.size() == b.size() );
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = a[ i ] + b[ i ];
  return c;
}

/**
 * ベクトルの引き算
 */
template < typename T >
inline
std::vector< T > operator - ( const std::vector< T > &a,
                              const std::vector< T > &b ){
  assert( a.size() == b.size() );
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = a[ i ] - b[ i ];
  return c;
}

/**
 * ベクトルのスカラー倍
 */
template < typename T >
inline
std::vector< T > operator * ( const T k,
                              const std::vector< T > &a ){
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = k * a[ i ];
  return c;
}

/**
 * ベクトルのスカラー割り
 */
template < typename T >
inline
std::vector< T > operator / ( const std::vector< T > &a,
                              const T k ){
  assert( k != 0 );
  std::vector< T > c( a.size() );
  for( int i = 0; i < a.size(); i++ ) c[ i ] = a[ i ] / k;
  return c;
}

/**
 * ベクトルの表示
 */
template < typename T >
inline
std::ostream & operator << ( std::ostream &os,
                             const std::vector< T > &a ){
  for( int i = 0; i < a.size(); i++ ) os << a[ i ] << "\t";
  return os;
}

/**
 * ベクトルのノルムを返す
 */
template < typename T >
inline T MyVecNorm( const std::vector< T > &a ){
  T sum = 0.0;
  for( int i = 0; i < a.size(); i++ ){
    sum += a[ i ] * a[ i ];
  }
  return sqrt( sum );
}

/**
 * ベクトルの内積を計算
 */
template < typename T >
inline
T MyVecDot( const std::vector< T > &a, const std::vector< T > &b ){
  assert( a.size() == b.size() );
  T sum = 0.0;
  for( int i = 0; i < a.size(); i++ ){
    sum += a[ i ] * b[ i ];
  }
  return sum;
}

/**
 * ベクトルの要素のヒストグラムを計算
 */
template < typename T >
void
MyVecHist( const std::vector< T > &data,
           std::map< T, int > &hist
           ){
  using namespace std;
  hist.clear();
  for( typename vector< T >::const_iterator it = data.begin(); it != data.end(); it++ ){
    if( hist.find( *it ) == hist.end() ) hist[ *it ] = 1;
    else hist[ *it ] = hist[ *it ] + 1;
  }
}

/**
 * ベクトルの要素の中から最頻の要素を返す
 * @param[out] count 頻度（出現回数）が入る。
 * @return 最頻の要素の値
 */
template < typename T >
T
MyVecMostFrequentElem( const std::vector< T > &data,
                       int *count = NULL // 最頻値の数が入る
                       ){
  using namespace std;
  map< T, int > hist;
  MyVecHist( data, hist );
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

/**
 * 行列の行数と列数を構造体にまとめて返す
 * - 行列が矩形かどうかはノーチェック（行列の第１行目しか見ない）
 */
inline
MyPoint2< int >
MyMatSize( const std::vector< std::vector< double > > &A ){
  return A.empty() ? MyPoint2< int >( 0, 0 ) : MyPoint2< int >( A.size(), A[ 0 ].size() );
}

/**
 * 正方行列かどうかのチェック
 */
inline
bool
MyMatIsSquare( const std::vector< std::vector< double > > &A ){
  for( int i = 0; i < A.size(); i++ ) if( A[ i ].size() != A.size() ) return false;
  return true;
}

/**
 * 行列が矩形（列数が行によって変化しない）かどうかのチェック
 */
inline
bool
MyMatIsRect( const std::vector< std::vector< double > > &A ){
  for( int i = 0; i < A.size(); i++ ) if( A[ i ].size() != A[ 0 ].size() ) return false;
  return true;
}

/**
 * ２つの行列の行数と列数が同じかどうかのチェック
 * - 全行全列について調べる
 */
inline
bool
MyMatAreTheSameSize( const std::vector< std::vector< double > > &A,
                     const std::vector< std::vector< double > > &B ){
  return MyMatIsRect( A ) && MyMatIsRect( B ) && MyMatSize( A ) == MyMatSize( B );
}

/**
 * 行列の足し算
 */

template < typename T >
inline
std::vector< std::vector< T > >
operator + ( const std::vector< std::vector< T > > &A,
             const std::vector< std::vector< T > > &B ){
  assert( MyMatSize( A ) == MyMatSize( B ) );
  using namespace std;
  int M = A.size();
  int N = A[ 0 ].size();
  vector< vector< T > > C( M, vector< T >( N ) );
  for( int i = 0; i < M; i++ ){
    for( int j = 0; j < N; j++ ){
      C[ i ][ j ] = A[ i ][ j ] + B[ i ][ j ];
    }
  }
  return C;
}

/**
 * 行列の引き算
 */

template < typename T >
inline
std::vector< std::vector< T > >
operator - ( const std::vector< std::vector< T > > &A,
             const std::vector< std::vector< T > > &B ){
  assert( MyMatSize( A ) == MyMatSize( B ) );
  using namespace std;
  int M = A.size();
  int N = A[ 0 ].size();
  assert( M > 0 );
  assert( M == B.size() );
  assert( N == B[ 0 ].size() );
  vector< vector< T > > C( M, vector< T >( N ) );
  for( int i = 0; i < M; i++ ){
    for( int j = 0; j < N; j++ ){
      C[ i ][ j ] = A[ i ][ j ] - B[ i ][ j ];
    }
  }
  return C;
}

/**
 * 行列の掛け算
 */
template < typename T >
inline
std::vector< std::vector< T > >
operator * ( const std::vector< std::vector< T > > &A,
             const std::vector< std::vector< T > > &B ){
  using namespace std;
  int M = A.size();
  assert( M > 0 );
  int N = A[ 0 ].size();
  assert( N == B.size() );
  assert( N > 0 );
  int L = B[ 0 ].size();
  assert( L > 0 );
  // M x N, N x L
  vector< vector< T > > C( M, vector< T >( L ) );
  for( int i = 0; i < M; i++ ){
    for( int j = 0; j < L; j++ ){
      C[ i ][ j ] = 0;
      for( int k = 0; k < N; k++ ){
        C[ i ][ j ] += A[ i ][ k ] * B[ k ][ j ];
      }
    }
  }
  return C;
}

/**
 * 行列のスカラー倍
 */

template < typename T >
inline
std::vector< std::vector< T > >
operator * ( T k,
             const std::vector< std::vector< T > > &A ){
  using namespace std;
  int M = A.size();
  assert( M > 0 );
  int N = A[ 0 ].size();
  assert( N > 0 );
  vector< vector< T > > C( M, vector< T >( N ) );
  for( int i = 0; i < M; i++ ){
    for( int j = 0; j < N; j++ ){
      C[ i ][ j ] = k * A[ i ][ j ];
    }
  }
  return C;
}

/** 
 * 行列の転置
 */

template < typename T >
inline
std::vector< std::vector< T > >
MyMatTrans( const std::vector< std::vector< T > > &A ){
  using namespace std;
  int M = A.size();
  assert( M > 0 );
  int N = A[ 0 ].size();
  assert( N > 0 );
  vector< vector< double > > C( N, vector< T >( M ) );
  for( int i = 0; i < N; i++ ){
    for( int j = 0; j < M; j++ ){
      C[ i ][ j ] = A[ j ][ i ];
    }
  }
  return C;
}

/** 
 * 行列の表示
 */

template < typename T >
inline
std::ostream & operator << ( std::ostream &os,
                             const std::vector< std::vector< T > > &A)
{
  int M = A.size();
  int N = A[ 0 ].size();
  for( int i = 0; i < M; i++ ){
    for( int j = 0; j < N; j++ ){
      os << A[ i ][ j ] << "\t";
    }
    os << std::endl;
  }
  return os;
}

/**
 * 行列とベクトルの掛け算
 * - ベクトルになる
 * - M x N * N x 1 => M x 1
 */
template < typename T >
inline
std::vector< T >
operator * ( const std::vector< std::vector< T > > &A,
             const std::vector< T > &x
              ){
  using namespace std;
  // M x N, N x 1
  int M = A.size();
  assert( M > 0 );
  int N = A[ 0 ].size();
  assert( N == x.size() );
  assert( N > 0 );
  vector< T > b( N, 0 );
  for( int i = 0; i < M; i++ ){
    for( int j = 0; j < N; j++ ){
      b[ i ] += A[ i ][ j ] * x[ j ];
    }
  }
  return b;
}

/** 
 * ベクトルの転置
 * - 横１列の行列になる
 * - N x 1 ベクトル => 1 x N 行列
 */
template < typename T >
inline
std::vector< std::vector< T > >
MyVecTrans( const std::vector< T > &x ){
  using namespace std;
  int N = x.size();
  assert( N > 0 );
  vector< vector< double > > A( 1, vector< T >( N ) );
  for( int i = 0; i < N; i++ ){
    A[ 0 ][ i ] = x[ i ];
  }
  return A;
}

/** 
 * ベクトルの行列化
 * - 縦１列の行列になる
 */
template < typename T >
inline
std::vector< std::vector< T > >
MyVec2Mat( const std::vector< T > &x ){
  using namespace std;
  int N = x.size();
  assert( N > 0 );
  vector< vector< double > > A( N, vector< T >( 1 ) );
  for( int i = 0; i < N; i++ ){
    A[ i ][ 0 ] = x[ i ];
  }
  return A;
}

/** 
 * ゼロベクトルを返す
 */
template < typename T >
inline
std::vector< T >
MyVecZero( int n ){
  return std::vector< T >( n, 0 );
}

/** 
 * ゼロ行列を返す
 */
template < typename T >
inline
std::vector< std::vector< T > >
MyMatZero( int n ){
  return std::vector< std::vector< T > >( n, std::vector< T >( n, 0 ) );
}

/** 
 * 単位行列を返す
 */
template < typename T >
inline
std::vector< std::vector< T > >
MyMatIdentity( int n ){
  std::vector< std::vector< T > > A( n, std::vector< T >( n, 0 ) );
  for( int i = 0; i < n; i++ ) A[ i ][ i ] = 1;
  return A;
}

/**
 * 行列から指定された位置の列ベクトルを取り出す
 */
inline
std::vector< double >
MyGetColVector( const std::vector< std::vector< double > > &A,
                int col_index ){
  assert( A.size() > 0 );
  assert( MyMatIsRect( A ) );
  int row = A.size();
  int col = A[ 0 ].size();
  assert( col_index >= 0 && col_index < col );
  std::vector< double > col_vec( row );
  for( int i = 0; i < row; i++ ){
    col_vec[ i ] = A[ i ][ col_index ];
  }
  return col_vec;
}

/**
 * 列ベクトルの配列を返す。
 *
 * 行列の転置と同じ
 */
inline
std::vector< std::vector< double > >
MyGetColVectors( const std::vector< std::vector< double > > &A ){
  return MyMatTrans( A );
}

/**
 * 行列の対角成分を返す。
 */
inline
std::vector< double >
MyGetDiagVector( const std::vector< std::vector< double > > &A ){
  assert( MyMatIsRect( A ) );
  int n = A.size();
  std::vector< double > diag_vec( n );
  for( int i = 0; i < n; i++ ){
    diag_vec[ i ] = A[ i ][ i ];
  }
  return diag_vec;
}

//#########################################################################################
// 数学
//#########################################################################################

/**
 * １変数関数の数値微分を計算
 * - T fx( T x ) な関数
 * - 中心差分
 * @param fx 関数へのポインタ
 * @param x この値での微分値を計算
 * @param[out] out 出力値
 * @param h 微分計算時に用いる微小変化量。デフォルトは、1E-6
 */
template < typename T >
inline int MyCalcGrad( T (*fx)( T ), T x, T *out, T h = 1E-06 ){

  // h が 0 だとゼロ割になってしまうのでダメ
  assert( h != 0 );

  // 両隣の関数値の変化から微分値を計算（中心差分方式）   
  *out = ( fx( x + h ) - fx( x - h ) ) / ( 2 * h );

  return 0;
}

/**
 * １変数関数の数値微分を計算
 * - 値を返す版
 */
template < typename T >
inline
T MyCalcGrad( T (*fx)( T ), T x, T h = 1E-06 ){
  T out;
  MyCalcGrad( fx, x, &out, h );
  return out;
}

/**
 * 多変数関数の勾配（ナブラ）を返す
 * - vector< T > fx( const vector< T > &x ) な関数
 * - 数値微分（中心差分）
 * @param fx 多変数関数へのポインタ
 * @param x この位置での勾配を計算
 * @param [out] 出力値
 * @param h 数値微分に用いる微小変化値。デフォルトは、1E-6
 */
template < typename T >
inline
int
MyVecGrad( T (*fx)( const std::vector< T > & ),
           const std::vector< T > &x,
           std::vector< T > &out,
           T h = 1E-06
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

/**
 * 勾配ベクトルを返り値として返すパターン
 */
template < typename T >
inline std::vector< T >
MyVecGrad( T (*fx)( const std::vector< T > & ),
           const std::vector< T > &x, T h = 1E-06 ){
  std::vector< T > out;
  MyVecGrad( fx, x, out, h );
  return out;
}

/**
 * 数値微分（微小差分）でヘッセを計算する
 * - d2f/dxdy = ( f(x+h,y+h) - f(x-h,y+h) - f(x+h,y-h) + f(x-h,y-h) ) / (4*h*h)
 * @param h 差分計算時の微小変化量。二乗されることに注意。小さくしすぎると値がおかしくなる。
 */
template < typename T >
inline
int
MyMatHessian( T (*fx)( const std::vector< T > & ),
              const std::vector< T > &x,
              std::vector< std::vector< T > > &out,
              T h = 1E-3 ){
  using namespace std;

  // 入力チェック
  assert( h != 0 );
  
  // 次元
  int N = x.size();

  // 出力バッファ
  if( out.empty() ) out.resize( N, vector< T >( N ) );
  else assert( MyMatSize( out ) == MyPoint2i( N, N ) );
  
  for( int i = 0; i < N; i++ ){
    for( int j = i; j < N; j++ ){
      vector< T > h1( x );
      h1[ i ] += h;
      h1[ j ] += h;
      vector< T > h2( x );
      h2[ i ] -= h;
      h2[ j ] += h;
      vector< T > h3( x );
      h3[ i ] += h;
      h3[ j ] -= h;
      vector< T > h4( x );
      h4[ i ] -= h;
      h4[ j ] -= h;
      out[ i ][ j ] = ( fx( h1 ) - fx( h2 ) - fx( h3 ) + fx( h4 ) ) / ( 4 * h * h );
      out[ j ][ i ] = out[ i ][ j ];
    }//j
  }//i
  
  return 0;
}

/**
 * ３x３の逆行列
 * - 解析解
 * @param [in] src 計算対象の行列。
 * @param [out] dst メモリが確保済みでなければ内部で確保。src と同じサイズの確保済みを渡しても OK 。
 * @return 行列式の値を返す。行列式がゼロの場合はエラー。
 */
inline
double
MyMatInv3x3( const std::vector< std::vector< double > > &src,
             std::vector< std::vector< double > > &dst )
{
  // assert( src.size() == 3 );
  // assert( src[ 0 ].size() == 3 );
  assert( MyMatSize( src ) == MyPoint2i( 3, 3 ) );

  double detA =
      src[0][0] * src[1][1] * src[2][2] +
      src[1][0] * src[2][1] * src[0][2] +
      src[2][0] * src[0][1] * src[1][2] -
      src[0][0] * src[2][1] * src[1][2] -
      src[2][0] * src[1][1] * src[0][2] -
      src[1][0] * src[0][1] * src[2][2];

  assert( detA != 0 );

  if( dst.empty() ) dst.resize( 3, std::vector< double >( 3 ) );
  else{
    assert( dst.size() == 3 );
    assert( dst[ 0 ].size() == 3 );
  }

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

/**
 * ４x４の逆行列
 * - 解析解
 * @param [in] src 計算対象の行列。
 * @param [out] dst メモリが確保済みでなければ内部で確保。src と同じサイズの確保済みを渡しても OK 。
 * @return 行列式の値を返す。行列式がゼロの場合はエラー。
 */
inline
double
MyMatInv4x4( const std::vector< std::vector< double > > &src,
             std::vector< std::vector< double > > &dst )
{
  // assert( src.size() == 4 );
  // assert( src[ 0 ].size() == 4 );
  assert( MyMatSize( src ) == MyPoint2i( 4, 4 ) );
  
  double detA =
      src[0][0]*src[1][1]*src[2][2]*src[3][3] + src[0][0]*src[1][2]*src[2][3]*src[3][1] + src[0][0]*src[1][3]*src[2][1]*src[3][2] +
      src[0][1]*src[1][0]*src[2][3]*src[3][2] + src[0][1]*src[1][2]*src[2][0]*src[3][3] + src[0][1]*src[1][3]*src[2][2]*src[3][0] +
      src[0][2]*src[1][0]*src[2][1]*src[3][3] + src[0][2]*src[1][1]*src[2][3]*src[3][0] + src[0][2]*src[1][3]*src[2][0]*src[3][1] +
      src[0][3]*src[1][0]*src[2][2]*src[3][1] + src[0][3]*src[1][1]*src[2][0]*src[3][2] + src[0][3]*src[1][2]*src[2][1]*src[3][0] -
      src[0][0]*src[1][1]*src[2][3]*src[3][2] - src[0][0]*src[1][2]*src[2][1]*src[3][3] - src[0][0]*src[1][3]*src[2][2]*src[3][1] -
      src[0][1]*src[1][0]*src[2][2]*src[3][3] - src[0][1]*src[1][2]*src[2][3]*src[3][0] - src[0][1]*src[1][3]*src[2][0]*src[3][2] -
      src[0][2]*src[1][0]*src[2][3]*src[3][1] - src[0][2]*src[1][1]*src[2][0]*src[3][3] - src[0][2]*src[1][3]*src[2][1]*src[3][0] -
      src[0][3]*src[1][0]*src[2][1]*src[3][2] - src[0][3]*src[1][1]*src[2][2]*src[3][0] - src[0][3]*src[1][2]*src[2][0]*src[3][1];

  assert( detA != 0 );

  if( dst.empty() ) dst.resize( 4, std::vector< double >( 4 ) );
  else{
    assert( dst.size() == 4 );
    assert( dst[ 0 ].size() == 4 );
  }

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

/**
 * 多項式近似
 * - 二次式 y = a x^2 + b x + c でのフィッティング
 * - 二次元のデータ列、(x_i, y_i) (i=0,1,2,...)、に対するフィッティング
 */
int
MyFitQuad( const std::vector< double > &data_x,
           const std::vector< double > &data_y,
           double *a, double *b, double *c )
{
  using namespace std;
  
  int N = data_x.size();
  assert( N > 0 );
  assert( N == data_y.size() );
  
  double n = 0.0, sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0, sum_x4 = 0.0, 
      sum_y = 0.0, sum_xy = 0.0, sum_x2y = 0.0;
  
  for( int i = 0; i < N; i++ ){
    n += 1;
    sum_x += data_x[ i ];
    sum_x2 += data_x[ i ] * data_x[ i ];
    sum_x3 += data_x[ i ] * data_x[ i ] * data_x[ i ];
    sum_x4 += data_x[ i ] * data_x[ i ] * data_x[ i ] * data_x[ i ];
    sum_y += data_y[ i ];
    sum_xy += data_x[ i ] * data_y[ i ];
    sum_x2y += data_x[ i ] * data_x[ i ] * data_y[ i ];
  }// i
  
  vector< vector< double > > A( 3, vector< double >( 3 ) );
  A[ 0 ][ 0 ] = n;      A[ 0 ][ 1 ] = sum_x;  A[ 0 ][ 2 ] = sum_x2;
  A[ 1 ][ 0 ] = sum_x;  A[ 1 ][ 1 ] = sum_x2; A[ 1 ][ 2 ] = sum_x3;
  A[ 2 ][ 0 ] = sum_x2; A[ 2 ][ 1 ] = sum_x3; A[ 2 ][ 2 ] = sum_x4;

  vector< double > B( 3 );
  B[ 0 ] = sum_y;
  B[ 1 ] = sum_xy;
  B[ 2 ] = sum_x2y;

  vector< vector< double > > InvA;
  MyMatInv3x3( A, InvA );

  *c = InvA[0][0] * B[0] + InvA[0][1] * B[1]  + InvA[0][2] * B[2];
  *b = InvA[1][0] * B[0] + InvA[1][1] * B[1]  + InvA[1][2] * B[2];
  *a = InvA[2][0] * B[0] + InvA[2][1] * B[1]  + InvA[2][2] * B[2];

  return 0;
}

/**
 * 多項式近似
 * - 三次式 y = a x^3 + b x^2  + c x + d でのフィッティング
 * - 二次元のデータ列、(x_i, y_i) (i=0,1,2,...)、に対するフィッティング
 */
int
MyFitCubic( const std::vector< double > &data_x,
            const std::vector< double > &data_y, 
            double *a, double *b, double *c, double *d )
{
  using namespace std;
  
  int N = data_x.size();
  assert( N > 0 );
  assert( N == data_y.size() );

  double sum_m = 0.0, sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0;
  double sum_x4 = 0.0, sum_x5 = 0.0, sum_x6 = 0.0;
  double sum_y = 0.0, sum_yx = 0.0, sum_yx2 = 0.0, sum_yx3 = 0.0;
  
  for( int i = 0; i < N; i++ ){
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

  vector< vector< double > > A( 4, vector< double >( 4 ) );
  A[0][0] = sum_m;  A[0][1] = sum_x;  A[0][2] = sum_x2; A[0][3] = sum_x3;
  A[1][0] = sum_x;  A[1][1] = sum_x2; A[1][2] = sum_x3; A[1][3] = sum_x4;
  A[2][0] = sum_x2; A[2][1] = sum_x3; A[2][2] = sum_x4; A[2][3] = sum_x5;
  A[3][0] = sum_x3; A[3][1] = sum_x4; A[3][2] = sum_x5; A[3][3] = sum_x6;
               
  vector< double > B( 4 );
  B[0] = sum_y;
  B[1] = sum_yx;
  B[2] = sum_yx2;
  B[3] = sum_yx3;
  
  vector< vector< double > > InvA;
  MyMatInv4x4( A, InvA );

  *d = InvA[0][0] * B[0] + InvA[0][1] * B[1]  + InvA[0][2] * B[2] + InvA[0][3] * B[3];
  *c = InvA[1][0] * B[0] + InvA[1][1] * B[1]  + InvA[1][2] * B[2] + InvA[1][3] * B[3];
  *b = InvA[2][0] * B[0] + InvA[2][1] * B[1]  + InvA[2][2] * B[2] + InvA[2][3] * B[3];
  *a = InvA[3][0] * B[0] + InvA[3][1] * B[1]  + InvA[3][2] * B[2] + InvA[3][3] * B[3];

  return 0;
}

// /**
//  * 乱数生成
//  * - 平均 mu、分散 sg2 の正規分布で分布する乱数を発生する。 
//  */
// double MyRandGauss( double mu, double sg2 ){
//   using namespace std;
//   // Box-Muller's Method
//   double X = rand() / (double)RAND_MAX;
//   double Y = rand() / (double)RAND_MAX;
//   double Z1 = sqrt( sg2 ) * sqrt( -2 * log( X ) ) * cos( 2 * M_PI * Y ) + mu;
//   //double Z2 = sqrt( sg2 ) * sqrt( -2 * log( X ) ) * sin( 2 * M_PI * Y ) + mu;
//   return Z1;
// }

/**
 * 乱数生成
 * - 平均 mu、標準偏差 sg の正規分布で分布する乱数を発生する。 
 */
double MyRandGauss( double mu, double sg ){
  using namespace std;
  // Box-Muller's Method
  double X = rand() / (double)RAND_MAX;
  double Y = rand() / (double)RAND_MAX;
  double Z1 = sg * sqrt( -2 * log( X ) ) * cos( 2 * M_PI * Y ) + mu;
  return Z1;
}

/**
 * 乱数生成
 * - ポアソン分布に従う乱数を発生する。
 * - ただし、lambda が大きいと exp( -lambda ) が 0 になり、返ってこないので注意。
 */
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

/** 
 * 行列の LU 分解
 * - 枢軸選択（ピボッティング）は実装していない。なので計算に失敗する場合もあり。
 * @param[in,out] A 対象となる行列。LU 分解した結果で上書きされる。
 * @return 0:成功、0以外:失敗
 */
int
MyLUDecomp( std::vector< std::vector< double > > &A ){
  int N = A.size();
  assert( N > 0 );
  assert( A[ 0 ].size() == N );
  using namespace std;
  for( int i = 0; i < N - 1; i++ ){
    if( A[ i ][ i ] == 0 ) return -1; 
    for( int j = i + 1; j < N; j++ ){
      A[ j ][ i ] = A[ j ][ i ] / A[ i ][ i ];
      for( int k = i + 1; k < N; k++ ){
        A[ j ][ k ] = A[ j ][ k ] - A[ j ][ i ] * A[ i ][ k ];
      }
    }
  }
  return 0;
}

/**
 * LU 分解による連立一次方程式の計算
 * - 計算量は、係数行列のサイズ n に対して、O(n^3)
 * @param[in,out] A 正方行列。関数の呼び出し後は、LU 分解された結果が入る。
 * @param[in,out] x 解。初期値は不要。メモリは確保済みでも確保済みでなくても可。
 * @param[in,out] b 定数ベクトル。内部で変数として利用されるため、呼び出し後、中身は変更されている。
 */
int
MyLUSolve( std::vector< std::vector< double > > &A,
           std::vector< double > &x,
           std::vector< double > &b,
           bool modify = true
           ){
  using namespace std;

  // 次元
  int N = A.size();

  // 入力チェック
  assert( N > 0 );
  assert( A[ 0 ].size() == N );
  assert( b.size() == N );
  if( x.empty() ) x.resize( N );
  else assert( x.size() == N );

  // LU 分解をする
  assert( ! MyLUDecomp( A ) );

  // 前進代入
  for( int i = 0; i < N; i++ ){
    b[ i ] = b[ i ];
    for( int j = 0; j < i; j++ ){
      b[ i ] -= A[ i ][ j ] * b[ j ];
    }
  }

  // 後退代入
  for( int i = N - 1; i >= 0; i-- ){
    x[ i ] = b[ i ];
    for( int j = i + 1; j < N; j++ ){
      x[ i ] -= A[ i ][ j ] * x[ j ];
    }
    assert( A[ i ][ i ] != 0 );
    x[ i ] /= A[ i ][ i ];
  }

  return 0;
}

/**
 * LU 分解による連立一次方程式の計算
 * - 事前に LU 分解済みの行列を渡す。
 * - 定数ベクトル b を次々変えて方程式を解きたい用。
 * - 計算量は、係数行列のサイズ n に対して、O(n^2)
 * @param[in] L 下半分行列。
 * @param[in] U 上半分行列。
 * @param[in,out] x 解。初期値は不要。メモリは確保済みでも確保済みでなくても可。
 * @param[in,out] b 定数ベクトル。内部で変数として利用されるため、呼び出し後、中身は変更されている。
 */
int
MyLUSolve( const std::vector< std::vector< double > > &L,
           const std::vector< std::vector< double > > &U,
           std::vector< double > &x,
           std::vector< double > &b
           ){
  using namespace std;

  // 入力チェック
  assert( MyMatIsSquare( L ) && MyMatIsSquare( U ) );
  assert( MyMatSize( L ) == MyMatSize( U ) );
  int N = L.size();
  assert( b.size() == N );
  if( x.empty() ) x.resize( N );
  else assert( x.size() == N );

  // 前進代入
  for( int i = 0; i < N; i++ ){
    b[ i ] = b[ i ];
    for( int j = 0; j < i; j++ ){
      b[ i ] -= L[ i ][ j ] * b[ j ];
    }
  }

  // 後退代入
  for( int i = N - 1; i >= 0; i-- ){
    x[ i ] = b[ i ];
    for( int j = i + 1; j < N; j++ ){
      x[ i ] -= U[ i ][ j ] * x[ j ];
    }
    assert( U[ i ][ i ] != 0 );
    x[ i ] /= U[ i ][ i ];
  }
  
  return 0;
}

/**
 * LU 分解された結果が一緒になった行列 A から、下半分行列 L、上半分行列 U を抽出する。
 * @param[in] A MyLUDecomp() で LU 分解済みの行列を渡すこと
 * @param[out] L 下半分行列。メモリは確保済み or 未確保でも良い。
 * @param[out] U 上半分行列。メモリは確保済み or 未確保でも良い。
 */
int
MyLUSet( const std::vector< std::vector< double > > &A,
         std::vector< std::vector< double > > &L,
         std::vector< std::vector< double > > &U ){
  using namespace std;

  assert( MyMatIsSquare( A ) );
  int N = A.size();
  if( L.empty() ) L.resize( N, vector< double >( N ) );
  else assert( MyMatAreTheSameSize( L, A ) );
  if( U.empty() ) U.resize( N, vector< double >( N ) );
  else assert( MyMatAreTheSameSize( U, A ) );

  for( int i = 0; i < N; i++ ){
    for( int j = i; j < N; j++ ){
      U[ i ][ j ] = A[ i ][ j ];
    }
    for( int j = 0; j < i; j++ ){
      L[ i ][ j ] = A[ i ][ j ];
    }
    L[ i ][ i ] = 1;
  }
  return 0;
}

/**
 * 行列が対角優位かどうかのチェック
 * - 対角優位：係数行列の各行について、対角成分の絶対値が非対角成分の絶対値の総和よりも大きいこと
 * @return 対角優位:true, そうでない場合:false
 */
bool
MyMatIsDiagDominant( const std::vector< std::vector< double > > &A ){
  int N = A.size();
  for( int i = 0; i < N; i++ ){
    double sum = 0;
    for( int j = 0; j < N; j++ ){
      if( i != j ) sum += MyAbs( A[ i ][ j ] );
    }
    if( sum > MyAbs( A[ i ][ i ] ) ) return false;
  }
  return true;
}

/**
 * 連立一次方程式を解く
 * - Ax = b
 * - ヤコビ反復法
 * - 係数行列が以下のいずれかの条件を満たさないと収束しない
 *   - 対角優位（係数行列の各行について対角成分の絶対値が非対角成分の絶対値の総和よりも大きい）
 *   - 対称行列＆正値（固有値が全て正）
 * - 計算量は、係数行列のサイズ n に対して、O(n^2)
 * @param A 係数行列。n x n の正方行列。
 * @param[in,out] x 解ベクトル。初期値を入れておく or 初期値なしの空ベクトル。空の場合は初期値 0 。
 * @param thres 収束条件：解の変化量（ノルム）がこの値を下回ったら計算終了
 * @param max_itr_num 収束条件：繰り返し計算の最大繰り返し回数。
 * @param dout デバッグ情報の表示
 */
int
MyJacobiSolve( const std::vector< std::vector< double > > &A,
               std::vector< double > &x,
               const std::vector< double > &b,
               double thres = 1E-06,
               int max_itr_num = 100,
               std::ostream *dout = 0 ){
  using namespace std;

  // 次元
  int N = A.size();

  // 入力チェック
  assert( N > 0 );
  assert( A[ 0 ].size() == N );
  assert( b.size() == N );
  if( x.empty() ) x.resize( N, 0 );
  else assert( x.size() == N );

  // 収束するかどうかのチェック
  // if( ! MyMatIsDiagDominant( A ) ) return -1;
  
  // 一時変数
  vector< double > x_next( N );
  
  if( dout ){
    *dout << "--- MyJacobiSolve() ---" << endl;
    *dout << "[0]\t" << x << endl;
  }
    
  // 反復処理
  for( int k = 0; k < max_itr_num; k++ ){
    for( int i = 0; i < N; i++ ){
      x_next[ i ] = b[ i ];
      for( int j = 0; j < N; j++ ){
        if( i != j ){
          x_next[ i ] -= A[ i ][ j ] * x[ j ];
        }
      }//j
      assert( A[ i ][ i ] != 0 );
      x_next[ i ] /= A[ i ][ i ];
    }//i

    // 収束判定
    double dx = MyVecNorm( x_next - x );
    if( dout ) *dout << "[" << k + 1 << "]\t x: " << x_next << " dx: " << dx << endl;
    if( dx < thres ) break;

    // 更新
    x = x_next;

  }//k
  
  return 0;
}

/**
 * 連立一次方程式を解く
 * - Ax = b
 * - ガウスザイデルの反復法
 * - 係数行列が以下のいずれかの条件を満たさないと収束しない
 *   - 対角優位（係数行列の各行について対角成分の絶対値が非対角成分の絶対値の総和よりも大きい）
 *   - 対称行列＆正値（固有値が全て正）
 * - 計算量は、係数行列のサイズ n に対して、O(n^2)
 * - ヤコビの反復法に比べて、内部で使うメモリの量が少ない＆収束が速い。
 * - なので、基本的にはヤコビ反復法よりはこっちを使った方がよい。
 * @param A 係数行列。n x n の正方行列。
 * @param[in,out] x 解ベクトル。初期値を入れておく or 初期値なしの空ベクトル。空の場合は初期値 0 。
 * @param thres 収束条件：解の変化量（各要素の変化量絶対値の総和）がこの値を下回ったら計算終了
 * @param max_itr_num 収束条件：繰り返し計算の最大繰り返し回数。
 * @param dout デバッグ情報の表示
 */
int
MyGaussSeidelSolve( const std::vector< std::vector< double > > &A,
                    std::vector< double > &x,
                    const std::vector< double > &b,
                    double thres = 1E-06,
                    int max_itr_num = 100,
                    std::ostream *dout = 0 ){
  using namespace std;

  // 次元
  int N = A.size();

  // 入力チェック
  assert( N > 0 );
  assert( A[ 0 ].size() == N );
  assert( b.size() == N );
  if( x.empty() ) x.resize( N, 0 );
  else assert( x.size() == N );

  // 収束するかどうかのチェック
  // if( ! MyMatIsDiagDominant( A ) ) return -1;
  
  if( dout ){
    *dout << "--- MyGaussSeidelSolve() ---" << endl;
    *dout << "[0]\t" << x << endl;
  }
    
  // 反復処理
  for( int k = 0; k < max_itr_num; k++ ){
    double dx = 0; // 解の変化量計算（収束判定のため）
    for( int i = 0; i < N; i++ ){
      double x_old = x[ i ]; // 更新前の値を覚えておく（解の変化量計算のため）
      x[ i ] = b[ i ];
      for( int j = 0; j < N; j++ ){
        if( j != i ) x[ i ] -= A[ i ][ j ] * x[ j ];
      }//j
      assert( A[ i ][ i ] != 0 );
      x[ i ] /= A[ i ][ i ];
      dx += MyAbs( x_old - x[ i ] );
    }//i
    // 収束判定
    if( dout ) *dout << "[" << k + 1 << "]\t x: " << x << " dx: " << dx << endl;
    if( dx < thres ) break;

  }//k
  
  return 0;
}

/**
 * QR 分解を行う。
 * - 内部での高速化は未実装。
 */
int MyQRDecomp( const std::vector< std::vector< double > > &A,
                std::vector< std::vector< double > > &Q,
                std::vector< std::vector< double > > &R ){
  using namespace std;
  using namespace my;

  // A は、正方行列＆逆行列が存在する必要（正則）
  assert( MyMatIsSquare( A ) );
  // assert( A が正則 );

  int n = A.size();
  Q = vector< vector< double > >( n );
  R = MyMatZero< double >( n );

  vector< vector< double > > a = MyGetColVectors( A );
  for( int i = 0; i < n; i++ ){
    vector< double > u = a[ i ];
    for( int j = 0; j < i; j++ ){
      R[ j ][ i ] = MyVecDot( a[ i ], Q[ j ] );
      u = u - R[ j ][ i ] * Q[ j ];
    }//j
    R[ i ][ i ] = MyVecNorm( u );
    Q[ i ] = u / R[ i ][ i ];
  }//i

  Q = MyMatTrans( Q );

  return 0;
}

/**
 * 正方＆正則行列の固有値を返す。
 * - QR分解による方法。
 * - 高速化はしていない。
 */
int MyGetEigenVals_QR( const std::vector< std::vector< double > > &A,
                       std::vector< double > &out_vals,
                       double itr_end_thres = 1E-6,
                       int max_itr_num = 100 ){
  using namespace std;
  using namespace my;

  vector< vector< double > > A_k = A, Q, R;
  vector< double > last_vals = MyGetDiagVector( A_k );
  for( int k = 0; k < max_itr_num; k++ ){
    assert( ! MyQRDecomp( A_k, Q, R ) );
    A_k = R * Q;
    out_vals = MyGetDiagVector( A_k );
    if( k > 0 && MyVecNorm( out_vals - last_vals ) < itr_end_thres ) break;
    last_vals = out_vals;
  }//k
  
  return 0;
}

//#########################################################################################
// 最小化
//#########################################################################################

/**
 * 各種最小化処理を行うクラス
 */
class MyMinSearch {
  double _error_thres; //!< 収束判定に使う閾値
  int _max_itr_count; //!< 繰り返し計算の打ち切り回数。
  std::ostream *_dout; //!< 画面表示などデバッグの際の表示先
  int _itr_count; //!< 繰り返し回数のカウント
  double _cur_error; //!< 現在の誤差
  bool _is_converged; //!< 収束したかどうかのフラグ

 public:
  /**
   * 直線検索で用いる１変数関数の最小化アルゴリズムの種類
   */
  typedef enum {
    GoldenSection = 0,
    GradientBased,
    DownhillSimplex
  } LineSearchMethodType;

  /**
   * デバッグ表示のときに使うオプション
   */
  typedef enum {
    OutAll = 0,
    OutCounterOnly
  } DebugOutType;

 private:

  LineSearchMethodType _line_search_method_type; //<! 直線検索で使う１変数関数の最小化アルゴリズム
  static const std::vector< double > *_LineSearch_x; //<! 直線検索内部で使う変数
  static const std::vector< double > *_LineSearch_dx; //<! 直線検索内部で使う変数
  static double (*_LineSearch_fx)( const std::vector< double > & ); //<! 直線検索内部で使う変数
  /** 直線検索内部で使う関数 */
  static double lineSearch_Ft( double t ){
    return _LineSearch_fx( (*_LineSearch_x) + t * (*_LineSearch_dx) );
  }
  static double lineSearch_Ft2( const std::vector< double > &t ){
    assert( t.size() > 0 );
    return _LineSearch_fx( (*_LineSearch_x) + t[0] * (*_LineSearch_dx) );
  }
  DebugOutType _dout_type; //!< デバッグ出力の際の表示オプション

  /**
   * 繰り返し計算前の初期化
   */
  void init() {
    _itr_count = 0;
    _cur_error = 0;
    _is_converged = false;
  }
  
 public:
  MyMinSearch() : _error_thres( 1E-6 ),
                  _max_itr_count( 10000 ),
                  _line_search_method_type( GoldenSection ),
                  _dout( 0 ), _dout_type( OutAll ){
    init();
  };

  void setErrorThres( double error_thres ) { _error_thres = error_thres; }
  void setMaxItrCount( int max_itr_count ) { _max_itr_count = max_itr_count; }
  void setLineSearchMethod( LineSearchMethodType type ) { _line_search_method_type = type; }
  void setDebugOut( std::ostream *stream, DebugOutType type = OutAll ) { _dout = stream; _dout_type = type; };
  
  double getErrorThres() const { return _error_thres; }
  int getMaxItrCount() const { return _max_itr_count; }
  int getItrCount() const { return _itr_count; }
  double getCurError() const { return _cur_error; }

  /**
   * 収束したかどうか
   */
  bool isConverged() const { return _is_converged; }
  
  /**
   * １変数関数の最小化
   * - 黄金分割
   * - a < x < b の範囲における f(x) の値を最小にする x の値を返す。
   * @param fx 評価関数
   * @param[in,out] a x の検索範囲（の初期値）(a,b)
   * @param[in,out] b x の検索範囲（の初期値）(a,b)
   * @param[out] out 出力値。評価関数の値を最小にする x の値。（a,b の最終結果の中点）
   */
  int runGoldenSection( double (*fx)( double ),
                        double *a,
                        double *b,
                        double *out ){
    using namespace std;

    // 初期化
    init();
    
    // 内分比（黄金比）
    const double tau = ( sqrt( 5 ) - 1 ) / 2.0;

    if( _dout && _dout_type == OutAll ){
      *_dout << "--- GoldenSection ---" << endl;
      *_dout << "a:\t" << *a << endl;
      *_dout << "b:\t" << *b << endl;
    }

    // 探索処理
    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){
      
      // [a, b] をそれぞれ ( tau / ( tau + 1 ) ), ( 1 / ( tau + 1 ) ) に内分する点を求める
      double t1 = (*a) + ( (*b) - (*a) ) * tau / ( 1 + tau );
      double t2 = (*a) + ( (*b) - (*a) ) * 1 / ( 1 + tau );

      // それぞれの位置での関数値を計算
      double f1 = fx( t1 );
      double f2 = fx( t2 );
    
      if( _dout && _dout_type == OutAll ){
        *_dout << "[" << _itr_count << "]\t";
        *_dout << "f(" << t1 << ") = " << f1 << ",\t";
        *_dout << "f(" << t2 << ") = " << f2 << endl;
      }

      // 関数値の大小に応じて、探索範囲を狭めていく
      if( f1 > f2 ) *a = t1;
      else *b = t2;

      // 収束条件評価値
      _cur_error = MyAbs( *b - *a );
      
      // 閾値以下になったら終了
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }
    }

    // 探索範囲の中点を出力する
    *out = ( *b + *a ) / 2.0;

    return 0;
  }

  /**
   * １変数関数の最小化
   * - 勾配利用。勾配は数値微分。
   * @param fx 評価関数
   * @param[in,out] x 初期値＆出力値
   */
  int runGradientBased( double (*fx)( double ),
                        double *x ){
    using namespace std;

    // 固定パラメータ
    const double INIT_STEP_PCT = 1; // 最初の移動量。初期値に対する % で指定。
    const double INIT_STEP_WHEN_ZERO = 0.00025; // 初期値がゼロだった場合の最初の移動量。
    const double MAX_ITR_NUM2 = 100; // 内部の while 文での最大繰り返し数。

    // 初期化
    init();
    
    // 初期値のセット
    double h = (*x) * INIT_STEP_PCT / 100.0;
    if( h == 0 ) h = INIT_STEP_WHEN_ZERO;

    // 諸変数
    double gx_x, X, Xd, fx_X, fx_Xd;

    // 初期位置での勾配
    gx_x = MyCalcGrad( fx, *x );

    // 収束条件評価値
    _cur_error = MyAbs( gx_x );
    
    if( _dout && _dout_type == OutAll ){
      *_dout << "--- GradientBased ---" << endl;
      *_dout << "[" << _itr_count << "]";
      *_dout << "\t f'(" << *x << ") = " << gx_x << endl;
    }

    // 探索処理（勾配が閾値以下（≒０）になるまで）
    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){

      // 勾配の正負から検索方向を決める。
      h = - MySgn( gx_x ) * MyAbs( h );

      // 現在位置から h 離れた位置の関数値を求める
      X = *x;
      Xd = *x + h;
      fx_X = fx( X );
      fx_Xd = fx( Xd );
    
      if( _dout && _dout_type == OutAll ){
        *_dout << "\t f(" << *x << ") = " << fx_X << ",\t";
        *_dout << "f(" << *x << " + " << h << ") = " << fx_Xd << endl;
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
        
          if( _dout && _dout_type == OutAll ){
            *_dout << "\t f(" << X << ") = " << fx_X << ",\t";
            *_dout << "f(" << X << " + " << h << ") = " << fx_Xd << endl;
          }

          // 安全のため、閾値以上繰り返したらループを抜ける。
          if( ++cnt > MAX_ITR_NUM2 ) break;
        }

        // 位置の更新
        *x = X;
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
        
          if( _dout && _dout_type == OutAll ){
            *_dout << "\t f(" << X << ") = " << fx_X << ",\t";
            *_dout << "f(" << X << " + " << h << ") = " << fx_Xd << endl;
          }
        
          // 安全のため、閾値以上繰り返したらループを抜ける。
          if( ++cnt > MAX_ITR_NUM2 ) break;
        }

        // 位置の更新
        *x = Xd;
        h *= 2;
      
      }// else

      // 勾配の値を更新
      gx_x = MyCalcGrad( fx, *x );

      // 収束条件評価値
      _cur_error = MyAbs( gx_x );
    
      if( _dout && _dout_type == OutAll ){
        *_dout << "[" << _itr_count << "]";
        *_dout << "\t f'(" << *x << ") = " << gx_x << endl;
      }

      // 収束判定
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }
    
    }// for

    return 0;
  }
  
  /**
   * 多変数関数の最小化
   * - Downhill Simplex 法 (Nelder-Mead)
   * - 関数 double fx( const vector< double > &x ) の値を最小にする入力 x を求める
   * - １変数関数も可。
   * @param fx 評価関数
   * @param[in,out] x 初期値＆出力値
   */
  int runDownhillSimplex( double (*fx)( const std::vector< double > & ),
                          std::vector< double > &x ){
    using namespace std;
  
    // 固定パラメータ
    const double ALPHA = 1; // 反射の大きさ
    const double GAMMA = 2; // 拡張の大きさ
    const double RHO = 0.5; // 収縮の大きさ
    const double SIGMA = 0.5; // シンプレックス縮小の大きさ
    const double INIT_SMP_SIZE_PCT = 5; // 初期値の何％の範囲でシンプレックスを生成するか
    const double INIT_SMP_SIZE_WHEN_ZERO = 0.00025; // 上記で初期値がゼロだった場合に代わりに使う値
  
    // 初期化
    init();
    
    if( _dout && _dout_type == OutAll ){
      *_dout << "--- DownhillSimplex ---" << endl;
    }

    // シンプレックス
    typedef multimap< double, vector< double > > map_type;
    map_type smp;

    // 次元
    int n = x.size();
  
    // 諸変数
    vector< double > x_in;
    double x_out;

    // --- 初期値の周りに乱数でシンプレックスを生成 ---

    for( int i = 0; i < n; i++ ){
      x_in.clear();
      for( int j = 0; j < n; j++ ){
        double rand_val = ( rand() / (double)RAND_MAX - 0.5 ) * 2;
        double size = MyAbs( x[ j ] * INIT_SMP_SIZE_PCT / 100.0 );
        double val = x[ j ] + rand_val * size;
        //if( x[ j ] == 0 ) val = INIT_SMP_SIZE_WHEN_ZERO;
        if( x[ j ] == 0 ){
          size = MyAbs( INIT_SMP_SIZE_WHEN_ZERO * INIT_SMP_SIZE_PCT / 100.0 );
          val = INIT_SMP_SIZE_WHEN_ZERO + rand_val * size;
        }
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
      double val = ( n + 1 ) * x[ i ] - sum;
      x_in.push_back( val );
    }
    x_out = fx( x_in );
    smp.insert( make_pair< double, vector< double > >( x_out, x_in ) );

    // --- 探索処理 ---

    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){

      // 現時点での解
      vector< double > x_min = (smp.begin())->second;

      // その位置での関数値
      double f_min = (smp.begin())->first;

      if( _dout && _dout_type == OutAll ){
        *_dout << "[" << _itr_count << "]\t f_min: " << f_min << endl;
        *_dout << "\t x_min: " << x_min << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ){
        *_dout << "[" << _itr_count << "]" << endl;
      }

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
    
      if( _dout && _dout_type == OutAll ){
        *_dout << "\t smp_size: " << smp_size << endl;
      }

      // 誤差＝シンプレックスサイズ
      _cur_error = smp_size;
      
      // シンプレックスサイズが閾値を下回れば、計算終了
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }

      if( _dout && _dout_type == OutAll ){
        *_dout << "\t simplex:" << endl;
        for( map_type::const_iterator it = smp.begin(); it != smp.end(); it++ ){
          *_dout << "\t\t" << it->first << "\t (" << it->second << ")" << endl;
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
    
      if( _dout && _dout_type == OutAll ){
        *_dout << "\t Step.1:" << endl;
        *_dout << "\t\t x_c: " << x_c << endl;
        *_dout << "\t\t x_max: " << x_max << endl;
        *_dout << "\t\t x_ref: " << x_ref << endl;
        *_dout << "\t\t f_ref: " << f_ref << "\t f_max: " << f_max << endl;
        if( f_ref <= f_max ) *_dout << "\t\t f_ref(" << f_ref << ") <= f_max(" << f_max << ")" << endl;
        else *_dout << "\t\t f_ref(" << f_ref << ") > f_max(" << f_max << ")" << endl;
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

        if( _dout && _dout_type == OutAll ){
          *_dout << "\t Step.2:" << endl;
          *_dout << "\t\t x_exp: " << x_exp << endl;
          *_dout << "\t\t f_ref: " << f_ref << "\t f_exp: " << f_exp << endl;
          if( f_ref <= f_exp ){
            *_dout << "\t\t f_ref(" << f_ref << ") <= f_exp(" << f_exp << ")" << endl;
            *_dout << "\t\t x_max(" << x_max << ") ---> x_ref(" << x_ref << ")" << endl;
          }
          else{
            *_dout << "\t\t f_ref(" << f_ref << ") > f_exp(" << f_exp << ")" << endl;
            *_dout << "\t\t x_max(" << x_max << ") ---> x_exp(" << x_exp << ")" << endl;
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

        if( _dout && _dout_type == OutAll ){
          *_dout << "\t Step.3:" << endl;
          *_dout << "\t\t x_con: " << x_con << endl;
          *_dout << "\t\t f_con: " << f_con << endl;
          *_dout << "\t\t f_max: " << f_max << endl;
          if( f_con <= f_max ){
            *_dout << "\t\t f_con(" << f_con << ") <= f_max(" << f_max << ")" << endl;
          }
          else{
            *_dout << "\t\t f_con(" << f_con << ") > f_max(" << f_max << ")" << endl;
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
        
          if( _dout && _dout_type == OutAll ){
            *_dout << "\t\t x_max(" << x_max << ") ---> x_con(" << x_con << ")" << endl;
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

          if( _dout && _dout_type == OutAll ){
            *_dout << "\t Step.4" << endl;
            map_type::const_iterator it1 = smp_new.begin();
            map_type::const_iterator it2 = smp.begin();
            for( int i = 0; i < smp.size(); i++ ){
              *_dout << "\t\t" << it1->second << " ---> " << it2->second << endl;
              it1++;
              it2++;
            }
          }

          // 次のステップへ
        
        }// else
      }// else

    }// for
  
    // 出力値をセット
    x = (smp.begin())->second;

    return 0;
  }

  /**
   * 直線検索
   * - fx( x + t * dx ) を最小にする t の値を返す
   * - x, dx は、ベクトル
   * @param fx 評価関数
   * @param x 初期位置
   * @param dx 検索方向
   * @param[out] out 出力値
   */
  int runLineSearch( double (*fx)( const std::vector< double > &),
                     const std::vector< double > &x,
                     const std::vector< double > &dx,
                     double *out ){
    using namespace std;

    if( _dout && _dout_type == OutAll ){
      *_dout << "--- LineSearch ---" << endl;
    }

    // F(t) = fx( x + t * dx ) のセットアップ
    _LineSearch_x = &x;
    _LineSearch_dx = &dx;
    _LineSearch_fx = fx;
    double (*ft)( double ) = lineSearch_Ft;
    double (*ft2)( const std::vector< double > & ) = lineSearch_Ft2;
  
    switch( _line_search_method_type ){
      case GradientBased:
        {
          *out = 0; // 初期値
          assert( ! runGradientBased( ft, out ) );
        }
        break;
      case GoldenSection:
        {
          // --- 検索範囲を決める ---
          const double STEP = 0.00025; // いくつか適当かわからない。値が大きいほど局所解を巻き込みやすい。
          const int MAX_ITR = 100;
          double a = -STEP;
          double b = STEP;
          double fa = ft( a );
          double ff = ft( 0 );
          double fb = ft( b );
          if( _dout && _dout_type == OutAll ){
            *_dout << "[init]\t";
            *_dout << "f(" << a << ") = " << fa << ",\t";
            *_dout << "f(0) = " << ff << ",\t";
            *_dout << "f(" << b << ") = " << fb << endl;
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
          if( _dout && _dout_type == OutAll ){
            *_dout << "[start]\t";
            *_dout << "f(" << a << ") = " << fa << ",\t";
            *_dout << "f(0) = " << ff << ",\t";
            *_dout << "f(" << b << ") = " << fb << endl;
          }
          // --- 探索実行 ---
          assert( ! runGoldenSection( ft, &a, &b, out ) );
        }
        break;
      case DownhillSimplex:
        {
          // １次元の Downhill Simplex
          vector< double > t( 1, 0 );
          assert( ! runDownhillSimplex( ft2, t ) );
          *out = t[ 0 ];
        }
        break;
    }

    return 0;
  }

  /**
   * 最急降下法
   * - 関数 double f( const vector< double > &x ) の値を最小にする入力 x を求める
   * @param fx 評価関数
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runSteepestDescent( double (*fx)( const std::vector< double > &),
                          std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();

    // 収束したかどうか（繰り返し計算内部で別メソッドを呼び出す関係で自分で保持）
    bool is_converged = false;

    // 繰り返し計算回数（これも別途保持）
    int itr_count = 0;

    // 現時点での評価値
    //double fx_val = fx( x );

    // 探索処理
    for( itr_count = 0; itr_count < _max_itr_count; itr_count++ ){

      // 現在位置での勾配
      vector< double > x_grad;
      assert( ! MyVecGrad( fx, x, x_grad ) );

      // 勾配方向に直線検索
      double t;
      assert( ! runLineSearch( fx, x, x_grad, &t ) );

      // 位置の更新
      vector< double > dx = t * x_grad;
      x = x + dx;

      // 収束判定評価値
      // １）移動量
      _cur_error = MyVecNorm( dx );
      // ２）評価値の変化量
      // double fx_val_new = fx( x );
      // _cur_error = MyAbs( fx_val_new - fx_val );
      // fx_val = fx_val_new;

      if( _dout && _dout_type == OutAll ){
        if( itr_count == 1 ) *_dout << "--- SteepestDescent() ---" << endl;
        *_dout << "[" << itr_count << "] ";
        *_dout << "x: " << (x - dx) << " ";
        *_dout << "x_grad: " << x_grad << " ";
        *_dout << "t: " << t << " ";
        *_dout << "dx: " << dx << " ";
        *_dout << "norm(dx): " << _cur_error << " ";
        *_dout << "x updated: " << x << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << itr_count << "]"<< endl;
    
      // 終了判定
      if( _cur_error < _error_thres ){
        is_converged = true;
        break;
      }
    
    } // for 

    // 繰り返しカウンターのセット
    _itr_count = itr_count;

    // フラグのセット
    _is_converged = is_converged;
    
    return 0;
  }

  /**
   * ニュートンラプソン法
   * - 関数 double fx( const vector< double > &x ) の値を最小にする入力 x を求める
   * - ナブラおよびヘッセを計算する関数を与える必要
   *   - nx( const vector< double > &x_in, vector< double > &nf_out )
   *   - Hx( const vector< double > &x_in, vector< vector< double > > &H_out )
   * @param fx 評価関数
   * @param nx ナブラ（勾配）を計算する関数
   * @param Hx ヘッセ行列を計算する関数
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runNewtonRaphson( double (*fx)( const std::vector< double > &),
                        void (*nx)( const std::vector< double > &,
                                    std::vector< double > & ),
                        void (*Hx)( const std::vector< double > &,
                                    std::vector< std::vector< double > > & ),
                        std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();

    // 変数の次元数
    int n = x.size();

    // 勾配ベクトル、変数の移動量
    vector< double > n_x( n ), dx( n );

    // ヘッセ行列
    vector< vector< double > > H_x( n, vector< double >( n ) );
  
    // 現時点での評価値
    //double fx_val = fx( x );

    // 反復処理
    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){

      // 現在位置での勾配
      nx( x, n_x );

      // 現在位置でのヘッセ
      Hx( x, H_x );

      // 連立一次方程式 H Δx = -∇f を解く 
      n_x = -1.0 * n_x;
      assert( ! MyLUSolve( H_x, dx, n_x ) );

      // x の値を更新
      x = x + dx;

      // 収束判定評価値
      // １）移動量
      _cur_error = MyVecNorm( dx );
      // ２）評価値の変化量
      // double fx_val_new = fx( x );
      // _cur_error = MyAbs( fx_val_new - fx_val );
      // fx_val = fx_val_new;
      
      if( _dout && _dout_type == OutAll ){
        if( _itr_count == 0 ) *_dout << "--- NewtonRaphson ---" << endl;
        *_dout << "[" << _itr_count << "]\t x: " << x << "\t |dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << _itr_count << "]"<< endl;
    
      // 収束判定
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }

    }
  
    return 0;
  }

  /**
   * ニュートンラプソン法
   * - ナブラとヘッセを数値微分で計算するバージョン
   * @param fx 評価関数
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runNewtonRaphson( double (*fx)( const std::vector< double > &),
                        std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();

    // 変数の次元数
    int n = x.size();

    // 勾配ベクトル、変数の移動量
    vector< double > n_x( n ), dx( n );

    // ヘッセ行列
    vector< vector< double > > H_x( n, vector< double >( n ) );
  
    // 現時点での評価値
    //double fx_val = fx( x );

    // 反復処理
    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){

      // 現在位置での勾配
      MyVecGrad( fx, x, n_x );

      // 現在位置でのヘッセ
      MyMatHessian( fx, x, H_x );

      // 連立一次方程式 H Δx = -∇f を解く 
      n_x = -1.0 * n_x;
      assert( ! MyLUSolve( H_x, dx, n_x ) );

      // x の値を更新
      x = x + dx;

      // 収束判定評価値
      // １）移動量
      _cur_error = MyVecNorm( dx );
      // ２）評価値の変化量
      // double fx_val_new = fx( x );
      // _cur_error = MyAbs( fx_val_new - fx_val );
      // fx_val = fx_val_new;
      
      if( _dout && _dout_type == OutAll ){
        if( _itr_count == 0 ) *_dout << "--- NewtonRaphson ---" << endl;
        *_dout << "[" << _itr_count << "]\t x: " << x << "\t |dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << _itr_count << "]"<< endl;
    
      // 収束判定
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }

    }
    
    return 0;
  }

  /**
   * 共役勾配法
   * - 関数 double fx( const vector< double > &x ) の値を最小にする入力 x を求める
   * - ナブラおよびヘッセを計算する関数を与える必要
   *   - nx( const vector< double > &x_in, vector< double > &nf_out )
   *   - Hx( const vector< double > &x_in, vector< vector< double > > &H_out )
   * - ニュートン法と比べて、内部で連立一次方程式を解かなくていいというのが利点
   *   - なので収束はニュートン法よりは遅くなる。
   *   - ひょっとしたらニュートン法では解けなかったものが解けるというパターンもあるかもしれない。
   *   - とはいえ、与える引数が同じならニュートン法の方がよさそう。
   * @param fx 評価関数
   * @param nx ナブラ（勾配）を計算する関数
   * @param Hx ヘッセ行列を計算する関数
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runConjugateGradient( double (*fx)( const std::vector< double > &),
                            void (*nx)( const std::vector< double > &,
                                        std::vector< double > & ),
                            void (*Hx)( const std::vector< double > &,
                                        std::vector< std::vector< double > > & ),
                            std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();

    // 収束フラグ
    bool is_converged = false;

    // 繰り返し計算回数カウンタ
    int itr_count = 0;

    // 変数の次元数
    int n = x.size();

    // 勾配ベクトル、移動量、
    vector< double > n_x( n ), dx( n );

    // ヘッセ行列
    vector< vector< double > > H_x( n, vector< double >( n ) );

    // 共役勾配方向
    vector< double > m_k1( n, 0 );

    // 係数
    double a_k = 0;
    
    // 現時点での評価値
    //double fx_val = fx( x );

    // 反復処理
    for( itr_count = 0; itr_count < _max_itr_count; itr_count++ ){
      
      // 現在位置での勾配
      nx( x, n_x );
      
      // 現在位置でのヘッセ
      Hx( x, H_x );

      // 共役勾配方向の計算
      if( itr_count > 0 ){
        double a1 = MyVecDot( m_k1, H_x * n_x );
        double a2 = MyVecDot( m_k1, H_x * m_k1 );
        assert( a2 != 0 );
        a_k = - a1 / a2;
      }
      m_k1 = n_x + a_k * m_k1;

      // 直線検索
      double t = 0;
      assert( ! runLineSearch( fx, x, m_k1, &t ) );

      // 移動量
      dx = t * m_k1;

      // 値の更新
      x = x + dx;
      
      // 収束判定評価値
      // １）移動量
      _cur_error = MyVecNorm( dx );
      // ２）評価値の変化量
      // double fx_val_new = fx( x );
      // _cur_error = MyAbs( fx_val_new - fx_val );
      // fx_val = fx_val_new;

      if( _dout && _dout_type == OutAll ){
        if( itr_count == 0 ) *_dout << "--- ConjugateGradient ---" << endl;
        *_dout << "[" << itr_count << "]\t x: " << x << "\t a_k: " << a_k
               << "\t m_k1: " << m_k1 << "\t |dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << itr_count << "]"<< endl;

      // 収束判定
      if( _cur_error < _error_thres ){
        is_converged = true;
        break;
      }
      
    }// for

    // 終了処理
    _is_converged = is_converged;
    _itr_count = itr_count;
    
    return 0;
  } 

  // /**
  //  * 共役勾配法
  //  * - ナブラとヘッセを数値微分で計算するバージョン
  //  * @param fx 評価関数
  //  * @param[in,out] x 出力値。最初は初期値を入れておく。
  //  */
  // int runConjugateGradient( double (*fx)( const std::vector< double > &),
  //                           std::vector< double > &x ){
  //   using namespace std;

  //   // 初期化、諸変数
  //   bool is_converged = false;
  //   int itr_count = 0;
  //   int n = x.size();
  //   vector< double > n_x( n ), dx( n ), m_k1( n, 0 );
  //   vector< vector< double > > H_x( n, vector< double >( n ) );
  //   double a_k = 0;
    
  //   // 反復処理
  //   for( itr_count = 0; itr_count < _max_itr_count; itr_count++ ){
      
  //     // 現在位置での勾配
  //     MyVecGrad( fx, x, n_x );
      
  //     // 現在位置でのヘッセ
  //     MyMatHessian( fx, x, H_x );

  //     // 共役勾配方向の計算
  //     if( itr_count > 0 ){
  //       double a1 = MyVecDot( m_k1, H_x * n_x );
  //       double a2 = MyVecDot( m_k1, H_x * m_k1 );
  //       assert( a2 != 0 );
  //       a_k = - a1 / a2;
  //     }
  //     m_k1 = n_x + a_k * m_k1;

  //     // 直線検索
  //     double t = 0;
  //     assert( ! runLineSearch( fx, x, m_k1, &t ) );

  //     // 移動量
  //     dx = t * m_k1;
  //     _cur_error = MyVecNorm( dx );

  //     // 値の更新
  //     x = x + dx;
      
  //     if( _dout && _dout_type == OutAll ){
  //       if( itr_count == 0 ) *_dout << "--- ConjugateGradient ---" << endl;
  //       *_dout << "[" << itr_count << "]\t x: " << x << "\t a_k: " << a_k
  //              << "\t m_k1: " << m_k1 << "\t |dx|: " << _cur_error << endl;
  //     }
  //     else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << itr_count << "]"<< endl;

  //     // 収束判定
  //     if( _cur_error < _error_thres ){
  //       is_converged = true;
  //       break;
  //     }
      
  //   }// for

  //   // 終了処理
  //   _is_converged = is_converged;
  //   _itr_count = itr_count;
    
  //   return 0;
  // }
  
  /**
   * 共役勾配法
   * - ナブラを数値微分、ヘッセをビール・ソレンソンの式で近似するバージョン
   * @param fx 評価関数
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runConjugateGradient( double (*fx)( const std::vector< double > &),
                             std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();

    // 収束フラグ
    bool is_converged = false;

    // 繰り返し計算回数カウンタ
    int itr_count = 0;

    // 変数の次元数
    int n = x.size();

    // 勾配ベクトル、移動量、
    vector< double > n_x( n ), dx( n );

    // ヘッセ行列
    vector< vector< double > > H_x( n, vector< double >( n ) );

    // 共役勾配方向
    vector< double > m_k1( n, 0 );

    // 係数
    double a_k = 0;

    // 現時点での評価値
    //double fx_val = fx( x );

    // 前ステップでの勾配ベクトル
    vector< double > n_x_last( n );
    
    // 反復処理
    for( itr_count = 0; itr_count < _max_itr_count; itr_count++ ){
      
      // 現在位置での勾配
      MyVecGrad( fx, x, n_x );
      
      // 共役勾配方向の計算
      if( itr_count > 0 ){
        double a1 = MyVecDot( n_x, n_x -  n_x_last );
        double a2 = MyVecDot( m_k1, n_x -  n_x_last );
        assert( a2 != 0 );
        a_k = - a1 / a2;
      }
      m_k1 = n_x + a_k * m_k1;

      // 直線検索
      double t = 0;
      assert( ! runLineSearch( fx, x, m_k1, &t ) );

      // 移動量
      dx = t * m_k1;

      // 値の更新
      x = x + dx;
      
      // 収束判定評価値
      // １）移動量
      _cur_error = MyVecNorm( dx );
      // ２）評価値の変化量
      // double fx_val_new = fx( x );
      // _cur_error = MyAbs( fx_val_new - fx_val );
      // fx_val = fx_val_new;

      if( _dout && _dout_type == OutAll ){
        if( itr_count == 0 ) *_dout << "--- ConjugateGradient ---" << endl;
        *_dout << "[" << itr_count << "]\t x: " << x << "\t a_k: " << a_k
               << "\t m_k1: " << m_k1 << "\t |dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << itr_count << "]"<< endl;

      // 収束判定
      if( _cur_error < _error_thres ){
        is_converged = true;
        break;
      }

      // 勾配を記憶
      n_x_last = n_x;
      
    }// for

    // 終了処理
    _is_converged = is_converged;
    _itr_count = itr_count;
    
    return 0;
  }
  
  /**
   * 準ニュートン法
   * - 関数 double fx( const vector< double > &x ) の値を最小にする入力 x を求める
   * @param fx 評価関数
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runQuasiNewton( double (*fx)( const std::vector< double > &),
                      std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();

    // 収束判定フラグ
    bool is_converged = false;

    // 繰り返し計算回数のカウンター
    int itr_count = 0;

    // 変数の次元数
    int n = x.size();

    // 勾配ベクトル、移動量
    vector< double > n_x( n ), dx( n );

    // 更新後の勾配ベクトル
    vector< double > n_x_new( n );

    // ヘッセの近似に使う行列
    vector< vector< double > > I = MyMatIdentity< double >( n );
    vector< vector< double > > Bk = MyMatIdentity< double >( n );

    // 現時点での評価値
    // double fx_val = fx( x );

    // 反復処理
    for( itr_count = 0; itr_count < _max_itr_count; itr_count++ ){

      // この位置での勾配
      MyVecGrad( fx, x, n_x );
    
      // 探索方向
      dx = -1.0 * ( Bk * n_x );

      // 直線探索
      double t = 0;
      assert( ! runLineSearch( fx, x, dx, &t ) );

      // 移動量
      dx = t * dx;

      // 値の更新
      x = x + dx;

      // 収束判定評価値
      // １）移動量
      _cur_error = MyVecNorm( dx );
      // ２）評価値の変化量
      // double fx_val_new = fx( x );
      // _cur_error = MyAbs( fx_val_new - fx_val );
      // fx_val = fx_val_new;

      if( _dout && _dout_type == OutAll ){
        if( itr_count == 0 ) *_dout << "--- QuasiNewton ---" << endl;
        *_dout << "[" << itr_count << "] " << x << " ";
        *_dout << "n_x: " << n_x << " Bk: " << Bk << " t:" << t << " "; 
        *_dout << "|dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << itr_count << "]"<< endl;
      
      // 収束判定
      if( _cur_error < _error_thres ){
        is_converged = true;
        break;
      }

      // Bk の更新
      MyVecGrad( fx, x, n_x_new );
      vector< double > yk = n_x_new - n_x;
      double a = MyVecDot( yk, dx );
      assert( a != 0 );
      double b = 1.0 / a;
      vector< vector< double > > A = I - b * ( MyVec2Mat( yk ) * MyVecTrans( dx ) );
      Bk = MyMatTrans( A ) * Bk * A + b * ( MyVec2Mat( dx ) * MyVecTrans( dx ) );
      
    }
    
    // 終了処理
    _is_converged = is_converged;
    _itr_count = itr_count;

    return 0;
  }
  
  /**
   * ガウス・ニュートン法
   * - 二乗和の形で表現された評価関数 J = f1(x)^2 + f2(x)^2 + ... の値を最小する入力 x を求める。
   * - 非線形最小二乗法
   * - 評価関数 f1, f2, ..., を vector 配列で渡す
   * - 各評価関数の勾配（ナブラ）は内部で数値微分で計算される
   * @param vfx 評価関数の配列
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  typedef double (*vec_func_type)( const std::vector< double > & );
  int runGaussNewton( const std::vector< vec_func_type > &vfx,
                      std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();
    
    int m = vfx.size();
    int n = x.size();
      
    // 反復処理
    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){

      vector< double > nf = MyVecZero< double >( n );
      vector< vector< double > > H = MyMatZero< double >( n );

      for( int i = 0; i < m; i++ ){
        vector< double > nx( n );
        MyVecGrad( vfx[ i ], x, nx );
        nf = nf - vfx[ i ]( x ) * nx;
        H = H + MyVec2Mat( nx ) * MyVecTrans( nx );
      }

      // 連立一次方程式を解く
      vector< double > dx;
      assert( ! MyLUSolve( H, dx, nf ) );

      // 移動
      x = x + dx;

      // 収束判定評価値
      _cur_error = MyVecNorm( dx );
      
      if( _dout && _dout_type == OutAll ){
        if( _itr_count == 0 ) *_dout << "--- GaussNewton ---" << endl;
        *_dout << "[" << _itr_count << "] " << x << " ";
        *_dout << "nf: " << nf << " H: " << H << " ";
        *_dout << "|dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << _itr_count << "]"<< endl;

      // 収束判定
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }
    }
    
    return 0;
  }
  
  /**
   * レーベンバーグ・マーカート法
   * - 二乗和の形で表現された評価関数 J = f1(x)^2 + f2(x)^2 + ... の値を最小する入力 x を求める。
   * - 非線形最小二乗法
   * - 評価関数 f1, f2, ..., を vector 配列で渡す
   * - 各評価関数の勾配（ナブラ）は内部で数値微分で計算される。
   * - ガウス・ニュートン法でよい解が得られない場合に使うらしい。
   * @param vfx 評価関数の配列
   * @param[in,out] x 出力値。最初は初期値を入れておく。
   */
  int runLevenbergMarquardt( const std::vector< vec_func_type > &vfx,
                             std::vector< double > &x ){
    using namespace std;

    // 初期化
    init();
    
    int m = vfx.size();
    int n = x.size();
    double c = 0.0001;

    // スタート時点での評価関数の値
    double J = 0;
    for( int i = 0; i < m; i++ ){
      double a = vfx[ i ]( x );
      J += a * a;
    }

    // 反復処理
    for( _itr_count = 0; _itr_count < _max_itr_count; _itr_count++ ){

      vector< double > nf = MyVecZero< double >( n );
      vector< vector< double > > H = MyMatZero< double >( n );
      vector< vector< double > > I = MyMatIdentity< double >( n );

      // 現在位置での勾配とヘッセ行列を求める。
      // ここはガウス・ニュートン法と同じ
      for( int i = 0; i < m; i++ ){
        vector< double > nx( n );
        MyVecGrad( vfx[ i ], x, nx );
        nf = nf - vfx[ i ]( x ) * nx;
        H = H + MyVec2Mat( nx ) * MyVecTrans( nx );
      }

      vector< double > dx;

      const int MAX_ITR_COUNT2 = 100;
      for( int k = 0; k < MAX_ITR_COUNT2; k++ ){

        // ヘッセ行列を修正しながら探索。
        // c が大きいと勾配法に近づく。
        // c が小さいとガウスニュートン法に近づく。
        H = H + c * I;
      
        // 連立一次方程式を解く
        dx.clear();
        assert( ! MyLUSolve( H, dx, nf ) );

        // 新しい位置に移動
        vector< double > x2 = x + dx;

        // 新しい位置での評価関数の値を計算
        double J2 = 0;
        //for( int i = 0; i < n; i++ ){
        for( int i = 0; i < m; i++ ){
          double a = vfx[ i ]( x2 );
          J2 += a * a;
        }

        // 値がまだ大きければ
        if( J2 > J ){
          // 勾配法に近づける＆より大きく進む
          c *= 10;
        }
        else{
          // ガウスニュートン法に近づける＆より小さく進む
          c *= 0.1;
          J = J2;
          x = x2;
          break;
        }
      }//k

      // 収束判定評価値
      _cur_error = MyVecNorm( dx );
      
      if( _dout && _dout_type == OutAll ){
        if( _itr_count == 0 ) *_dout << "--- LevenbergMarquardt ---" << endl;
        *_dout << "[" << _itr_count << "] " << x << " ";
        *_dout << "nf: " << nf << " c: " << c << " H: " << H << " ";
        *_dout << "|dx|: " << _cur_error << endl;
      }
      else if( _dout && _dout_type == OutCounterOnly ) *_dout << "[" << _itr_count << "]"<< endl;

      // 収束判定
      if( _cur_error < _error_thres ){
        _is_converged = true;
        break;
      }
    }
    
    return 0;
  }
  
};

// 静的変数のインスタンス生成
const std::vector< double > * MyMinSearch::_LineSearch_x;
const std::vector< double > * MyMinSearch::_LineSearch_dx;
double (*(MyMinSearch::_LineSearch_fx))( const std::vector< double > & );

//#########################################################################################
// 画像操作
//#########################################################################################

/**
 * 画素値の取得
 * - プレーンごと
 */
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

/**
 * 画素値のセット
 * - プレーンごと
 */
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

/**
 * 画素値のセット
 * - RGB プレーンまとめて
 */
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

/**
 * 画素値のセット
 * - 構造体で
 */
void
inline
MySetPixel( IplImage *img_in,
            int x,
            int y,
            MyColor3us cl ){
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

//#########################################################################################
// 画像処理
//#########################################################################################

/**
 * 領域膨張
 * - 指定された画像の指定された画素位置を起点として領域膨張を行う。
 * - 隣接画素のうち、起点画素との画素値の差分が RGB 共に thres 以下の画素を同一領域として扱う。
 * @param x 起点の x 座標
 * @param y 起点の y 座標
 * @param thres 同一画素と判定するための画素値の差分絶対値の閾値
 * @param [out] img_out 同一領域と判定された画素位置の画素値を 0 以外にセットした画像を返す。
 * @param pix_val 出力画像（img_out）について、領域内と判定された画素の値をいくつにするか。デフォルトは１。
 * @return 領域のサイズ（画素数）
 */

int
MyPixelExpansion( const IplImage *img_in,
                  int x,
                  int y,
                  int thres,
                  IplImage *img_out,
                  int pix_val = 1
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
    MyColor3us col( img_in, x, y );
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
          MyColor3us col2( img_in, x2, y2 );
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
              MyColor3us col2( img_in, x2, y2 );
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

/**
 * 領域膨張
 * - MyPixelExpansion のエイリアス。
 */

int MyAreaExpansion( const IplImage *img_in, int x, int y, int thres,  IplImage *img_out,
                     int pix_val = 1 ){
  return MyPixelExpansion( img_in, x, y, thres, img_out, pix_val );
}

}// my

#endif
