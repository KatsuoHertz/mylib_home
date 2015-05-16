/**
 * @file mylib_xct.hpp
 * @brief 研究テーマ特化なユーティリティ関数、クラス
 * @version 2015.5.16
 */

#ifndef MYLIB_XCT_HPP
#define MYLIB_XCT_HPP

#include "mylib_home.hpp"

namespace my
{

using namespace std;

/**
 * ima ファイルを扱うクラス
 */
class MyImaData
{
  vector< unsigned short > _dat;
  int _width;
  int _height;

 public:
  // constructors
  MyImaData( const char *filename, int width, int height ){
    ifstream inf( filename, ios::binary );
    assert( inf.is_open() );
    unsigned short *tmp_buf = new unsigned short [ width * height ];
    assert( tmp_buf != NULL );
    inf.read( (char *)tmp_buf, 2 * width * height );
    for( int y = 0; y < height; y++ ){
      for( int x = 0; x < width; x++ ){
        // ima ファイル、y 軸反転
        unsigned short mat_index = ( tmp_buf[ ( height - 1 - y ) * width  + x ] - 24 ) / 1000;
        _dat.push_back( mat_index );
			}
		}
    inf.close();
    delete [] tmp_buf;
    _width = width;
    _height = height;
  }

  // Accessors
  int width() const { return _width; };
  int height() const { return _height; };
  unsigned short get( int x, int y ) const { return _dat[ y * _width + x ]; };
};

/**
 * 各組織に関する情報
 */
class MyTissueSettings
{
  map< int, string > _names;
  map< int, double > _pe;
  map< int, double > _cs;
  map< int, vector< int > > _colors;
  vector< int > _ids;
 public:
  MyTissueSettings( const char *filename ){
    ifstream ifs( filename );
    assert( ifs.is_open() );
    while( ifs.good() ){
      string line;
      getline( ifs, line );
      vector< string > la = MySplit( line );
      if( la.size() == 7 ){
        int mat_idx = atoi( la[ 0 ].c_str() );
        string name = la[ 1 ];
        double wpe = atof( la[ 2 ].c_str() );
        double wcs = atof( la[ 3 ].c_str() );
        vector< int > color( 3 );
        color[ 0 ] = atoi( la[ 4 ].c_str() );
        color[ 1 ] = atoi( la[ 5 ].c_str() );
        color[ 2 ] = atoi( la[ 6 ].c_str() );
        _names[ mat_idx ] = name;
        _pe[ mat_idx ] = wpe;
        _cs[ mat_idx ] = wcs;
        _colors[ mat_idx ] = color;
        _ids.push_back( mat_idx );
      }
    }
    ifs.close();
  }
  string name( int mat_index ) const {
    map<int, string>::const_iterator it = _names.find( mat_index ); return it->second; }
  double pe( int mat_index ) const {
    map<int, double>::const_iterator it = _pe.find( mat_index ); return it->second; }
  double cs( int mat_index ) const {
    map<int, double>::const_iterator it = _cs.find( mat_index ); return it->second; }
  vector< int > color( int mat_index ) const {
    map<int, vector< int > >::const_iterator it = _colors.find( mat_index ); return it->second; }
  vector< int > ids() const { return _ids; }
  int size() const { return _ids.size(); }
};

/**
 * 二次元データのバイナリ入出力
 */
template< typename T >
class MyImageDat
{
  vector< T > _dat;
  int _width;
  int _height;
 public:
  MyImageDat( int width, int height, T val = 0 ){
    _dat.resize( width * height, val );
    _width = width;
    _height = height;
  }
  MyImageDat( const char *filename ){ this->load( filename ); }
  int width() const { return _width; }
  int height() const { return _height; }
  T get( int x, int y ) const { return _dat[ y * _width + x ]; }
  void get( vector< T > &dat ) const { dat = _dat; }
  void set( int x, int y, T val ){ _dat[ y * _width + x ] = val; }
  void set( const vector< T > &dat ){ _dat = dat; }
  void save( const char *filename ) const {
    ofstream ofs( filename, ios::binary );
    assert( ofs.is_open() );
    ofs.write( (char *)( &_width ), sizeof( int ) );
    ofs.write( (char *)( &_height ), sizeof( int ) );
    ofs.write( (char *)( &( _dat[ 0 ] ) ), sizeof( T ) * _width * _height );
    ofs.close();
  }
  void load( const char *filename ) {
    ifstream ifs( filename, ios::binary );
    assert( ifs.is_open() );
    ifs.read( (char *)( &_width ), sizeof( int ) );
    ifs.read( (char *)( &_height ), sizeof( int ) );
    _dat.clear();
    _dat.resize( _width * _height, 0 );
    ifs.read( (char *)( &( _dat[ 0 ] ) ), sizeof( T ) * _width * _height );
    ifs.close();
  }
};

/**
 * X線のスペクトル情報
 */
class MyXraySpect
{
  vector< vector< double > > _data;
  vector< int > _photon_count;
  double _x_step;
  double _x_start;
  double _x_last;
 public:
  MyXraySpect( const char *data_fn ){
    ifstream inf( data_fn );
    assert( inf );
    while( inf.good() ){
      string line;
      getline( inf, line );
      vector< string > la = MySplit( line );
      if( la.size() == 2 ){
        vector<double> tmp;
        tmp.push_back( atof( la[ 0 ].c_str() ) );
        tmp.push_back( atof( la[ 1 ].c_str() ) );
        _data.push_back( tmp );
      }
    }
    inf.close();
    assert( _data.size() > 1 );
    _x_step = _data[ 1 ][ 0 ] - _data[ 0 ][ 0 ];
    assert( _x_step > 0 );
    _x_start = _data[ 0 ][ 0 ];
    _x_last = _data[ _data.size() - 1 ][ 0 ];
  }
  void setPhotonTotal( int total_count ){
    // 全体の面積を計算
    double area = 0;
    for( int i = 0; i < _data.size(); i++ ) area += _data[ i ][ 1 ];
    // フォトンを比例配分
    _photon_count.clear();
    for( int i = 0; i < _data.size(); i++ ){
      _photon_count.push_back( _data[ i ][ 1 ] / area * total_count + 0.5 );
    }
  }
  // 指定されたエネルギーでのフォトンの数を返す
  int getPhotonCount( double E ) const {
    int index = ( E - _x_start ) / _x_step;
    assert( index >= 0 && index < _photon_count.size() );
    return _photon_count[ index ];
  }
  // アクセサ
  double xStart(){ return _x_start; }
  double xStep(){ return _x_step; }
  double xLast(){ return _x_last; }
};

/**
 * 各種パラメータ保持
 */
struct MyParam
{
  MyXraySpect *xray;
  int num_photons;
  int num_detectors;
  double cm_per_pixel;
  int num_projections;
  double proj_deg_step;
  vector<double> ene_bins;
  double ref_energy;
  MyParam() : xray( 0 ) { };
  MyParam( const char *filename ){
    using namespace std;
    vector< vector< string > > buf;
    MyReadTextString( filename, buf );
    for( int i = 0; i < buf.size(); i++ ){
      if( buf[ i ][ 0 ] == string( "X_RAY_SPECT_FN" ) )
        xray = new MyXraySpect( buf[ i ][ 1 ].c_str() );
      else if( buf[ i ][ 0 ] == string( "NUM_PHOTONS" ) )
        num_photons = atoi( buf[ i ][ 1 ].c_str() );
      else if( buf[ i ][ 0 ] == string( "NUM_DETECTORS" ) )
        num_detectors = atoi( buf[ i ][ 1 ].c_str() );
      else if( buf[ i ][ 0 ] == string( "CM_PER_PIXEL" ) )
        cm_per_pixel = atof( buf[ i ][ 1 ].c_str() );
      else if( buf[ i ][ 0 ] == string( "NUM_PROJECTIONS" ) )
        num_projections = atoi( buf[ i ][ 1 ].c_str() );
      else if( buf[ i ][ 0 ] == string( "PROJ_DEG_STEP" ) )
        proj_deg_step = atof( buf[ i ][ 1 ].c_str() );
      else if( buf[ i ][ 0 ] == string( "ENE_BINS" ) ){
        for( int j = 1; j < buf[ i ].size(); j++ ){
          ene_bins.push_back( atof( buf[ i ][ j ].c_str() ) );
        }
      }
      else if( buf[ i ][ 0 ] == string( "REF_ENERGY" ) )
        ref_energy = atof( buf[ i ][ 1 ].c_str() );
    }
    xray->setPhotonTotal( num_photons );
  }
  void print( ostream &out ){
    out << xray->xStart() << "\t" << xray->xStep() << "\t" << xray->xLast() << endl;
    out << "NUM_PHOTONS\t" << num_photons << endl;
    out << "NUM_DETECTORS\t" << num_detectors << endl;
    out << "CM_PER_PIXEL\t" << cm_per_pixel << endl;
    out << "NUM_PROJECTIONS\t" << num_projections << endl;
    out << "PROJ_DEG_STEP\t" << proj_deg_step << endl;
    out << "ENE_BINS\t";
    for( int i = 0; i < ene_bins.size(); i++ ) out << ene_bins[ i ] << "\t";
    out << endl;
  }
};

/**
 * ラドン変換
 * - 二次元の実数値分布に対してラドン変換を行う関数
 * - ０度から１８０度まで 
 */
int
MyRadonImg( const MyImageDat<double> *img_in,
            MyImageDat<double> *img_out
            ){
  using namespace std;
  int num_detectors = img_out->width();
  int num_projections = img_out->height();
  assert( num_projections > 0 );
  double proj_deg_step = 180.0 / num_projections;
  assert( num_detectors > 0 );
  vector<double> line_buf( num_detectors, 0 );
  int img_width = img_in->width();
  int img_height = img_in->height();
  // 各投影毎に
  for( int i = 0; i < num_projections; i++ ){
    // 角度
    double theta = i * proj_deg_step * M_PI / 180.0;
    double sin_th = sin( theta );
    double cos_th = cos( theta );
    // ラインバッファの初期化
    fill( line_buf.begin(), line_buf.end(), 0 );
    // 各画素ごとに
    for( int y = 0; y < img_height; y++ ){
      for( int x = 0; x < img_width; x++ ){
        // この画素位置での値
        double val_in = img_in->get( x, y );
        // 各画素を２x２のサブピクセルに分ける
        for( int j = 0; j < 2; j++ ){
          for( int k = 0; k < 2; k++ ){
            double x2 = x + 0.5 * j;
            double y2 = y + 0.5 * k;
            // 中心位置移動
            //x2 -= img_width / 2;
            //y2 -= img_height / 2;
            x2 -= img_width / 2 - 0.75;//octaveのradon()に合わせる場合
            y2 -= img_height / 2 - 0.75;//octaveのradon()に合わせる場合
            // 投影位置計算
            //double Y = - sin_th * x2 + cos_th * y2;//当初
            double Y = cos_th * x2 - sin_th * y2;//octaveのradon()に合わせた
            // オフセット
            Y = Y + num_detectors / 2;
            assert( Y >= 0 && Y < num_detectors );
            // 最も近い２つの bin にサブピクセルの値を分割
            int Y1 = (int)Y;
            // 比例配分する
            double a = Y - Y1;
            if( Y1 < line_buf.size() ) line_buf[ Y1 ] += ( 1 - a ) * val_in / 4;
            if( Y1 + 1 < line_buf.size() ) line_buf[ Y1 + 1 ] += a * val_in / 4;
            // if( Y != Y1 ){
            //   // サブピクセルの値を均等分割＝元の値の 1/8
            //   line_buf[ Y1 ] += val_in / 8;
            //   line_buf[ Y1 + 1 ] += val_in / 8;
            // }
            // else{
            //   // ちょうど bin の中央に投影される場合
            //   line_buf[ Y1 ] += val_in / 4;
            // }
          }//k
        }//j
      }//x
    }//y
    // バッファにしまう
    for( int j = 0; j < num_detectors; j++ ){
      img_out->set( j, i, line_buf[ j ] );
    }//j
  }//i
  return 0;
}

/**
 * Klein-Nishina の関数
 */
double MyfKN( double a ){
  double a1 = 1 + a;
  double a2 = a * a;
  double a12 = 1 + 2 * a;
  double log_a12 = log( a12 );
	return a1 / a2 * ( 2 * a1 / a12 - log_a12 / a )
      + log_a12 / ( 2 * a ) - ( 1 + 3 * a ) / ( a12 * a12 );
}

/**
 * サイノグラム計算
 * - 与えられた線吸収係数の分布からサイノグラムの各画素で観測されるフォトンの数を計算する。
 * - どのエネルギー帯で計算するかインデックスを与える。
 */
int
MyCalcSino( const MyImageDat<double> *img_pe,
            const MyImageDat<double> *img_cs,
            const MyParam *param,
            int ene_bin_index,
            MyImageDat<double> *img_out
            ){
  // 入力チェック
  assert( img_pe != NULL );
  assert( img_cs != NULL );
  assert( param != NULL );
  assert( img_out != NULL );
  assert( ene_bin_index < param->ene_bins.size() - 1 );
  assert( img_pe->width() == img_cs->width() );
  assert( img_pe->height() == img_cs->height() );
  assert( img_out->width() == param->num_detectors );
  assert( img_out->height() == param->num_projections );
  // 計算
  MyImageDat<double> *img_pe_sino = new MyImageDat<double>( param->num_detectors,
                                                            param->num_projections );
  MyImageDat<double> *img_cs_sino = new MyImageDat<double>( param->num_detectors,
                                                            param->num_projections );
  MyRadonImg( img_pe, img_pe_sino );
  MyRadonImg( img_cs, img_cs_sino );
  double E1 = param->ene_bins[ ene_bin_index ];
  double E2 = param->ene_bins[ ene_bin_index + 1 ];
  double dE = param->xray->xStep();
  double E0 = param->ref_energy;
  vector< double > f_pe, f_cs, n_E;
  for( double E = E1; E < E2; E += dE ){
    f_pe.push_back( ( E0 * E0 * E0 ) / ( E * E * E ) );
    f_cs.push_back( MyfKN( E / 510.975 ) / MyfKN( E0 / 510.975 ) );
  }
  // サイノグラムの各画素ごとに
  for( int y = 0; y < img_out->height(); y++ ){
    for( int x = 0; x < img_out->width(); x++ ){
      // 各エネルギーごとの吸収係数の計算＆積分
      int idx = 0;
      double sum = 0;
      double val_pe = img_pe_sino->get( x, y );
      double val_cs = img_cs_sino->get( x, y );
      for( double E = E1; E < E2; E += dE, idx++ ){
        // 吸収係数
        double mu = exp( - ( f_pe[ idx ] * val_pe + f_cs[ idx ] * val_cs ) * param->cm_per_pixel );
        // 入射X線のフォトンの数
        double I0 = param->xray->getPhotonCount( E );
        // 透過後のフォトンの数
        double I = I0 * mu;
        // 積分
        sum += I;
      }//E
      // 書き込み
      img_out->set( x, y, sum );
    }//x
  }//y
  delete img_pe_sino;
  delete img_cs_sino;
  return 0;
}

}//namespace

#endif
