// QR_Encode.h : CQR_Encode クラス宣言およびインターフェイス定義
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

#ifndef AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_
#define AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_



// 誤り訂正レベル
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// データモード
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// バージョン(型番)グループ
#define QR_VRESION_S	0 // 1 ～ 9
#define QR_VRESION_M	1 // 10 ～ 26
#define QR_VRESION_L	2 // 27 ～ 40

#define MAX_ALLCODEWORD	 3706 // 
#define MAX_DATACODEWORD 2956 // 
#define MAX_CODEBLOCK	  153 // 
#define MAX_MODULESIZE	  177 // 

// ビットマップ描画時マージン
#define QR_MARGIN	1
#define min(x,y) ((x)<(y)?(x):(y))

/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
    int ncRSBlock;		// ＲＳブロック数
    int ncAllCodeWord;	// ブロック内コードワード数
    int ncDataCodeWord;	// データコードワード数(コードワード数 - ＲＳコードワード数)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QRコードバージョン(型番)関連情報

typedef struct tagQR_VERSIONINFO
{
    int nVersionNo;	   // バージョン(型番)番号(1～40)
    int ncAllCodeWord; // 総コードワード数

    // 以下配列添字は誤り訂正率(0 = L, 1 = M, 2 = Q, 3 = H) 
    int ncDataCodeWord[4];	// データコードワード数(総コードワード数 - ＲＳコードワード数)

    int ncAlignPoint;	// アライメントパターン座標数
    int nAlignPoint[6];	// アライメントパターン中心座標

    RS_BLOCKINFO RS_BlockInfo1[4]; // ＲＳブロック情報(1)
    RS_BLOCKINFO RS_BlockInfo2[4]; // ＲＳブロック情報(2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode クラス

class CQR_Encode
{
    // 構築/消滅
public:
    CQR_Encode();
    ~CQR_Encode();

public:
    int m_nLevel;		// 
    int m_nVersion;		// 
    bool m_bAutoExtent;	// 
    int m_nMaskingNo;	// 

public:
    int m_nSymbleSize;
    unsigned char m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
    // bit5:
    // bit4:
    // bit1:
    // bit0:
    // 20h

private:
    int m_ncDataCodeWordBit; // 
    unsigned char m_byDataCodeWord[MAX_DATACODEWORD]; // 

    int m_ncDataBlock;
    unsigned char m_byBlockMode[MAX_DATACODEWORD];
    int m_nBlockLength[MAX_DATACODEWORD];

    int m_ncAllCodeWord; //
    unsigned char m_byAllCodeWord[MAX_ALLCODEWORD]; // 
    unsigned char m_byRSWork[MAX_CODEBLOCK]; // 

    // 
public:
    bool EncodeData(int nLevel, int nVersion, bool bAutoExtent, int nMaskingNo, char * lpsSource, int ncSource = 0);

private:
    int GetEncodeVersion(int nVersion, char * lpsSource, int ncLength);
    bool EncodeSourceData(char * lpsSource, int ncLength, int nVerGroup);

    int GetBitLength(unsigned char nMode, int ncData, int nVerGroup);

    int SetBitStream(int nIndex, unsigned short wData, int ncData);

    bool IsNumeralData(unsigned char c);
    bool IsAlphabetData(unsigned char c);
    bool IsKanjiData(unsigned char c1, unsigned char c2);

    unsigned char AlphabetToBinaly(unsigned char c);
    unsigned short KanjiToBinaly(unsigned short wc);

    void GetRSCodeWord(unsigned char* lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

    // モジュール配置関連ファンクション
private:
    void FormatModule();

    void SetFunctionModule();
    void SetFinderPattern(int x, int y);
    void SetAlignmentPattern(int x, int y);
    void SetVersionPattern();
    void SetCodeWordPattern();
    void SetMaskingPattern(int nPatternNo);
    void SetFormatInfoPattern(int nPatternNo);
    int  CountPenalty();

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
